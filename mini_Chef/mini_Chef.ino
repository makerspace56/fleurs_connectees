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
  servoPetales.attach(11);
  
  // Servo de la tete qui se penche vers la personne                
  servoTete.attach(10);

  //servo de la tige qui se dirige vers les autres fleurs
  servoTige.attach(9);

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}

void  loop()
{
    Serial.println("Animation !");
    // Execute le programme :
    // - elle se dirige vers la personne
    Serial.println("Révérence");
    parcourtServo(servoTete, 1000, 1000, 5);
    parcourtServo(servoPetales, 1800, 1200, 5);
      
    
    
    // - elle s'ouvre et se ferme 3x 
    for (int i = 0; i < 3; i++) {
      Serial.println("Ouverture pétales");
      parcourtServo(servoPetales, 900, 1800, 5);
      Serial.println("Fermeture pétales");
      parcourtServo(servoPetales, 1800, 900, 2);
    }
  // - elle passe en revu toute les autres fleurs de droite a gauche
      Serial.println("se dirige vers le premier de file");
      parcourtServo(servoTige, 1550, 900, 5);
      Serial.println("commence l'inspection jusqu'a l'autre bout");

     
      
    
    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale");
    parcourtServo(servoTete, 1000, 1000, 5);
    parcourtServo(servoPetales, 1200, 1800, 5);
   

    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(5000);
  }


