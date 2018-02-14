// fleur buke
// 1 servo - des led strip rgb - et capteur de mouvement - un transistors S8050


//Scenario:
//elle ouvre ses petales quand elle detecte un mouvement
//allumme les leds avec la fonction rainbow
//puis de se remet en position normal


#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#define BRIGHTNESS 50
#define LEDPIN 5 // pin DIN connecté a l'arduino
#define NUMBER_PIEXELS 9// nombre de led allumé 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIEXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int led = 2;          //led temoin
int inputPin = 8;     // entré du capteur
int command = 7;     // base du transistor pour commnader l'allumage et l'extinction du ruban de leds
                    //du ruban de led

int  val;         //variable pour l'etat du capteur

Servo myservo1;


void  setup()
{
  strip.begin();                  // connection avec le ledsrtip
  strip.setBrightness(BRIGHTNESS);
  pinMode(inputPin, INPUT);      // le pin du capteur est INPUT
  pinMode(7 , OUTPUT); // commande pour faire passer le transistor de passant a saturé
  pinMode(led,OUTPUT);                    // le pin de la led est OUTPUT 
  digitalWrite(2,HIGH);                // allume la led temoin 
  
  myservo1.attach(11);         //servo des petales qui s'ouvre et qui se ferme
  Serial.begin(9600);
}
void  loop()
{
  { val = digitalRead(inputPin);     // lit la valeur digital et la place dans la variable val
    Serial.println(val);

    if (val == HIGH)                    //quand le capteur detecte un mouvement la varible se met HIGH
    {
      Functservo(myservo1, 1100, 1600, 5);         //les pétales s'ouvrent
      delay(10);
      digitalWrite(command, LOW);          // leds strip allumé
      delay(10);
      rainbowCycle(5);                       // ohh un joli arc en ciel !! le 5 correspond au temp 
      delay(10);
      digitalWrite(command, HIGH);           // leds strip eteint
      Functservo(myservo1, 1600, 1100, 5);   //les petales se ferment

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
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < NUMBER_PIEXELS; i++) {
      c = Wheel(((i * 256 / NUMBER_PIEXELS) + j) & 255);
      strip.setPixelColor(i, *c, *(c + 1), *(c + 2));
    }
    strip.show();
    delay(SpeedDelay);
  }
}
byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}
