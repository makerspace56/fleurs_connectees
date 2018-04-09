#include <Wire.h>
#include <Servo.h>
/*
 * PIR sensor tester
 */
int calibrationTime = 5;

int inputPin = 8;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int premTour = 0 ;              // variable pour une boucle de placement au debut

//Ginette : grande avec 3 servos
#define SLAVE_ADDRESS 0x13
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    1400
#define SERVO_TETE_MAX    2200
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 900
#define SERVO_PETALES_MAX 1650

#define SERVO_TIGE_PIN    9
#define SERVO_TIGE_INIT   1600
#define SERVO_TIGE_MIN    1000
#define SERVO_TIGE_MAX    2100
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN 5000

// Servos
Servo servoTete;
Servo servoPetales;
Servo servoTige;

// parcourtServo : passe le servo concerné, le debut et la fin.
// Passe le nom du servo vers l'object servo a travers &servo

void parcourtServo(Servo &servo, int depart, int fin, int vitesse) {
  if (depart > fin) {
    for (int pos = depart ; pos >= fin ; pos--) {
      servo.write(pos);
      delay(vitesse);
    }
  } else {
    for (int pos = depart ; pos <= fin ; pos++) {
      servo.write(pos);
      delay(vitesse);
    }
  }
}

// données envoyé i2c
byte x = 0;

// BUS I2C : envoi de données
void envoiEvent() {
  Serial.println("envoie");
  Wire.beginTransmission(0x10);  // envoi vers adress esclave 0x10
  Wire.write(x);              // envoi 1 byte
  Wire.endTransmission();    // stop transmition
  Wire.beginTransmission(0x11);  // envoi vers adress esclave 0x11
  Wire.write(x);              // envoi 1 byte
  Wire.endTransmission();    // stop transmition
  delay(100);
}

// Led temoin rouge : ALIM
int ledTemoin = 2;
// Led temoin Verte : CAPTEUR
int ledPinV = 3;                // choose the pin for the LED

void setup() {
  // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin
  digitalWrite(ledTemoin, HIGH);
  pinMode(ledPinV, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input

  Serial.begin(9600);

  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
   }
   
  // Servo des petales qui s'ouvre et qui se ferme
  servoPetales.attach(SERVO_PETALES_PIN);
  // Servo de la tete qui se penche vers la personne
  servoTete.attach(SERVO_TETE_PIN);
  //servo de la tige qui se dirige vers les autres fleurs
  servoTige.attach(SERVO_TIGE_PIN);

  // Bus I2C
  Wire.begin(0x10);
  Wire.begin(0x11);
  //Wire.onReceive(receiveData);
  //Wire.onRequest();
}

void loop() {
  val = digitalRead(inputPin);// read input value
  Serial.println(val);
  
  int Tete_Moy = (SERVO_TETE_MIN+SERVO_TETE_MAX)/2; // esperons que ce soit l'horizontale
  int Petales_Moy = (SERVO_PETALES_MIN+SERVO_PETALES_MAX)/2; // a demi fermee

// PLACEMENT INITIAL 
if(premTour == 0){ // la on pourrait prevoir un truc modulo pour qu'elle se replace toutes les 65 danses? 
          // placement des servo en position "depart"
    servoPetales.write(SERVO_PETALES_MAX); // ferme cette fleur
    servoTete.write(Tete_Moy);
    servoTige.write(SERVO_TIGE_INIT);
    premTour = 1; 
} 
// LECTURE CAPTEUR > REACTION
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPinV, HIGH);  // turn LED ON
    envoiEvent();
    Serial.println("Animation !");
    // Execute le programme :
// LA DANSE  

// se tourne vers sa voisine de gauche 
   parcourtServo(servoTige, SERVO_TIGE_INIT, SERVO_TIGE_MAX, 2);
   
// se penche vers sa voisine, attend
    parcourtServo(servoTete,Tete_Moy, SERVO_TETE_MIN, 3);
    delay(500);
      Serial.println("Ouverture / fermeture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN,2);
         delay(500);
      parcourtServo(servoPetales, SERVO_PETALES_MIN,SERVO_PETALES_MAX, 2);
 
// se redresse
 parcourtServo(servoTete, SERVO_TETE_MIN, SERVO_TETE_MAX, 3);
    
    // se tourne vers sa voisine de dr
    delay(500);
     Serial.println("Ouverture / fermeture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN,2);
         delay(500);
      parcourtServo(servoPetales, SERVO_PETALES_MIN,SERVO_PETALES_MAX, 2);
    // se redresse
 parcourtServo(servoTete, SERVO_TETE_MAX, Tete_Moy, 5);
    // revient au centre
    parcourtServo(servoTige, SERVO_TIGE_MAX, SERVO_TIGE_INIT, 5); 
    // - elle s'ouvre et se ferme 3x & de plus en plus vite
    for (int i = 0; i < 3; i++) {
      Serial.println("Ouverture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 7-2*i);
      Serial.println("Fermeture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MIN,SERVO_PETALES_MAX,7-2*i);
    }
    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale"); 
    
    parcourtServo(servoPetales, SERVO_PETALES_MAX, Petales_Moy, 5);
    parcourtServo(servoTige, SERVO_TIGE_INIT, SERVO_TIGE_MIN, 5);
    // s'ouvre
    parcourtServo(servoPetales, Petales_Moy, SERVO_PETALES_MIN, 5);
    // balance la tete
       parcourtServo(servoTete, Tete_Moy, SERVO_TETE_MAX, 5);
    parcourtServo(servoTete, SERVO_TETE_MAX, SERVO_TETE_MIN, 5);
    parcourtServo(servoTete, SERVO_TETE_MIN, Tete_Moy, 5);
    // se referme doucement
    parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 10);
    // retour tige
    parcourtServo(servoTige, SERVO_TIGE_MIN, SERVO_TIGE_INIT, 10);
    
    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(10000);

    if (pirState == LOW) {   // on est toujours dans le Si on a lu un mouvement
      pirState = HIGH; // donc on passe la donnée lue a la variable memoire
    }
  } else { // si aucun mouvement n'est lu sur le sensor
    digitalWrite(ledPinV , LOW); // turn LED OFF
    if (pirState == HIGH) { // 
      Serial.println("Fin du mvmt");    //
      pirState = LOW; // on passe la donnée lue a la variable memoire
    }
  }
   // - Attente entre 2 mesures
    delay(1000);
}
