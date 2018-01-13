//
//  ,---.    ,---..-./`) ,---.   .--..-./`)
//  |    \  /    |\ .-.')|    \  |  |\ .-.')
//  |  ,  \/  ,  |/ `-' \|  ,  \ |  |/ `-' \ 
//  |  |\_   /|  | `-'`"`|  |\_ \|  | `-'`"`
//  |  _( )_/ |  | .---. |  _( )_\  | .---.
//  | (_ o _) |  | |   | | (_ o _)  | |   |
//  |  (_,_)  |  | |   | |  (_,_)\  | |   |
//  |  |      |  | |   | |  |    |  | |   |
//  '--'      '--' '---' '--'    '--' '---'
//

// 2 servos - 1 Module de capteur infrarouge d'évitement d'obstacles (https://e.banana-pi.fr/distance/121-module-de-capteur-infrarouge-d-evitement-d-obstacles.html)

// Scenario :
// - elle s'oriente vers la personne
// - s'ouvre et se ferme 3x
// - puis de se remet en position initiale

#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 0x13
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    750
#define SERVO_TETE_MAX    1250
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 1200
#define SERVO_PETALES_MAX 1600
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN 5000  

// Led temoin
int ledTemoin = 2;
int dataReceived = 0;

// Capteur
int inputCapteur = 8;

// Servos
Servo servoTete;
Servo servoPetales;

// parcourtServo : passe le servo concerné, le debut et la fin.
// Passe le nom du servo vers l'object servo a travers &servo
void parcourtServo( Servo &servo, int depart, int fin, int vitesse) {

  //if (vitesse <=1 ) {vitesse == 1; }
  if (depart > fin)
  {
    for (int pos = depart ; pos >= fin ; pos--)
    {
      servo.write(pos);
      delay(vitesse);
    }
  }
  else
  {
    for (int pos = depart ; pos <= fin ; pos++)
    {
      servo.write(pos);
      delay(vitesse);
    }
  }
}

// BUS I2C : réception de données
void receiveData(int byteCount) {
  while (Wire.available()) {
    dataReceived = Wire.read();
    Serial.print("Donnee recue : ");
    Serial.println(dataReceived);
  }
}

// BUS I2C : envoi de données
void sendData() {
  int envoi = dataReceived ;
  Wire.write(envoi);
}

void  setup()
{
  // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin
  digitalWrite(ledTemoin, HIGH);

  // Le pin du capteur est INPUT
  pinMode(inputCapteur, INPUT);

  // Servo des petales qui s'ouvre et qui se ferme
  servoPetales.attach(SERVO_PETALES_PIN);

  // Servo de la tete qui se penche vers la personne
  servoTete.attach(SERVO_TETE_PIN);

  // Bus I2C
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}

void  loop()
{
  // Lit la valeur digital et la place dans la variable val
  int val = digitalRead(inputCapteur);
  Serial.print("val = "); Serial.println(val);

  // Quand le capteur detecte un mouvement => la varible se met HIGH
  if (val == HIGH || dataReceived == 1)
  {
    // Reset dataReceived
    dataReceived = 0;

    // Animation
    Serial.println("Animation !");
    // Execute le programme :
    // - elle se dirige vers la personne
    Serial.println("Révérence");
    parcourtServo(servoTete, SERVO_TETE_MIN, SERVO_TETE_MAX, 5);

    // - elle s'ouvre et se ferme CHORE_OUV_FER_NB fois
    for (int i = 0; i < CHORE_OUV_FER_NB; i++) {
      Serial.println("Ouverture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 5);
      Serial.println("Fermeture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 2);
    }

    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale");
    parcourtServo(servoTete, SERVO_TETE_MAX, SERVO_TETE_MIN, 5);

    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(CHORE_ATTENTE_FIN);
  }
}

