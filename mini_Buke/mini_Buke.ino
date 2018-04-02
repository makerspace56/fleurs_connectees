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
#define NUMBER_PIEXELS 3// nombre de led allumé 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIEXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

#define SERVO_TETE_PIN    10
#define SERVO_TETE_MIN    1200
#define SERVO_TETE_MAX    2300
#define SERVO_PETALES_PIN 11
#define SERVO_PETALES_MIN 1600
#define SERVO_PETALES_MAX 2300
#define CHORE_OUV_FER_NB  3

int led = 2;          //led temoin
int led2 = 3;          // 2eme led
int inputPin = 8;     // entré du capteur
int command = 7;     // relay pour commnader l'allumage et l'extinction du ruban de leds
                    //du ruban de led

int  val = LOW;         //variable pour l'etat du capteur

Servo servoPetales;


void  setup()
{
  strip.begin();                  // connection avec le ledsrtip
  strip.setBrightness(BRIGHTNESS);
  pinMode(inputPin, INPUT);      // le pin du capteur est INPUT
  pinMode(7 , OUTPUT); // commande pour le relay
  pinMode(led,OUTPUT);                    // le pin de la led est OUTPUT 
  digitalWrite(led,HIGH);                // allume la led temoin 
  
  servoPetales.attach(SERVO_PETALES_PIN);         //servo des petales qui s'ouvre et qui se ferme
  Serial.begin(9600);
}
void  loop()
{
  { val = digitalRead(inputPin);     // lit la valeur digital et la place dans la variable val
    Serial.println(val);

    if (val == HIGH)                    //quand le capteur detecte un mouvement la varible se met HIGH
    {
      parcourtServo(servoPetales, SERVO_PETALES_MAX, SERVO_PETALES_MIN, 5);  //les pétales s'ouvrent
      delay(10);
      digitalWrite(command, HIGH);          // leds strip allumé
      Serial.println("command HIGH");
      delay(10);
      rainbowCycle(5);                       // ohh un joli arc en ciel !! le 5 correspond au temp 
      delay(10);
      digitalWrite(command, LOW);           // leds strip eteint
      Serial.println("command, LOW");
      parcourtServo(servoPetales, SERVO_PETALES_MIN, SERVO_PETALES_MAX, 5);   //les petales se ferment

    }
 
  }
}



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
