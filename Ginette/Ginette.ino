#include <Servo.h>
#include <NewPing.h>

// mini chef la bleue moyenne
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



#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// Led temoin
int ledTemoin = 3;

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



}
void loop() {

  unsigned int uS = sonar.ping();
  unsigned int USs = (uS / US_ROUNDTRIP_CM);// Send ping, get ping time in microseconds (uS).
  delay(500);  // Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.

  Serial.print("Ping: ");
  Serial.print(USs); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.println("cm");



  

  if  (USs <= 15 && USs >= 1)
  {
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



