/* 
Un programme pour tester une petite fleur sans capteur 
et sans qu'elle soit branchée à sa grande. 
La variable de lancement change automatiquement à chaque cycle, 
espacé par le délai CHORE_ATTENTE_FIN en millisecondes : 
La fleur danse puis se repose puis redanse puis se repose... etc à l'infini

LED Témoin d'alimentation : ledTemoin

*/

#include <Servo.h>

// Déclaration des Pin Moteurs et de leurs valeurs MIN / MAX
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    600
#define SERVO_TETE_MAX    1800
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 600
#define SERVO_PETALES_MAX 1800
// Déclaration de variable pour la chorégraphie
#define CHORE_OUV_FER_NB  2
#define CHORE_ATTENTE_FIN 2000
bool chore = false;

// Led temoin
int ledTemoin = 2; // choix du pin 

// variable pour une boucle de placement au debut
int premTour = 0 ;          

// déclaration des Servos
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
/*
  // Bus I2C
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(sendData);
*/
  // Initialise la liaison série pour debug
  Serial.begin(9600);
}
void  loop() {

  int Tete_Moy = (SERVO_TETE_MIN+SERVO_TETE_MAX)/2; // esperons que ce soit l'horizontale
  int Petales_Moy = (SERVO_PETALES_MIN+SERVO_PETALES_MAX)/2; // a demi fermee

// PLACEMENT INITIAL 
if(premTour == 0){ // la on pourrait prevoir un truc modulo pour qu'elle se replace toutes les 65 danses? 
          // placement des servo en position "depart"
    servoPetales.write(SERVO_PETALES_MAX); // ferme cette fleur
    servoTete.write(Tete_Moy);
    premTour = 1; 
} 
  
  if (chore == true) {
    chore = false;
    // Animation
    Serial.println("Animation !");
    digitalWrite(ledTemoin, HIGH);  // turn LED msg OFF
    // Execute le programme :
// delay(9250); // le temps que Ginette se tourne vers elle ? 
    // - elle s'ouvre 
    delay(1500);
     Serial.println("Petales !");
    parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 5);
    parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 2);

  delay(1500);
     Serial.println("Tete !");
// elle s'incline
  parcourtServo(servoTete, Tete_Moy, SERVO_TETE_MIN, 5);
  parcourtServo(servoTete, SERVO_TETE_MIN, SERVO_TETE_MAX, 2 );
  
  parcourtServo(servoTete, SERVO_TETE_MAX, Tete_Moy, 10);

    // - Attente pour s'économiser un peu après une danse
    delay(CHORE_ATTENTE_FIN);
  }
  else{ // si on ne reçoit rien
      digitalWrite(ledTemoin, LOW);  // turn LED msg OFF
      chore = true; 
      
    delay(CHORE_ATTENTE_FIN);
  }
}
