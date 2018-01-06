// Issu du document http://eskimon.fr/public/ebook/Arduino_-_Premiers_pas_en_informatique_embarquee.pdf

#include <Servo.h>
int temps=1500; //censéêtre àmi­cheminentre 1000 et 2000,un bonpointdedépart
Servo monServo;

void setup()
{
  Serial.begin(115200);
  Serial.println("Calibrage Servo");
  Serial.println("a= +10; q= -10; z= +100; s= -100");
  monServo.attach(11);
  //on démarreà unevaleurcenséêtrelamoitié de
  //l'excursion totaledel'angleréalisépar le servomoteur
  monServo.writeMicroseconds(temps);
}
void loop()
{
  //desdonnées surlaliaisonsérie ?(lorsque l'onappuiesur 'a'ou 'd')
  if(Serial.available())
  {
    char commande=Serial.read(); //onlit
    //on modifie la consigne si c'est un caractère qui nous intéresse
    switch(commande){
      case 'a': temps +=10;  // ajout   de  10 µs au temps
                break;
      case 'z': temps +=100; // ajout   de 100 µs au temps
                break;
      case 'q': temps -=10;  // retrait de  10 µs au temps
                break;
      case 's': temps -=100; // retrait de 100 µs au temps
                break;
                
    }
    // on modifielaconsigneduservo
    monServo.writeMicroseconds(temps);

    // et on faitunretour surlaconsolepour savoir oùonestrendu
    Serial.print("Consigne courante: ");Serial.println(temps);
  }
}
