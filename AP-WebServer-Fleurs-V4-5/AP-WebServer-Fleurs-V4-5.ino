/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.
    Modifs le 28/03/2018 par François Douchet
  Se connecter en WiFi sur :"AP-WebServeur-Bouton"
  Pas de mot de passe
  Se connecter avec l'adresse IP : 1.1.1.1/go
   quatre boutonst
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

int SERVO_TETE_GAU = 5;
int SERVO_TETE_DRO = 175;
int SERVO_TETE_LASTPOS = 0;
int SERVO_PETALES_MIN = 10;
int SERVO_PETALES_MAX = 170;
int SERVO_PETALES_LASTPOS = 0;
#define CHORE_OUV_FER_NB  3
#define CHORE_ATTENTE_FIN  5000
int pos = 0;
int v = 0;
byte  Action=0;
IPAddress apIP(1, 1, 1, 1);  // Defining a static IP address: local & gateway - Default IP in AP mode is 192.168.4.1
const char *ssid = "AP-WebServeur-Fleur";
const char *password = "";    // pas de mot de passe
ESP8266WebServer server(80);
IPAddress myIP;
const int led = LED_BUILTIN;

// Noms des différents Servos
Servo servoTete;
Servo servoPetales;

    //----------------------------------------------------------------------------------------------

const char HTTP_HEAD[]     PROGMEM    = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[]    PROGMEM    = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:30%;} body{text-align: center;font-family:verdana;} button{border:1;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT[]   PROGMEM    = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM    = "</head><body onload='process_JS()'><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_ITEM[]     PROGMEM    = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_END[]      PROGMEM    = "</div></body></html>";

const char HTTP_Bouton[]   PROGMEM    = "<button type='submit' name='Action'  value='{v}' >{t}</button><BR><BR>";
const char HTTP_Valeur[]   PROGMEM    = "{ti}<BR><input type='text' name='{t}' value='{v}'>";
//const char HTTP_Micropas[] PROGMEM    = "_____<select name='Moteur_steps'><option value='0'{0}>Full Step</option><option value='1'{1}>Half Step</option><option value='2'{2}>1/4 step</option><option value='3'{3}>1/8 Step</option><option value='4'{4}>1/16 Step</option><option value='5'{5}>1/32 Step</option></select><BR>";    //<option value='4'{4}>1/16 Step</option><option value='5'{5}>1/32 Step</option>
//const char HTTP_Couple[]   PROGMEM    = "_____<select name='Moteur_couple'><option value='0'{0}>Arret:pas de couple</option><option value='1'{1}>Arret:couple</option></select><BR>";

void parcourtServo( Servo &servo, int depart, int fin, int vitesse) { 
  //if (vitesse =0) vitesse == 2; 
  if (depart > fin){
    for ( pos = depart ; pos <= fin ; pos--){
      servo.write(pos);
      delay(vitesse);
    }
  }
  else {     // depart<fin
    for ( pos = depart ; pos >= fin ; pos++) {   
      servo.write(pos);
      delay(vitesse);
    }
  }   
}
void PAGE_Construction() {
  
  String page = FPSTR(HTTP_HEAD);      
  page.replace("{v}", "AP WebServeur Fleur");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE); 
  page += FPSTR(HTTP_HEAD_END);        
  page += F("<h2>Pilotez la fleur</h2>");
  page += "<form action='http://"+myIP.toString()+"/go' method='POST'>";
  
  //------ boutons------
  page += FPSTR(HTTP_Bouton);    page.replace("{t}", "Fleur ouverte");       page.replace("{v}", "10");  
  page += FPSTR(HTTP_Bouton);    page.replace("{t}", "Fleur fermee");        page.replace("{v}", "11"); 
  page += FPSTR(HTTP_Bouton);    page.replace("{t}", "Tete a gauche");         page.replace("{v}", "12");  
  page += FPSTR(HTTP_Bouton);    page.replace("{t}", "Tete a droite");        page.replace("{v}", "13");
  page += FPSTR(HTTP_Bouton);    page.replace("{t}", "Dance");               page.replace("{v}", "15");
  page += "</form>";
  page += FPSTR(HTTP_END);
  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);
}
    //----------------------------------------------------------------------------------------------------------------
void PAGE_LireArguments() {

  digitalWrite(led, LOW); // on allume la Led BuildIn
  Serial.println();   Serial.print("Arguments:");   Serial.println(server.args());    
  if (server.argName(0) == "Action") {
    Action = server.arg(0).toInt();
  }
  Serial.println();
  Serial.print("Variable du bouton actionné : ");
  Serial.println(Action);
  PAGE_Construction();
  delay(500);
  digitalWrite(led, HIGH);   // on eteint la Led
}



void handleNotFound() {
  digitalWrite(led, 1);  // on eteint la Led intrene
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);  // on allume la Led interne
}

void setup(void) {
 
   servoPetales.attach(D1);
   servoTete.attach(D0);
 /* 
  servoTete.write(SERVO_TETE_LASTPOS);
  delay(50);
  servoPetales.write(SERVO_PETALES_LASTPOS);
  delay(50);
*/

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);  // on eteint la led interne
  Serial.begin(115200);  
  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP_STA);   //set-up the custom IP address
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  WiFi.softAP(ssid, password);
  myIP = WiFi.softAPIP();
  Serial.print("addresse IP: ");
  Serial.println(myIP);
  Serial.println("");

  server.on("/bouton", PAGE_Construction);
  server.on("/go", PAGE_LireArguments);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
    });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(1000);

    switch(Action){
      
      case 10: Serial.println("Ouverture pétales  ---->");
                parcourtServo(servoPetales, SERVO_PETALES_LASTPOS, SERVO_PETALES_MAX, 15);
                SERVO_PETALES_LASTPOS = SERVO_PETALES_MAX;
                Serial.print("Position finale : ");
                Serial.println(pos);
                Action = 0;
                break;
      case 11: Serial.println("fermeture pétales  ---->");
                parcourtServo(servoPetales, SERVO_PETALES_LASTPOS, SERVO_PETALES_MIN, 15);
                SERVO_PETALES_LASTPOS = SERVO_PETALES_MIN;
                Serial.print("Position finale : ");
                Serial.println(pos);
                Action = 0; 
                break;
      case 12:Serial.println("Tete en bas (a gauche ?)  ---->");
                parcourtServo(servoTete, SERVO_TETE_LASTPOS, SERVO_TETE_GAU, 10); 
                SERVO_TETE_LASTPOS = SERVO_TETE_GAU;
                Serial.print("Position finale : ");
                Serial.println(pos);
                Action = 0;
                break;
      case 13: Serial.println("Tete en haut ( a droite ?) ---->");
                parcourtServo(servoTete, SERVO_TETE_LASTPOS, SERVO_TETE_DRO, 5); 
                SERVO_TETE_LASTPOS = SERVO_TETE_DRO;
                Serial.print("Position finale : ");
                Serial.println(pos);
                Action = 0;
                break;
      case 15:Serial.println("Dance  ---->");
                parcourtServo(servoPetales, SERVO_PETALES_LASTPOS, 90, 10);
                SERVO_PETALES_LASTPOS = 90;
                parcourtServo(servoTete, SERVO_TETE_LASTPOS, 90, 10); 
                SERVO_TETE_LASTPOS = 90;
                Action = 0; 
                break;
    }
}

