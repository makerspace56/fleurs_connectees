#include <Wire.h>
#include <Servo.h>
/*
 * PIR sensor tester
 */
int calibrationTime = 5;
 
int ledPin = 3;                // choose the pin for the LED
int inputPin = 8;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status


//Ginette : grande avec 3 servos
#define SLAVE_ADDRESS 0x13
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    1200
#define SERVO_TETE_MAX    1800
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 1300
#define SERVO_PETALES_MAX 230²&3²2
#define SERVO_TIGE_PIN    9
#define SERVO_TIGE_INIT   1500
#define SERVO_TIGE_MIN    900
#define SERVO_TIGE_MAX    1500
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN 5000

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


// BUS I2C : envoi de données
void envoiEvent() {
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
   // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin
  digitalWrite(ledTemoin, HIGH);
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);
  
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
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
 
void loop(){
  val = digitalRead(inputPin);// read input value
  Serial.println(val);
  
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
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
      parcourtServo(servoPetales, SERVO_PETALES_MAX * 0.5, SERVO_PETALES_MIN, 2);
    }
    parcourtServo(servoTige, SERVO_TIGE_INIT, SERVO_TIGE_MAX, 5);
    parcourtServo(servoTige, SERVO_TIGE_MAX, SERVO_TIGE_MIN, 5);

    parcourtServo(servoTige, SERVO_TIGE_MIN, SERVO_TIGE_INIT, 5);





    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale");
    parcourtServo(servoTete, SERVO_TETE_MAX, SERVO_TETE_MIN, 5);
    parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 5);


    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(100);
    
    
    if (pirState == LOW) {   // we have just turned on
      
      Serial.println("Motion detected!");  // We only want to print on the output change, not state
      
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    
    if (pirState == HIGH){  // we have just turned of
      
      Serial.println("Motion ended!");    // We only want to print on the output change, not state
      
      pirState = LOW;
    }
  }
}
