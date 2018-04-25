
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

// 2 servos - à lancer sans capteur 

// Scenario : 
#include <Servo.h>

//Ginette : grande avec 3 servos
//#define SLAVE_ADDRESS 0x13

// Paulette: petite 2 servos
//#define SLAVE_ADDRESS 0x10

// Arlette: petite 2 servos + voisine de Droite (face technique, gauche face avant)
#define SLAVE_ADDRESS 0x11
#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    600
#define SERVO_TETE_MAX    1800
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 600
#define SERVO_PETALES_MAX 1800
#define CHORE_OUV_FER_NB  2
#define CHORE_ATTENTE_FIN 2000


// Led temoin Rouge : ALIM
int ledTemoin = 2; // choix du pin 

// données envoyé i2c
byte x = 0;

bool chore = false;

int premTour = 0 ;              // variable pour une boucle de placement au debut

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

/* 
void receiveEvent(int howMany) {
  while (Wire.available() != 0) { // loop through all but the last
    int v = Wire.read(); // receive byte as a character
    switch (v) {
      case 0:
        chore = true;
         digitalWrite(ledPinV, HIGH);  // turn LED ON
        Serial.print("Chore #1");
        break;
      default:
        Serial.print("Code inconnu");
        break;
    }
  }
} 
*  
*/

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

    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(2000);
  }
  else{ // si on ne reçoit rien
      digitalWrite(ledTemoin, LOW);  // turn LED msg OFF
      chore = true; 
      
    delay(CHORE_ATTENTE_FIN);
  }
}
