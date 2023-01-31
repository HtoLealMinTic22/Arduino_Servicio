#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>




const char* ssid = "Sistema Adin 2G";
const char* password = "Leal083096";


volatile boolean swFigPrender  = false ;
volatile boolean swLuzEPrender = false ;
volatile boolean swCalcuMin1   = false ;
volatile boolean swCalcuMin2   = false ;
volatile boolean swCalcuMin3   = false ;
volatile boolean swCalcuMin4   = false ;
volatile boolean swMovimiento  = false ;
volatile boolean swMovimiento2 = false ;
volatile boolean swMovimiento3 = false ;
volatile boolean swMovimiento4 = false ;
volatile boolean swIniRel1     = true  ;

volatile boolean swSonido1 = true ;
volatile boolean swSonido2 = true ;
volatile boolean swSonido3 = true ;
volatile boolean swSonido4 = true ;

volatile boolean SwAlarma1 = true ;
volatile boolean SwAlarma2 = true ;
volatile boolean SwAlarma3 = true ;

volatile boolean SwPir1 = true  ;
volatile boolean SwPir2 = true  ;
volatile boolean SwPir3 = true  ;

volatile int Boton1 ;
volatile int Boton2 ;
volatile int Boton3 ;
volatile int Boton4 ;

volatile int LuzEnt  ;
volatile int Sw_Movto  ;
volatile int Sw_Movto2 ;
volatile int Sw_Movto3 ;
volatile int Vr_ADC_senLuz = 0 ;

const int pulsoMin  = 650 ;   // pulso em ms para girar a 0°
const int pulsoMax  = 2550 ;  // pulso em ms para girar a 180°
int angulo  ;
int angulo2 ;

int contconexion = 0 ;
unsigned long previoMillis = 0 ;
const int PinLuzSala    = 16 ;            // D0 - GPIO16 - Luz sala
//                                      // D1 - Clock  - Azul       
//                                      // D2 - Data   - Verde 
const int PinLuces    =  0 ;            // D3 - GPIO0  Luces pecebre  
const int PinMovto    =  2 ;            // D4 - GPIO2  P.I.R. Uno   
const int PinCascada  = 14 ;            // D5 - GPIO14 Cascada    
const int PinMovto3   = 13 ;            // D7 - GPIO13 P.I.R. Tres
const int PinMovto2   = 15 ;            // D8 - GPIO15 P.I.R. Dos 
const int PinFiguras  = 12 ;            // D6 - GPIO12 Figuras
const int PinLuzEnt   =  3 ;            // RX - GPIO3  Luz entrada
const int PinAlarma   =  1 ;            // TX - GPIO1  PinAlarma  

RTC_DS3231 rtc;
ESP8266WebServer server(80);

char strUrl ;
String ssid_leido ;
String pass_leido ;
int ssid_tamano=0;
int pass_tamano=0;

volatile int anorel=0 ;
volatile int mesrel=0 ;
volatile int diarel=0 ;

volatile int hrel1 =0 ;
volatile int hrel2 =0 ;
volatile int hrel3 =0 ;
volatile int hrel4 =0 ;
volatile int hrelant1 =0 ;
volatile int hrelant2 =0 ;
volatile int hrelant3 =0 ;
volatile int hrelant4 =0 ;

volatile int mrel1 =0 ;
volatile int mrel2 =0 ;
volatile int mrel3 =0 ;
volatile int mrel4 =0 ;
volatile int mrelant1 =0 ;
volatile int mrelant2 =0 ;
volatile int mrelant3 =0 ;
volatile int mrelant4 =0 ;

volatile int vr_TgLLeno =0 ;
volatile int ti =0 ;

volatile int tgMinutos1 =0 ;
volatile int tgMinutos2 =0 ;
volatile int tgMinutos3 =0 ;
volatile int tgMinutos4 =0 ;

volatile int tanorel=0 ;
volatile int tmesrel=0 ;
volatile int tdiarel=0 ;

volatile int thorarel1=0 ;
volatile int thorarel2=0 ;
volatile int thorarel3=0 ;
volatile int thorarel4=0 ;

volatile int tMinrel1=0 ;
volatile int tMinrel2=0 ;
volatile int tMinrel3=0 ;
volatile int tMinrel4=0 ;

volatile int AcuAno = 0 ;
volatile int AcuMes = 0 ;
volatile int Acudia = 0 ;
volatile int AcuHora= 0 ;

volatile int AcuMin1 = 0 ;
volatile int AcuMin2 = 0 ;
volatile int AcuMin3 = 0 ;
volatile int AcuMin4 = 0 ;

volatile int AcuHora1= 0 ;
volatile int AcuHora2= 0 ;
volatile int AcuHora3= 0 ;
volatile int AcuHora4= 0 ;

volatile long anomov =0 ;
volatile int mesmov =0 ;
volatile int diamov =0 ;
volatile int horamov=0 ;
volatile int minumov=0 ; 

volatile int horamov1=0 ;
volatile int minumov1=0 ; 
volatile int horamov2=0 ;
volatile int minumov2=0 ; 
volatile int horamov3=0 ;
volatile int minumov3=0 ; 
volatile int horamov4=0 ;
volatile int minumov4=0 ; 

volatile int dr1  =0 ;
volatile int dr2  =0 ;
volatile int dr3  =0 ;
volatile int dr4  =0 ;

volatile int h1  =0 ;
volatile int h2  =0 ;
volatile int h3  =0 ;
volatile int h4  =0 ;

volatile int m1  =0 ;
volatile int m2  =0 ;
volatile int m3  =0 ;
volatile int m4  =0 ;

volatile int mod1=0 ;
volatile float pes1=0 ;
volatile int hora1 ;
volatile int dura1 ;

volatile int val = 0 ;  
int estado = LOW ;

const char* ssid1 = "Sistema Adin 2G";
const char* pass1 = "Leal083096";

String  EstBoton1 ;
String  EstBoton2 ;
String  EstBoton3 ;
String  EstBoton4 ;
String mensaje=" " ;

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Temporizador General</title> "
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
  "<style type ='text/css'> body,td,th {color:#036;} label1{color: rgb(240,6,6);} label3{color: rgb(233, 82, 157);} label2{color: rgb(250,246,9,0.952);} body{background-color:#a0cbf3;} "
  "</style>"
"</head>"

"<body>"
"<center>"
"<br><br><br>"
"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 430px; height:250px; display: inline-block; margin: 30px; ' > "
   "<form action ='config' method='get' target='pantalla'>"
     "<center>"
       "<fieldset align='center' style='border-style:solid; border-color:#336666; width:400px; height:170px; '"
        "<h2>"
        "<label1><b>- CONFIGURAR RED-WIFI -</label1>"  
        "<br><br>"
        "</h2>"
   
        "<label for='usuario'> Usuario Wi-fi  .......: </label>"
        "<input class='input1' type='text' name='ssid'     id='ssid' placeholder='Wi-fi'    onfocus='FocoColor1(this)' style='width:50%;height:40px' >"
        "<br><br>"
        "<label for='pas'> Clave de acceso ...: </label> "
        "<input class='input1' type='password' name='pass' id='pass' placeholder='password' onfocus='FocoColor1(this)' style='width:50%;height:30px' >"
        "<br><br><br><br>" 
        "<input class='boton'  type='submit' value='ACTUALIZAR DATOS' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
        "</fieldset>" 
     "</center>" 
     "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
   "</form>" 
"</div>"
"</center>"

"<br>"
"<center>"
"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 430px; height:270px; display: inline-block; margin: 30px; ' > "
"<form action ='configDate' method='get' target='pantalla'>"
  "<center>"
    "<fieldset align='center' style='border-style:solid; border-color:#336666; width:400px; height:180px; '"
    "<h2>"
    "<label1>- CONFIGURAR Fecha/Hora -</label1>"  
    "<br><br>"
    "</h2>"

    "<label for='Fecha dd/mm/año'> Fecha.......: </label>"
    "<input class='input1' type='text' name='diaDate'  id='diaDate' placeholder='Dia'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' >"
    "<input class='input1' type='text' name='mesDate'  id='mesDate' placeholder='Mes'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' >"
    "<input class='input1' type='text' name='anoDate'  id='anoDate' placeholder='Año'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' >"
    "<br><br>"

    "<label for='Hora'> Hora.......: </label>"
    "<input class='input1' type='text' name='horaDate' id='horaDate' placeholder='Hora'    onfocus='FocoColor(this)' style='width:20%;height:40px ;border-radius:10px'>"
    "<input class='input1' type='text' name='minDate'  id='minDate'  placeholder='Minutos' onfocus='FocoColor(this)' style='width:20%;height:40px ;border-radius:10px'>"
    
    "<br><br><br><br>" 
    "<input class='boton'  type='submit' value='GUARDAR FECHA' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
    "<br><br>" 
   
    "</fieldset>" 
  "</center>" 
  "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
"</form>" 
"</div>"
"<center>"
;

void handleRoot() {

  server.send(200, "text/html", mensaje + pagina  );

}

void handleNotFound() {
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
}


void setup(void) {
    Serial.begin(115200);
    // Serial.println("\n");

    EEPROM.begin(4096);

    /// Reloj  
    if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
    }
 
    // Si se ha perdido la corriente, fijar fecha y hora
    if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2019, 1, 21, 3, 0, 0));
    }
   
    /// Reloj  
    /// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Wire.begin();     // D1-Clock / D2-Data  
    DateTime now = rtc.now();
    anorel = now.year()  ;
    mesrel = now.month() ;
    diarel = now.day()   ;
    hrel1  = now.hour()  ;
    mrel1  = now.minute();

    mod1 = EEPROM.read(0) ; // Modo 0-Automatico/1-Manual
    h1   = EEPROM.read(100) ; // Horario1 - Hora  
    hora1 = h1 ;
  
    m1   = EEPROM.read(101) ; // Horario1 - Minutos
    h2   = EEPROM.read(102) ; // Horario2 - Hora  
    m2   = EEPROM.read(103) ; // Horario2 - Minutos
    h3   = EEPROM.read(104) ; // Horario3 - Hora  
    m3   = EEPROM.read(105) ; // Horario3 - Minutos
    pes1 = EEPROM.read(106) ; // Peso racion   

    dr1  = EEPROM.read(108) ; // Duracion (1)
    dr2  = EEPROM.read(109) ; // Duracion (2)
    dr3  = EEPROM.read(110) ; // Duracion (3)

    h4   = EEPROM.read(120)   ; // Horario4 - Hora  
    m4   = EEPROM.read(121)   ; // Horario4 - Minutos
    dr4  = EEPROM.read(122)   ; // Duracion (4)    
    
    pinMode(PinLuzSala, OUTPUT)  ;    // D0 - GPIO16 - Luz sala
    pinMode(PinLuces,   OUTPUT)  ;    // D3 Luces pecebre  
    pinMode(PinFiguras, OUTPUT)  ;    // D6 - GPIO12
   
    pinMode(PinMovto,  INPUT )   ;    // D4 - GPIO2  P.I.R. Uno 
    pinMode(PinMovto2, INPUT )   ;    // D8 - GPIO15 P.I.R. Dos 
    pinMode(PinMovto3, INPUT )   ;    // D7 - GPIO13 P.I.R. Tres

    pinMode(PinCascada, OUTPUT)  ;    // D5 - GPIO14 Cascada  
    pinMode(PinLuzEnt,  OUTPUT)  ;    // RX - GPIO3  Luz entrada
    pinMode(PinAlarma,  OUTPUT)  ;    // TX - GPIO1 PinAlarma

    // no mover
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

//    IPAddress ip(192,168,0,39);
//    IPAddress gateway(192,168,0,1);
//    IPAddress subnet(255,255,255,0);
//    WiFi.config(ip,gateway,subnet) ;

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


void loop(void) {
  server.handleClient();
}
