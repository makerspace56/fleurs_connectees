//
//  ,---.    ,---..-./`) ,---.   .--..-./`)          ______     .-./`) ,---.  ,---.   ____
//  |    \  /    |\ .-.')|    \  |  |\ .-.')        |    _ `''. \ .-.')|   /  |   | .'  __ `.
//  |  ,  \/  ,  |/ `-' \|  ,  \ |  |/ `-' \        | _ | ) _  \/ `-' \|  |   |  .'/   '  \  \ 
//  |  |\_   /|  | `-'`"`|  |\_ \|  | `-'`"`        |( ''_'  ) | `-'`"`|  | _ |  | |___|  /  |
//  |  _( )_/ |  | .---. |  _( )_\  | .---.         | . (_) `. | .---. |  _( )_  |    _.-`   |
//  | (_ o _) |  | |   | | (_ o _)  | |   |         |(_    ._) ' |   | \ (_ o._) / .'   _    |
//  |  (_,_)  |  | |   | |  (_,_)\  | |   |         |  (_.\.' /  |   |  \ (_,_) /  |  _( )_  |
//  |  |      |  | |   | |  |    |  | |   |         |       .'   |   |   \     /   \ (_ o _) /
//  '--'      '--' '---' '--'    '--' '---'         '-----'`     '---'    `---`     '.(_,_).'
//

// Fleur Mini Diva (dans la continuité des kits mini)
// 1 servos - un module enregistreur/haut parleur ISD 1820-  1a capteurs de mouvement

// Scenario :
// - elle detecte un mouvement
// - la fleur s'ouvre et "chante"
// - puis de se remet en position initiale

#include <Servo.h>

// Led temoin
int ledTemoin = 2;

// Capteur
int inputPin = 8;

// Commande pour faire lecture sur le module de son
int commandeSon = 7;

// Servo
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
  pinMode(inputPin, INPUT);
  
  // Le pin du module qui declanche la musique
  pinMode(commandeSon , OUTPUT);
  // Son désactivé
  digitalWrite(commandeSon, LOW);
  
  // Servo des pétales qui s'ouvre et qui se ferme
  servoPetales.attach(11);        

  // Initialise la liaison série pour debug
  Serial.begin(9600);
}

void  loop()
{
  // lit la valeur digital et la place dans la variable val
  int val = digitalRead(inputPin);     
  Serial.print("val = ");Serial.println(val);

  // Quand le capteur detecte un mouvement la varaible se met HIGH
  if (val == HIGH)                    
  {
    // Declenche le son
    Serial.println("Declenche le son");
    digitalWrite(commandeSon, HIGH);
    delay(10);

    // Ouverture des pétales
    parcourtServo(servoPetales, 1100, 1600, 5);
    
    // Attend 7 secondes, la durée du son
    delay(7000);

    // Arrête le son
    digitalWrite(commandeSon, LOW);

    // Fermeture des pétales
    parcourtServo(servoPetales, 1600, 1100, 5);

    // - Attente pour s'économiser un peu qd qq active le capteur
    delay(5000);
  }
  else
  {
    // Sinon rien enfin position normal led temoin etainte
    digitalWrite(commandeSon, LOW);
  }
}

