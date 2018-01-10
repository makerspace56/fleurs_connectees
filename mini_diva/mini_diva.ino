// fleur Diva(dans la continuité des kits mini)
// 1 servos - un module enregistreur/haut parleur ISD 1820-  1a capteurs de mouvement 


//Scenario:
//elle detecte un mouvement 
//la fleur s'ouvre et "chante" 
//puis de se remet en position "normal"


#include <Servo.h>


int inputPin = 8;  //port entré du capteur
int command = 7;   //commande pour faire lecture sur le module de son
int ledTemoin = 2;
int  val;         //variable pour l'etat du capteur

Servo myservo1;


void  setup()
{
 
  pinMode(inputPin, INPUT);     // le pin du capteur est INPUT
  pinMode(7 , OUTPUT);          // pin branché sur le module qui declanche la musique
  digitalWrite(7,LOW);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  myservo1.attach(11);        //servo des petales qui s'ouvre et qui se ferme
  Serial.begin(9600);
}
void  loop()
{
  { val = digitalRead(inputPin);     // lit la valeur digital et la place dans la variable val
    Serial.println(val);

    if (val == HIGH)                    //quand le capteur detecte un mouvement la varible se met HIGH
    {
     
      digitalWrite(command, HIGH);       // lecture
      delay(10);
      Functservo(myservo1, 1100, 1600, 5);
      delay(7000);                            // le temps d'un son
      digitalWrite(command, LOW);
      Functservo(myservo1, 1600, 1100, 5);





    }
    else
    {
      digitalWrite(command, LOW);              //sinon rien enfin position normal led temoin etainte

    }
  }
}



void Functservo( Servo &monservo, int depart, int fin, int vitesse) {
  //Functservo: passe en premier le servo concerné, le debut et la fin.
  // Passe le nom du servo vers l'object servo a travers &monservo


  //if (vitesse <=1 ) {vitesse == 1; }
  if (depart > fin) {

    for (int pos = depart ; pos >= fin ; pos--)
    {
      monservo.write(pos);
      delay(vitesse);
      //Serial.println(pos);
    }
  }
  else
  {
    for (int pos = depart ; pos <= fin ; pos++)
    {
      monservo.write(pos);
      delay(vitesse);
      //Serial.println(pos);
    }
  }

}

