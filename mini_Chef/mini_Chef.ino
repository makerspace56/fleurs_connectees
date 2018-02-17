// 
//      ,---.    ,---..-./`) ,---.   .--..-./`)             _______   .---.  .---.     .-''-.   ________  
//      |    \  /    |\ .-.')|    \  |  |\ .-.')           /   __  \  |   |  |_ _|   .'_ _   \ |        | 
//      |  ,  \/  ,  |/ `-' \|  ,  \ |  |/ `-' \          | ,_/  \__) |   |  ( ' )  / ( ` )   '|   .----' 
//      |  |\_   /|  | `-'`"`|  |\_ \|  | `-'`"`        ,-./  )       |   '-(_{;}_). (_ o _)  ||  _|____  
//      |  _( )_/ |  | .---. |  _( )_\  | .---.         \  '_ '`)     |      (_,_) |  (_,_)___||_( )_   | 
//      | (_ o _) |  | |   | | (_ o _)  | |   |          > (_)  )  __ | _ _--.   | '  \   .---.(_ o._)__| 
//      |  (_,_)  |  | |   | |  (_,_)\  | |   |         (  .  .-'_/  )|( ' ) |   |  \  `-'    /|(_,_)     
//      |  |      |  | |   | |  |    |  | |   |          `-'`-'     / (_{;}_)|   |   \       / |   |      
//      '--'      '--' '---' '--'    '--' '---'            `._____.'  '(_,_) '---'    `'-..-'  '---'      
                                                                                                  
//                                       

//3 servos 

// Scenario :
// - cette fleurs c'est la "MAMAN"
// - elle part de la position normal puis se penche s'ouvre et se ferme 3x 
// - ensuite elle "regarde" vers les autres fleurs au nombre de 6 de droite a gauche 
//   et les passe en revu telle un colonel devant ses troupes
// - puis de gauche a droite repasse devant les fleurs et marque un arret sur
// - enfin de se remet en position initiale

#include <Servo.h>
// mini chef la bleue grande 
//#define SLAVE_ADDRESS 0x13
//#define SERVO_TETE_PIN    10
//#define SERVO_TETE_MIN    600
//#define SERVO_TETE_MAX    800
//#define SERVO_PETALES_PIN 11
//#define SERVO_PETALES_MIN 1600
//#define SERVO_PETALES_MAX 2100
//#define SERVO_TIGE_PIN    9
//#define SERVO_TIGE_MIN    900
//#define SERVO_TIGE_MAX    2300
//#define CHORE_OUV_FER_NB  3
//#define CHORE_ATTENTE_FIN 5000 

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

// Led temoin
int ledTemoin = 3;

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
  servoTige.attach(SERVO_TIGE_PIN);
  servoTige.write(SERVO_TIGE_INIT);

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}

void  loop()
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
    delay(5000);
  }


