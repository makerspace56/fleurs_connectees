// fleur mini 
// 2 servos - 1 Module de capteur infrarouge d'évitement d'obstacles (https://e.banana-pi.fr/distance/121-module-de-capteur-infrarouge-d-evitement-d-obstacles.html)


//Scenario :
//elle s'oriente vers la personne 
//et s'ouvre et se ferme 3x 
//puis de se remet en position "normal"


#include <Servo.h>

int led = 2;          //led temoin
int inputPin = 8;    //port entréé
int  val;         //variable pour l'etat du cpateur

Servo myservo2;  // servos
Servo myservo1;


void  setup()
{
pinMode(led,OUTPUT);                    // le pin de la led est OUTPUT 
digitalWrite(2,HIGH);                // allume la led temoin 
pinMode(inputPin,INPUT);              // le pin du capteur est INPUt
 myservo1.attach(11);                //servo des petales qui s'ouvre et qui se ferme
 myservo2.attach(10);               // servo de la tete qui se penche vers la personne
Serial.begin(9600);
}
void  loop()
{
{ val=digitalRead(inputPin);       // lit la valeur digital et la place dans la variable val 
Serial.print(val);

if(val==HIGH)                       //quand le capteur detecte un mouvement la varible se met HIGH
{
                                    // execute le programe
Functservo(myservo2,1500,900,5);   //elle se dirirge vers la personne 
 for (int i = 0; i < 3; i++) {      //et s'ouvre et se ferme 3x 
Functservo(myservo1,1200,1600,5);
Functservo(myservo1,1600,1200,2);
 }
Functservo(myservo2,900,1500,5);    //puis de se remet en position "normal"
}

}
}



void Functservo( Servo &monservo, int depart,int fin, int vitesse) {
  //Functservo: passe en premier le servo concerné, le debut et la fin.
  // Passe le nom du servo vers l'object servo a travers &monservo
 
  
  //if (vitesse <=1 ) {vitesse == 1; }
  if (depart > fin){
      
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
