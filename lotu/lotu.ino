#include <Servo.h>

#define PERIODE_OUVERTURE_PETALES 1
#define PERIODE_ROTATION_TIGE 60

// tableau entiers positions 2 moteurs 1 par colonne
int angles[9][2] = {{321, 231},
{332, 247},
{318, 250},
{310, 242},
{299, 227},
{296, 208},
{310, 204},
{327, 220},
{336, 238}
};

int tempsEnSecCompteur = 0;

int cpt = 0;
int cpt2 = 1;


const int valPr = A0;
int sensorValue = 0;
int sensorMin = 1023;     
int sensorMax = 0;

int temps = 1500; //censé être à mi-chemin entre 1000 et 2000, un bon point de départ
 
Servo s1, s2, s3;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("LOTU is starting...");
  
  s1.attach(10,1500,2000);
  s2.attach(9,1500,2000);
  s3.attach(11);
  //on démarre à une valeur censé être la moitié de
  //l'excursion totale de l'angle réalisé par le servomoteur
  //monServo.writeMicroseconds(temps);
  // 1600 = ouvert à fond
  // 2300 = fermé completement
  s3.writeMicroseconds(1500);
  delay(2000);
  s3.writeMicroseconds(2200);
}

void Functservo( Servo &monservo, int depart, int fin, int vitesse) {
  //Functservo: passe en premier le servo concerné, le debut et la fin.
  // Passe le nom du servo vers l'object servo a travers &monservo
 
  
  if (vitesse <=1 ) {vitesse == 1; }
  if (depart > fin){
      
      for (int pos = depart ; pos >= fin ; pos--) 
        { 
         monservo.writeMicroseconds(pos);
         delay(vitesse);
       ///  Serial.println(pos);
        }
  }
   else
  {
        for (int pos = depart ; pos <= fin ; pos++) 
        { 
         monservo.writeMicroseconds(pos);
         delay(vitesse);
       //  Serial.println(pos);
        }
  }
}

void tourneTige(){
  int angle1 = angles[cpt][0];
  int angle2 = angles[cpt][1];

  int angle1next = angles[cpt2][0];
  int angle2next = angles[cpt2][1];

  int ms1 = map(angle1, 285, 331, 1520, 1730);
  int ms2 = map(angle2, 228, 288, 1210, 1500);

  int ms1next = map(angle1next, 285, 331, 1520, 1730);
  int ms2next = map(angle2next, 228, 288, 1210, 1500);

  Functservo( s1, ms1, ms1next,50);
  Functservo( s2, ms2, ms2next,50);
  
  cpt ++;
  if (cpt == 9){
    cpt = 0;
  }

  cpt2 ++;
  if (cpt2 == 9){
    cpt2 = 0;
  }
}

void ouvrePetales(){
  sensorValue = analogRead(A0);
   Serial.print("valueA0= ");Serial.println(sensorValue);
  sensorValue = map(sensorValue, sensorMin, sensorMax, 2000, 1000);
 

  s3.writeMicroseconds(sensorValue);
  

}

void loop()
{
  // Toutes les PERIODE_OUVERTURE_PETALES => ouvre la fleur en fonction de la lumière
  //if (tempsEnSecCompteur % PERIODE_OUVERTURE_PETALES == 0){
  //  ouvrePetales();
  //}
  
  // Tous les PERIODE_ROTATION_TIGE secondes => tourne la tige et RAZ du compteur de temps
  if (tempsEnSecCompteur % PERIODE_ROTATION_TIGE == 0){
    tourneTige();
    tempsEnSecCompteur = 0;  
  }

  // Incremente le compteur de temps et attend 1 seconde
  tempsEnSecCompteur ++;
  delay(1000); 
}
