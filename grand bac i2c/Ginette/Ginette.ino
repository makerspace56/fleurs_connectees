#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

//capteur ultrason
#define TRIGGER_PIN  8  //  Arduino pin relié au trigger pin du capteur ultrason
#define ECHO_PIN     7  // Arduino pin relié a l'echo pin du capteur ultrason
#define MAX_DISTANCE 200 // Maximum distance que l'on veut "capter"
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.



//Ginette : grande avec 3 servos
#define SLAVE_ADDRESS 0x13
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    600
#define SERVO_TETE_MAX    800
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 800
#define SERVO_PETALES_MAX 1500
#define SERVO_TIGE_PIN    9
#define SERVO_TIGE_INIT   1500
#define SERVO_TIGE_MIN    900
#define SERVO_TIGE_MAX    1500
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN 5000 


// Paulette : petite 2 servos
//#define SLAVE_ADDRESS 0x10
//#define SERVO_TETE_PIN    10
//#define SERVO_TETE_MIN    750
//#define SERVO_TETE_MAX    1250
//#define SERVO_PETALES_PIN 11
//#define SERVO_PETALES_MIN 800
//#define SERVO_PETALES_MAX 1600
//#define CHORE_OUV_FER_NB  3
//#define CHORE_ATTENTE_FIN 5000  

// Arlette: petite 2 servos
//#define SLAVE_ADDRESS 0x11
//#define SERVO_TETE_PIN    10
//#define SERVO_TETE_MIN    750
//#define SERVO_TETE_MAX    1250
//#define SERVO_PETALES_PIN 11
//#define SERVO_PETALES_MIN 800
//#define SERVO_PETALES_MAX 1600
//#define CHORE_OUV_FER_NB  3
//#define CHORE_ATTENTE_FIN 5000  


// Servos
Servo servoTete;  
Servo servoPetales;
Servo servoTige;

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
// données envoyé i2c
byte x = 0;



// BUS I2C : réception de données
//void receiveEvent(int howMany) {
//  while (1 < Wire.available()) { // loop through all but the last
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
//  int x = Wire.read();    // receive byte as an integer
//  Serial.println(x);         // print the integer
//}

// BUS I2C : envoi de données
void envoiEvent(){
  Serial.println("envoie");
   Wire.beginTransmission(0x10);  // envoi vers adress esclave 0x12
  Wire.write(x);              // envoi 1 byte
  Wire.endTransmission();    // stop transmition
  Wire.beginTransmission(0x11);  // envoi vers adress esclave 0x12
  Wire.write(x);              // envoi 1 byte
  Wire.endTransmission();    // stop transmition
  delay(100);
}


// Led temoin
int ledTemoin = 2;

void setup() {
  Serial.begin(9600);
 // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin 
  digitalWrite(ledTemoin, HIGH);
 

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

  unsigned int uS = sonar.ping();
  unsigned int USs = (uS / US_ROUNDTRIP_CM);// envoi de ping, reception de ping, temp en  microseconds (uS).
  delay(500);  // attente 500ms entre 2 pings (about 2 piings

  Serial.print("Ping: ");
  Serial.print(USs); // afficher le resultats des pings recu en cm 
  Serial.println("cm");



  

  if  (USs <= 15 && USs >= 1)
  {
    envoiEvent();
    Serial.println("Animation !");
    // Execute le programme :
    // - elle se dirige vers la personne
    Serial.println("Révérence");
    parcourtServo(servoTete, SERVO_TETE_MIN, SERVO_TETE_MAX, 5);
    parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 5);
      
    
    
    // - elle s'ouvre et se ferme 3x 
    for (int i = 0; i < 3; i++) {
      Serial.println("Ouverture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 5);
      Serial.println("Fermeture pétales");
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 2);
    }
     parcourtServo(servoTige, SERVO_TIGE_INIT, SERVO_TIGE_MAX, 5);
     parcourtServo(servoTige, SERVO_TIGE_MAX, SERVO_TIGE_MIN, 5);
     
     parcourtServo(servoTige, SERVO_TIGE_MIN, SERVO_TIGE_INIT, 5);
  

     
      
    
    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale");
    parcourtServo(servoTete, SERVO_TETE_MAX, SERVO_TETE_MIN, 5);
    parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 5);
   

    // - Attente pour s'économiser un peu qd qq active le capteur
    
  }
  }



