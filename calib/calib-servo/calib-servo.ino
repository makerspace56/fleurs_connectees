// Issu du document http://eskimon.fr/public/ebook/Arduino_-_Premiers_pas_en_informatique_embarquee.pdf
//valeur max 1360 
#include <Servo.h>
const int SERVO_COUNT = 3;

Servo servos[SERVO_COUNT];
int servoTemps[SERVO_COUNT] = {1500, 1500, 1500};
int currentServoIndex = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Calibrage Servo");
  Serial.println("p= nextServo; a= +10; q= -10; z= +100; s= -100");
  servos[0].attach(11);
  servos[1].attach(10);
  servos[2].attach(9);
  
  // On démarre à une valeur censée être la moitié de
  // l'excursion totaledel'angleréalisépar le servomoteur
  for(int i = 0; i < SERVO_COUNT; i++){
    servos[i].writeMicroseconds(servoTemps[i]);
  }
}
void loop()
{
  //desdonnées surlaliaisonsérie ?(lorsque l'onappuiesur 'a'ou 'd')
  if(Serial.available())
  {
    char commande=Serial.read(); //onlit
    //on modifie la consigne si c'est un caractère qui nous intéresse
    switch(commande){
      case 'a': servoTemps[currentServoIndex] +=10;  // ajout   de  10 µs au temps
                break;
      case 'z': servoTemps[currentServoIndex] +=100; // ajout   de 100 µs au temps
                break;
      case 'q': servoTemps[currentServoIndex] -=10;  // retrait de  10 µs au temps
                break;
      case 's': servoTemps[currentServoIndex] -=100; // retrait de 100 µs au temps
                break;
      case 'p': currentServoIndex = (currentServoIndex + 1) % SERVO_COUNT;
                Serial.print("Servo courant : ");Serial.println(currentServoIndex);
                break;
    }
    // on modifielaconsigneduservo
    servos[currentServoIndex].writeMicroseconds(servoTemps[currentServoIndex]);

    // et on faitunretour surlaconsolepour savoir oùonestrendu
    Serial.print("servo[");Serial.print(currentServoIndex);Serial.print("] = ");Serial.println(servoTemps[currentServoIndex]);
  }
}
