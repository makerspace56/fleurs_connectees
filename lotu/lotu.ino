#include <Servo.h>

#define PERIODE_OUVERTURE_PETALES 1
#define PERIODE_ROTATION_TIGE 20

// Petit modif
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

int mss[9][2] = {{1700, 1200},
{1600, 1300},
{1550, 1180},
{1500, 1000},
{1620, 1030},
{1750, 1070},
{1860, 1210},
{1800, 1350},
{1700, 1320}
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
  // 1100 = ouvert à fond
  // 1600 = fermé completement
  s3.writeMicroseconds(1350);
}

void Functservo(Servo &monservo, int depart, int fin, int vitesse) {
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

void tourneTigeCompletEnUneFois(){
    for(int i = 0; i < 9; i++){
      tourneTige();
    }
}

void tourneTige(){
  int angle1 = angles[cpt][0];
  int angle2 = angles[cpt][1];

  int angle1next = angles[cpt2][0];
  int angle2next = angles[cpt2][1];

  int ms1 = map(angle1, 285, 331, 1520, 1730); //mss[cpt][0];
  int ms2 = map(angle2, 228, 288, 1210, 1500); // mss[cpt][1];

  Serial.print(cpt);Serial.print(" - (ms1, ms2) = ("); Serial.print(ms1);Serial.print(", ");Serial.print(ms2);Serial.println(")");
  int ms1next = map(angle1next, 285, 331, 1520, 1730); // mss[cpt2][0];
  int ms2next = map(angle2next, 228, 288, 1210, 1500); // mss[cpt2][1];

  Functservo( s1, ms1, ms1next, 10);
  Functservo( s2, ms2, ms2next, 10);
  
  cpt ++;
  if (cpt == 9){
    cpt = 0;
  }

  cpt2 ++;
  if (cpt2 == 9){
    cpt2 = 0;
  }
}

void tourneTigeParZero(){

  int ms1 = mss[cpt][0];//map(angle1, 285, 331, 1520, 1730);
  int ms2 = mss[cpt][1];//map(angle2, 228, 288, 1210, 1500);

  Serial.print(cpt);Serial.print(" - (ms1, ms2) = ("); Serial.print(ms1);Serial.print(", ");Serial.print(ms2);Serial.println(")");
  int ms1next = mss[cpt2][0];//map(angle1next, 285, 331, 1520, 1730);
  int ms2next = mss[cpt2][1];//map(angle2next, 228, 288, 1210, 1500);

  Functservo( s1, ms1, mss[0][0], 5);
  Functservo( s2, ms2, mss[0][1], 5);
  Functservo( s1, mss[0][0], ms1next, 5);
  Functservo( s2, mss[0][1], ms2next, 5);
  
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
  
  // Configuration retenue
  // Photoresistance avec pont 1k Ohm: 
  // Interne avec neon = 115
  // Interne sans neon = 50
  // Dehors soleil ciel blanc hier 16h = 640

  // Configuration non retenue
  // Photoresistance avec pont 220 Ohm: 
  // Interne avec neon = 500 +-20
  // Interne sans neon = 240
  // Dehors soleil ciel blanc hier 16h = 840

  sensorValue = analogRead(A0);
  Serial.print("valueA0= ");Serial.println(sensorValue);
  sensorValue = map(sensorValue, 50, 115, 1600, 1100);
  sensorValue = max(1100, sensorValue);
  sensorValue = min(1600, sensorValue);
  
  s3.writeMicroseconds(sensorValue);
}

void loop()
{
  // Toutes les PERIODE_OUVERTURE_PETALES => ouvre la fleur en fonction de la lumière
  if (tempsEnSecCompteur % PERIODE_OUVERTURE_PETALES == 0){
    ouvrePetales();
  }
  
  // Tous les PERIODE_ROTATION_TIGE secondes => tourne la tige et RAZ du compteur de temps
  if (tempsEnSecCompteur % PERIODE_ROTATION_TIGE == 0){
    tourneTigeCompletEnUneFois();
    tempsEnSecCompteur = 0;  
  }

  // Incremente le compteur de temps et attend 1 seconde
  tempsEnSecCompteur ++;
  delay(1000); 
}
