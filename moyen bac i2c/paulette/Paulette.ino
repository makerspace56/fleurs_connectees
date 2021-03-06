                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
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
//

// 2 servos - 1 Module de capteur infrarouge d'évitement d'obstacles (https://e.banana-pi.fr/distance/121-module-de-capteur-infrarouge-d-evitement-d-obstacles.html)

// Scenario :
// - elle s'oriente vers la personne
// - s'ouvre et se ferme 3x
// - puis de se remet en position initiale

#include <Wire.h>
#include <Servo.h>

// Paulette: petite 2 servos
#define SLAVE_ADDRESS 0x10
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    1100
#define SERVO_TETE_MAX    1600
#define SERVO_PETALES_PIN 9
#define SERVO_PETALES_MIN 1100
#define SERVO_PETALES_MAX 1800
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN 5000  

// Led temoin
int ledTemoin = 2; 

// données envoyé i2c
byte x = 0;

bool chore = false;


// Capteur
//int inputCapteur = 8;

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

void receiveEvent(int howMany) {
  while (Wire.available() != 0) { // loop through all but the last
    int v = Wire.read(); // receive byte as a character
    switch(v){
      case 0:
        chore = true;
        Serial.print("Chore #1");
        break;
     default:
        Serial.print("Code inconnu");
        break;
    }
  }
}


// BUS I2C : envoi de données
//void envoiEvent(){
//  Wire.beginTransmission(0x10);
//  Wire.beginTransmission(0x11);  // envoi vers adress esclave 0x12
//  Wire.write(x);              // envoi 1 byte
//  Wire.endTransmission();    // stop transmition
//  delay(100);
//}

void  setup()
{
  // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin
  digitalWrite(ledTemoin, HIGH);
  

  
 

 // Servo des petales qui s'ouvre et qui se ferme
  servoPetales.attach(SERVO_PETALES_PIN);
// Servo de la tete qui se penche vers la personne                
  servoTete.attach(SERVO_TETE_PIN);
//servo de la tige qui se dirige vers les autres fleurs
  //servoTige.attach(SERVO_TIGE_PIN);

  // Bus I2C
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(sendData);

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}
void  loop(){
   if (chore){
    chore = false;
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
    delay(5000);
    delay(CHORE_ATTENTE_FIN);
   }
  }


