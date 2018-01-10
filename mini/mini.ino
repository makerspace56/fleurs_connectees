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

#include <Servo.h>

// Led temoin
int ledTemoin = 2; 
         
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

void  setup()
{
  // Le pin de la led est OUTPUT
  pinMode(ledTemoin, OUTPUT);
  // Allume la led temoin 
  digitalWrite(ledTemoin, HIGH);
  
  // Le pin du capteur est INPUT
  pinMode(inputCapteur,INPUT);

  // Servo des petales qui s'ouvre et qui se ferme
  servoPetales.attach(11);
  
  // Servo de la tete qui se penche vers la personne                
  servoTete.attach(10);

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}

void  loop()
{
  // Lit la valeur digital et la place dans la variable val
  int val = digitalRead(inputCapteur);
  Serial.print("val = ");Serial.println(val);

  // Quand le capteur detecte un mouvement => la varible se met HIGH
  if (val == HIGH)                       
  {
    Serial.println("Animation !");
    // Execute le programme :
    // - elle se dirige vers la personne
    Serial.println("Révérence");
    parcourtServo(servoTete, 1500, 900, 5);
    
    // - elle s'ouvre et se ferme 3x 
    for (int i = 0; i < 3; i++) {
      Serial.println("Ouverture pétales");
      parcourtServo(servoPetales, 1200, 1600, 5);
      Serial.println("Fermeture pétales");
      parcourtServo(servoPetales, 1600, 1200, 2);
    }
    
    // - puis elle se remet en position initiale
    Serial.println("Retour en position initiale");
    parcourtServo(servoTete, 900, 1500, 5);

    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(5000);
  }
}

