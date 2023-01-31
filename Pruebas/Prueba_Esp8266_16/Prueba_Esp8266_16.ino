#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "RTClib.h"
#include <Wire.h>
#include <Servo.h>

const char* ssid = "Sistema Adin 2G";
const char* password = "Leal083096";
String mensaje=" - Mensaje de inicio - " ;

Servo myservo1;  // Pastor 
Servo myservo2;  // Lavandera

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
const int PinLuzSala    = 16 ;          // D0 - GPIO16 - Luz sala
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

//int PinPastor3  =  15 ;  

RTC_DS3231 rtc;

ESP8266WebServer server(80);

//const int led = 13;
String strUrl ;
//  char ssid[20] ; 
// char pass[20] ; 
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

// Definir variables alarma --
volatile int Almh1  =0 ;
volatile int Almh2  =0 ;
volatile int Almh3  =0 ;

volatile int Almm1  =0 ;
volatile int Almm2  =0 ;
volatile int Almm3  =0 ;

volatile int Almt1  =0 ;
volatile int Almt2  =0 ;
volatile int Almt3  =0 ;
// ---------------------------

volatile int mod1=0 ;
volatile float pes1=0 ;
volatile int hora1 ;
volatile int dura1 ;

volatile int val = 0 ;  
int estado = LOW ;

String  EstBoton1 ;
String  EstBoton2 ;
String  EstBoton3 ;
String  EstBoton4 ;
//String mensaje=" - Mensaje de inicio - " ;

// No sirve "<meta http-equiv='Refresh' content='1'>"


String PagWeb =
"<!DOCTYPE html>"
"<html lang='es'>"

"<head>"
 "<meta charset='UTF-8'>"
 "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
 "<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
 "<title>Tempo-Gral.</title> "

 "<style type ='text/css'> body,td,th { color:#036;} body{background-color: rgb(161, 235, 163);} "
 "</style>"

  "<script type='text/javascript'>   "
    "window.onload = FocoColor() ;      "
    "function FocoColor(x){"
    "x.style='width:75px; height:35px; background-color:rgba(247, 245, 137, 0.952); color:red; text-shadow:1px 1px 1px blue; text-align:center; border-radius:5px' ;"
    "}"

    "function FocoColor1(x){"
    "x.style='width:180px; height:35px; background-color:rgba(250,246,9,0.952); color:red; text-shadow:1px 1px 1px blue; text-align:center; border-radius:5px' ;"
    "}"    
  "</script>"
 
"</head>"
"<body>"
"</form> "  
"<center>"
"<br>" 

// Definicion de horarios
"<fieldset align='center' style='border-style:solid; border-color:#336666; width:460px; height:300px; '"

"<form action ='guardar_horario' method='get'> "
"<br>";


String Titulo1 = "<h2>"
"<label>Luz sala...............: </label>"  ;

String FeActual = "<h2>"
"<label1>Fecha : </label1>"  ;

String AlmTitulo1 = "<h2>"
"<br>"
"<label1>Alarmas. . . . : </label1>"  ;

String PagWebHora1 = "<h2>"
"</form> "
"</h2>";

String Titulo2 = "<h2>"
"<label>Cascada y Luces...: </label>"  ;

String PagWebHora2 = "<h2>"
"</form> "
"</h2>";

String Titulo3 = "<h2>"
"<label>Figuras..................: </label>"    ;

String Titulo4 = "<h2>"
"<label>Luz entrada......: </label>"    ;

String PagWebHora3 = "<h2>"
"</form> "
"</h2>";

String PagWebHora4 = "<h2>"
"</fieldset"
"<br>"
"</center>" 
"</h2>";


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

//"<br><br>"
//"<form action ='BuscaWifi' method='get' target='pantalla'>"
// "<center>"
//   "<fieldset align='center' style='border-style:solid; border-color:#336666; width:400px; height:50px; '"
//
//   "<br>" 
//   "<input class='boton'  type='submit' value='ESCANEAR - WIFI'  style='width:70%;height:50px'  />"
//   "<iframe id='pantaEsc' name='pantaEsc' src='' width:50px height:50px frameborder='0' scrolling='no' >  </iframe>"
//
//   "</fieldset>" 
// "</center>" 
// "</form>"

"<center>"
"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:420px; height:210px; display: inline-block; margin:1px; ' > "
"<form action ='configWifi' method='get' target='pantalla'>"
  "<center>"
    "<fieldset align='center' style='border-style:solid; border-color:#336666; width:380px; height:170px; '"
    "<h2>"
    "<label1><b>- CONFIGURAR RED-WIFI -</label1>"  
    "<br><br>"
    "</h2>"
   
    "<label for='usuario'> Usuario Wi-fi  .......: </label>"
    "<input class='input1' type='text' name='ssid'     id='ssid' placeholder='Wi-fi'    onfocus='FocoColor1(this)' style='width:50%;height:40px' >"
    "<br><br>"
    "<label for='pas'> Clave de acceso ...: </label> "
    "<input class='input1' type='password' name='pass' id='pass' placeholder='password' onfocus='FocoColor1(this)' style='width:50%;height:30px' >"
    "<br><br>" 
    "<input class='boton'  type='submit' value='GUARDAR DATOS' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
    "</fieldset>" 
  "</center>" 
  "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
"</form>" 
"</div>"
"</center>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:420px; height:210px; display: inline-block; margin:1px; ' > "
"<form action ='configDate' method='get' target='pantalla'>"
  "<center>"
    "<fieldset align='center' style='border-style:solid; border-color:#336666; width:380px; height:180px; '"
    "<h2>"
    "<label1>- CONFIGURAR Fecha/Hora -</label1>"  
    "<br><br>"
    "</h2>"

    "<label for='Fecha dd/mm/año'> Fecha.......: </label>"
    "<input class='input1' type='text' name='diaDate'  id='diaDate' placeholder='Dia'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' ;  align='center'    >"
    "<input class='input1' type='text' name='mesDate'  id='mesDate' placeholder='Mes'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' >"
    "<input class='input1' type='text' name='anoDate'  id='anoDate' placeholder='Año'   onfocus='FocoColor(this)' style='width:15%;height:40px ;border-radius:10px' >"
    "<br><br>"

    "<label for='Hora'> Hora.......: </label>"
    "<input class='input1' type='text' name='horaDate' id='horaDate' placeholder='Hora'    onfocus='FocoColor(this)' style='width:20%;height:40px ;border-radius:10px'>"
    "<input class='input1' type='text' name='minDate'  id='minDate'  placeholder='Minutos' onfocus='FocoColor(this)' style='width:20%;height:40px ;border-radius:10px'>"
    
    "<br><br>" 
    "<input class='boton'  type='submit' value='GUARDAR FECHA' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
    "<br><br>" 
   
    "</fieldset>" 
  "</center>" 
  "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
"</form>" 
"</div>"
;


String pagHoras = "<!DOCTYPE html>"
"<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
  "<style type ='text/css'> body,td,th {color:#036;} label1{color: rgb(240,6,6);} label2{color: rgb(250,246,9,0.952);} body{background-color:#a0cbf3;} "
  "</style>"
  "<script>"
    "function envioHorarios() {"
    " "
    " "
    "}"
  "</script>"
"</head>"

"<body>"
"<center>"
  "<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 460px; height:300px; display: inline-block; margin: 30px; ' > "

    "<h2><label1>- PROGRAMAR HORARIOS -</label1></h2>"    
    "<form action ='configHor1' method='get' target='pantalla'>"
         "<h2>"
             "<label>Luz sala..: </label>" 
             "<input type='number' name='hora1' id='hora1' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:10%;height:35px' align='left'> "
             "<input type='number' name='minu1' id='minu1' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:10%;height:35px'> "
             "<input type='number' name='dura1' id='dura1' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:10%;height:35px'> "
             "<input class='boton'  type='submit' value='Actualiza' style='width:15%;height:40px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
         "</h2>"
    "</form>" 

    "<form action ='configHor2' method='get' target='pantalla'>"
        "<h2>"
             "<label>Casc/Luces: </label>"   
             "<input type='number' name='hora2' id='hora2' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:10%;height:35px' align='left'> "
             "<input type='number' name='minu2' id='minu2' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:10%;height:35px'> "
             "<input type='number' name='dura2' id='dura2' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:10%;height:35px'> "
             "<input class='boton' type='submit' value='Actualiza' style='width:15%;height:40px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
        "</h2>"
    "</form>"

    "<form action ='configHor3' method='get' target='pantalla'>"
        "<h2>"
             "<label>Figuras...: </label>"   
             "<input type='number' name='hora3' id='hora3' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:10%;height:35px' align='left'> "
             "<input type='number' name='minu3' id='minu3' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:10%;height:35px'> "
             "<input type='number' name='dura3' id='dura3' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:10%;height:35px'> "
             "<input class='boton' type='submit' value='Actualiza' style='width:15%;height:40px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
        "</h2>"
    "</form>"

    "<form action ='configHor4' method='get' target='pantalla'>"
        "<h2>"
             "<label>Luz entr..: </label>"  
             "<input type='number' name='hora4' id='hora4' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:10%;height:35px' align='left'> "
             "<input type='number' name='minu4' id='minu4' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:10%;height:35px'> "
             "<input type='number' name='dura4' id='dura4' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:10%;height:35px'> "
             "<input class='boton' type='submit' value='Actualiza' style='width:15%;height:40px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
        "</h2>"
    "</form>"



    
  "</div>"
"</center>"
"<br><br>" 

"<center>"
"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
  "<form action ='Prender1' method='post' target='pantalla'>"
     "<h2> <label1> Luz sala</label1></h2>"  
     "<input class='boton' type='submit' id='SalaOn' value='ON'  onclick='prueba() ' style='width:50%;height:60px ;background-color:red   ;border-radius:10px'  />"
  "</form>"

  "<form action ='Apagar1' method='get' target='pantalla'>"
     "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px'  />"
  "</form>"
"</div>"
"<br><br>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
   "<form action ='Prender2' method='get' target='pantalla'>"
     "<h2> <label1>Cascada y Luces</label1></h2>"  
     "<input class='boton'  type='submit' value='ON'  style='width:50%;height:60px ;background-color:rgba(250,246,9,0.952) ;border-radius:10px' />"
   "</form>"

   "<form action ='Apagar2' method='get' target='pantalla'>"
     "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px '  />"
   "</form>"
"</div"
"<br><br>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
   "<form action ='Prender3' method='get' target='pantalla'>"
     "<h2> <label1>Figuras</label1></h2>"  
     "<input class='boton'  type='submit' value='ON'  style='width:50%;height:60px ;background-color:rgba(240,166,166, 0.8) ;border-radius:10px'  />"
   "</form>"
 
   "<form action ='Apagar3' method='get' target='pantalla'>"
      "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px'  />"
   "</form>"
"</div>"
"<br><br>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
   "<form action ='Prender4' method='get' target='pantalla'>"
     "<h2> <label1>Luz entrada</label1></h2>"  
       "<input class='boton'  type='submit' value='ON'  style='width:50%;height:60px ;background-color:#b8ce40 ;border-radius:10px'  />"
   "</form>"

   "<form action ='Apagar4' method='get' target='pantalla'>"
      "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px'  />"
   "</form>"
"</div>"
"<br><br>"

"</body>"
; // fin String pagHoras




void handleRoot() {
     //digitalWrite(led, 1);
     
     server.send(200, "text/html", mensaje + PagWeb +
                           FeActual+String(anorel)+"/"+ String(mesrel)+"/"+ String(diarel)+ "  --  "+
                           String(hrel1)+":"+ String(mrel1)+
                           Titulo1+String(h1)+":"+String(m1)+" / "+String(dr1)+" Min"+PagWebHora1 + 
                           Titulo2+String(h2)+":"+String(m2)+" / "+String(dr2)+" Min"+PagWebHora2 +
                           Titulo3+String(h3)+":"+String(m3)+" / "+String(dr3)+" Min"+PagWebHora3 +
                           Titulo4+String(h4)+":"+String(m4)+" / "+String(dr4)+" Min"+PagWebHora4 +
                           pagHoras + EstBoton1+ EstBoton2+ EstBoton3+  EstBoton4 + pagina 
                           ); 

    server.on("/configDate"  , wifi_Date); 
    //server.on("/configWifi"  , wifi_conf); 
    server.on("/configHor1", graba_Horas1); 
    server.on("/configHor2", graba_Horas2); 
    server.on("/configHor3", graba_Horas3);     
    server.on("/configHor4", graba_Horas4);     
    server.on("/configAlm", graba_Alm);     
    server.on("/Prender1", Sala_On); 
    server.on("/Apagar1" , Sala_Of); 
    server.on("/Prender2", Sala_On2); 
    server.on("/Apagar2" , Sala_Of2); 
    server.on("/Prender3", Sala_On3); 
    server.on("/Apagar3" , Sala_Of3); 
    server.on("/Prender4", Sala_On4); 
    server.on("/Apagar4" , Sala_Of4); 
  
}

void handleNotFound() {
  //digitalWrite(led, 1);
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
  //digitalWrite(led, 0);
}

void setup(void) {
  //pinMode(led, OUTPUT);
  //digitalWrite(led, 0);
  Serial.begin(115200);
  EEPROM.begin(4096);

  #ifndef ESP8266
     while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
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

    Almh1  = EEPROM.read(111) ; // Hora alarma 1
    Almm1  = EEPROM.read(112) ; // Min. alarma 1   
    Almt1  = EEPROM.read(113) ; // Tiempo alarma 1   
    
    Almh2  = EEPROM.read(114) ; // Hora alarma 2   
    Almm2  = EEPROM.read(115) ; // Min. alarma 2      
    Almt2  = EEPROM.read(116) ; // Tiempo alarma 2       
    
    Almh3  = EEPROM.read(117) ; // Hora alarma 3    
    Almm3  = EEPROM.read(118) ; // Min. alarma 3      
    Almt3  = EEPROM.read(119) ; // Tiempo alarma 3      

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
 ///  no colocar -     pinMode(PinAlarma,  OUTPUT)  ;    // TX - GPIO1 PinAlarma


  WiFi.mode(WIFI_STA);
  //IPAddress ip(192,168,0,39);
  //IPAddress gateway(192,168,0,1);
  //IPAddress subnet(255,255,255,0);
  //WiFi.config(ip,gateway,subnet) ;
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


void wifi_Date(){
  String diaDateEnt  = server.arg("diaDate") ; 
  String mesDateEnt  = server.arg("mesDate") ; 
  String anoDateEnt  = server.arg("anoDate") ; 
  String horaDateEnt = server.arg("horaDate") ; 
  String minDateEnt  = server.arg("minDate") ; 

  if (horaDateEnt != "")
    {
     int hrelx = horaDateEnt.toInt()   ; 
     DateTime nowx = rtc.now();
     int mrelx = nowx.minute() ;
     rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }

  if (minDateEnt != "")
    {
     DateTime nowx = rtc.now(); 
     int hrelx = nowx.hour() ; 
     int mrelx = minDateEnt.toInt()  ;
     rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }
    
  if (diaDateEnt != "")
    {
     DateTime nowx = rtc.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();  
     int diarel = diaDateEnt.toInt() ;
     rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }

  if (mesDateEnt != "")
    {
     mesrel = mesDateEnt.toInt() ;
     DateTime nowx = rtc.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();    
     rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));     
    }  

  if (anoDateEnt != "")
    {
     anorel = anoDateEnt.toInt() ; 
     DateTime nowx = rtc.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();    
     rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));          
    }
}

void graba_Horas1() {
 
      String horaEnt1 = (server.arg("hora1"));
      String minuEnt1 = (server.arg("minu1"));
      String duraEnt1 = (server.arg("dura1"));
  
      // Horario Uno
      if (horaEnt1 != ""){
        h1  = horaEnt1.toInt() ;
        EEPROM.write(100, h1); 
        EEPROM.commit();
        delay(10);
      }

     if (minuEnt1 != ""){
        m1  = minuEnt1.toInt() ;
        EEPROM.write(101, m1);  
        EEPROM.commit();
        delay(10);
      }

     if (duraEnt1 != ""){
        dr1 = duraEnt1.toInt() ;
        EEPROM.write(108, dr1);  
        EEPROM.commit();
        delay(10);
      }
}

void graba_Horas2() {
  
     String horaEnt2 = (server.arg("hora2"));
     String minuEnt2 = (server.arg("minu2"));  
     String duraEnt2 = (server.arg("dura2"));  

     // Horario Dos  
     if (horaEnt2 != ""){
        h2  = horaEnt2.toInt() ;
        EEPROM.write(102, h2);  
        EEPROM.commit();
        delay(10);
      }

      if (minuEnt2 != ""){
        m2 = minuEnt2.toInt() ;
        EEPROM.write(103, m2);  
        EEPROM.commit();
        delay(10);
      }

      if (duraEnt2 != ""){
        dr2 = duraEnt2.toInt() ;
        EEPROM.write(109, dr2);  
        EEPROM.commit();
        delay(10);
      }
}

void graba_Horas3() {

      String horaEnt3 = (server.arg("hora3"));
      String minuEnt3 = (server.arg("minu3"));
      String duraEnt3 = (server.arg("dura3"));
 
      // Horario Tres
      if (horaEnt3 != ""){
        h3  = horaEnt3.toInt() ;
        EEPROM.write(104, h3);  
        EEPROM.commit();
        delay(10); 
      }

      if (minuEnt3 != ""){
        m3  = minuEnt3.toInt() ;
        EEPROM.write(105, m3);  
        EEPROM.commit();
        delay(10); 
      }

      if (duraEnt3 != ""){
        dr3 = duraEnt3.toInt() ;
        EEPROM.write(110, dr3);  
        EEPROM.commit();
        delay(10);
      }
}



void graba_Horas4() {

      String horaEnt4 = (server.arg("hora4"));
      String minuEnt4 = (server.arg("minu4"));
      String duraEnt4 = (server.arg("dura4"));  

      // Horario Cuatro
      if (horaEnt4 != ""){
        h4  = horaEnt4.toInt() ;
        EEPROM.write(120, h4);  
        EEPROM.commit();
        delay(10); 
      }

      if (minuEnt4 != ""){
        m4  = minuEnt4.toInt() ;
        EEPROM.write(121, m4);  
        EEPROM.commit();
        delay(10); 
      }

      if (duraEnt4 != ""){
        dr4 = duraEnt4.toInt() ;
        EEPROM.write(122, dr4);  
        EEPROM.commit();
        delay(10);
      }
}






void graba_Alm() {
  String AlmhoraEnt1 = (server.arg("Almhora1"));
  String AlmminuEnt1 = (server.arg("Almminu1"));
  String AlmduraEnt1 = (server.arg("Almdura1"));
  
  String AlmhoraEnt2 = (server.arg("Almhora2"));
  String AlmminuEnt2 = (server.arg("Almminu2"));  
  String AlmduraEnt2 = (server.arg("Almdura2"));  
  
  String AlmhoraEnt3 = (server.arg("Almhora3"));
  String AlmminuEnt3 = (server.arg("Almminu3"));
  String AlmduraEnt3 = (server.arg("Almdura3"));

      // Horario Uno Alarma
      if (AlmhoraEnt1 != ""){
        Almh1  = AlmhoraEnt1.toInt() ;
        EEPROM.write(111, Almh1); 
        EEPROM.commit();
        delay(10);
      }

     if (AlmminuEnt1 != ""){
        Almm1  = AlmminuEnt1.toInt() ;
        EEPROM.write(112, Almm1);  
        EEPROM.commit();
        delay(10);
      }

     if (AlmduraEnt1 != ""){
        Almt1 = AlmduraEnt1.toInt() ;
        EEPROM.write(113, Almt1);  
        EEPROM.commit();
        delay(10);
      }


      // Horario Dos Alarma
      if (AlmhoraEnt2 != ""){
        Almh2  = AlmhoraEnt2.toInt() ;
        EEPROM.write(114, Almh2); 
        EEPROM.commit();
        delay(10);
      }

     if (AlmminuEnt2 != ""){
        Almm2  = AlmminuEnt2.toInt() ;
        EEPROM.write(115, Almm2);  
        EEPROM.commit();
        delay(10);
      }

     if (AlmduraEnt2 != ""){
        Almt2 = AlmduraEnt2.toInt() ;
        EEPROM.write(116, Almt2);  
        EEPROM.commit();
        delay(10);
      }

      // Horario Tres Alarma
      if (AlmhoraEnt3 != ""){
        Almh3  = AlmhoraEnt3.toInt() ;
        EEPROM.write(117, Almh3); 
        EEPROM.commit();
        delay(10);
      }

     if (AlmminuEnt3 != ""){
        Almm3  = AlmminuEnt3.toInt() ;
        EEPROM.write(118, Almm3);  
        EEPROM.commit();
        delay(10);
      }

     if (AlmduraEnt3 != ""){
        Almt3 = AlmduraEnt3.toInt() ;
        EEPROM.write(119, Almt3);  
        EEPROM.commit();
        delay(10);
      }
}

void graba(int addr, String a) {
  int tamano = (a.length()+1) ;
  Serial.print(tamano);
  char inchar[tamano] ;
  a.toCharArray(inchar, tamano); 
  EEPROM.write(addr, tamano);  
  for (int i=0; i<tamano; i++) {
    addr++ ;
    EEPROM.write(addr, inchar[i]);  
   }
  EEPROM.commit();
}

String lee(int addr) {
  String nuevostring ;
  int valor ;
  int tamano=EEPROM.read(addr) ;
  for (int i=0; i<tamano; i++) {
     addr++;
     valor=EEPROM.read(addr);
     nuevostring +=(char)(valor);
    }
  return nuevostring ;
}

void paginaconf() {
    // server.send(200, "text/html", pagina + mensaje + paginafin ) ;     
    // server.send(200, "text/html",String("<h2>Conexion Exitosa" + getssid) ) ;     
}


 
//void modoconf() {
//    delay(100) ;
//    digitalWrite(13, HIGH);
//    delay(100) ;
//    digitalWrite(13, LOW);
//    delay(100) ;
//    digitalWrite(13, HIGH);
//    delay(100) ;
//    digitalWrite(13, LOW);
// 
//    WiFi.softAP(ssidConf, passConf)  ;
//    IPAddress myIp = WiFi.softAPIP();
//    Serial.print("IP del acceso point .. :");
//    Serial.println(myIp);
//    Serial.println(" - WebServer Inicializado - ");
//
//    server.on("/", paginaconf); 
//    server.on("/guardar_conf", guardar_conf); 
//    server.on("/escanear", escanear); 
//    
//    server.begin();
//    
//    while (true) {
//      server.handleClient();
//    }
//}

//void guardar_conf() {
//   Serial.println(server.arg("ssid"));  // Recibimos los valores que envia el GET, del formulario Web
//   graba(0,server.arg("ssid")) ;
//   Serial.println(server.arg("pass"));
//   grabar(50,server.arg("pass")) ;
//
//   mensaje = "Configuracion Guardada" ;
//   paginaconf() ;
//}
//
//void setup_Wifi() {
//   WiFi.mode(WIFI_STA); // para que no el SoftAP en modo normal, sino como estacion
//   WiFi.begin(ssid1, pass1);  
//   
//   while (WiFi.status() != WL_CONNECTED and contconexion <50) {
//    ++contconexion;
//    delay(250);
//    Serial.print(".");
//    }
//
//    if (contconexion<50){
//      // IPAddress ip(192,168,0,10);
//      // IPAddress gateway(192,168,0,1);
//      // IPAddress subnet(255,255,255,0);
//      // WiFi.config(ip,gateway,subnet) ;
//
//      Serial.println("");
//      Serial.println("WiFi connected");
//      Serial.println(WiFi.localIP());
//      digitalWrite(13, HIGH);
//
//    }
//    else {
//      Serial.println("");
//      Serial.println("- Error de conexion - ");
//      digitalWrite(13, LOW);
//    } 
//
//}  // final de setup_Wifi

void escanear() {
   // hoy07sep22
   //digitalWrite(PinLuzSala, LOW); // D0 - GPIO16 - Prende dispositivo Uno
   int n = WiFi.scanNetworks(); // Busca las redes instaladas
   Serial.println("- Escaneo Terminado -");
   if (n==0)
     {//no hay redes
      Serial.println("- No hay redes -");
      mensaje = "- No hay redes -" ;
     }
   else
     {
      Serial.print(n);      
      Serial.println("- Redes encontradas -");
      mensaje = "" ;

      for (int i=0; i<n; ++i) {
        mensaje = (mensaje) + "<p>" + String(i+1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") Ch :" + "</p>\r\n" ;   
        delay(10) ; 
      }
      Serial.println(mensaje);  
      //paginaconf() ; 
      //server.send(200, "text/html",String("<h2>Conexion Exitosa" + getssid) ) ;     
     }  
}

void Sala_On() { 
  // Utiliza Modulo - Rele 
  int EstLuzSala =  digitalRead(PinLuzSala) ;
  if (EstLuzSala==HIGH)
    {
     digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno
     SwPir2 = false ;
     Boton1 = 1 ;
     aviso() ;      
    }
}

void Sala_Of(){
  int EstLuzSala =  digitalRead(PinLuzSala) ;
  if (EstLuzSala==LOW)
    {
     digitalWrite(PinLuzSala, HIGH); // Apaga dispositivo Uno
     SwPir2 = true ;
     Boton1 = 0 ;
     aviso() ;
    }
}

void Sala_On2() { 
  int EstPinLuces =  digitalRead(PinLuces) ;
  if (EstPinLuces==LOW)
    {
     digitalWrite(PinLuces,  HIGH) ; // Prende Luces pecebre  
     digitalWrite(PinCascada,HIGH) ; // Prende cascada
     Boton2 = 1 ;
     aviso() ;
    }
}

void Sala_Of2(){
  int EstPinLuces =  digitalRead(PinLuces) ;
  if (EstPinLuces==HIGH)
    {
     digitalWrite(PinLuces, LOW)  ; // Apaga Luces pecebre     
     digitalWrite(PinCascada,LOW) ; // Apaga cascada
     Boton2 = 0 ;  
     aviso() ;
    }
}

// hoy17Dic
void Sala_On3(){ 
  int EstPinLava =  digitalRead(PinFiguras) ;
  if (EstPinLava==LOW)
    {
     digitalWrite(PinFiguras,HIGH) ; // Prende figuras 
     swFigPrender = true ;
     Boton3 = 1 ;
     aviso() ;
     SwPir1 = false ;
     SwPir2 = false ;
    }
}

// hoy17Dic
void Sala_Of3(){
  int EstPinLava =  digitalRead(PinFiguras) ;
  if (EstPinLava==HIGH)
    {
     digitalWrite(PinFiguras,LOW); // Apaga figuras 
     swFigPrender = false ;
     Boton3 = 0 ;
     aviso() ;
     SwPir1 = true ;
     SwPir2 = true ;
    }
}

// Luz entrada
void Sala_On4(){ 
  int EstPinEnt =  digitalRead(PinLuzEnt) ;
  if (EstPinEnt==HIGH)
    {
     digitalWrite(PinLuzEnt,LOW) ; // Prende Luz entrada
     SwPir3 =false ;
     Boton4 = 1 ;
     aviso() ;
    }
}

// Luz entrada
void Sala_Of4(){
  int EstPinEnt =  digitalRead(PinLuzEnt) ;
  if (EstPinEnt==LOW)
    {
     digitalWrite(PinLuzEnt,HIGH); // Apaga Luz entrada
     SwPir3=true ;
     Boton4 = 0 ;
     aviso() ;
    }
}

void ModoAutomatico() {
   DateTime now = rtc.now();
   if (now.hour()==h1 && now.minute()==m1 && dr1 !=0) {
      digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno
      Boton1 = 1 ;
      aviso() ;
     
      minumov1 = m1 ;
      horamov1 = h1 ; 
      swCalcuMin1 = true ;
      AcuHora1    = 0 ;
      AcuMin1     = 0 ;

      hrelant1 = h1 ;
      mrelant1 = m1 ;
      SwPir2   = false ;    
   }

   if (swCalcuMin1==true) {
      totalMinutos1() ;
      if (AcuMin1>=dr1)
        {
         digitalWrite(PinLuzSala, HIGH); // Apaga dispositivo Uno
         Boton1 = 0 ;
         aviso() ;
         swSonido1 = true ;
         swCalcuMin1 = false ;
         SwPir2 = true ;  
        }
   } //// Fin horario uno 
        
   //// ------------------------------------------------------------- Inicio horario dos 
   if (now.hour()==h2 && now.minute()==m2 && dr2 !=0) {
      aviso2() ;
      delay(2000) ;
      digitalWrite(PinLuces,  HIGH) ; // Prende Luces pecebre  
      digitalWrite(PinCascada,HIGH) ; // Prende cascada
      Boton2 = 1 ;
           
      minumov2 = m2 ;
      horamov2 = h2 ; 
      swCalcuMin2 = true ;
      AcuHora2    = 0 ;
      AcuMin2     = 0 ;

      hrelant2 = h2 ;
      mrelant2 = m2 ;
      SwPir1   = false ;      
   }

   if (swCalcuMin2==true) {
      totalMinutos2() ;
      if (AcuMin2>=dr2)
        {
         digitalWrite(PinLuces,  LOW) ; // Apaga Luces pecebre     
         digitalWrite(PinCascada,LOW) ; // Apaga cascada    
         Boton2 = 0 ; 
         aviso() ;       
         swCalcuMin2 = false ;
         SwPir1 = true ;     
        }
   }   //// Fin horario dos
     

   //// ------------------------------------------------------Inicio horario tres      
   if (now.hour()==h3 && now.minute()==m3 && dr3 !=0) {
      digitalWrite(PinFiguras,HIGH) ; // Prende lavandera 
      Boton3 = 1 ;
      aviso3() ;
          
      Servo1Levanta() ;
      minumov3 = m3 ;
      horamov3 = h3 ; 
      swCalcuMin3 = true ;
      AcuHora3    = 0 ;
      AcuMin3     = 0 ;

      hrelant3 = h3 ;
      mrelant3 = m3 ;
   }
   
   if (swCalcuMin3==true) {
      totalMinutos3() ;
      if (AcuMin3>=dr3)
        {
         digitalWrite(PinFiguras,LOW) ; // Apaga lavandera 
         Boton3 = 0 ;
         aviso() ;
          
         swFigPrender = false ;
         swCalcuMin3 = false ;
        }
      else 
        {
         swFigPrender = true ;
        }  
   }
   //// Fin horario tres

   //// ------------------------------------------------------Inicio horario cuatro - Luz entrada
   if (now.hour()==h4 && now.minute()==m4 && dr4 !=0) {
      digitalWrite(PinLuzEnt,LOW) ; // Prende Luz entrada
      Boton4 = 1 ;
      aviso4() ;      
     
      minumov4 = m4 ;
      horamov4 = h4 ; 
      swCalcuMin4 = true ;
      AcuHora4    = 0 ;
      AcuMin4     = 0 ;

      hrelant4 = h4 ;
      mrelant4 = m4 ;
      SwPir3   = false ;    
   }

   if (swCalcuMin4==true) {
      totalMinutos4() ;
      if (AcuMin4>=dr4)
        {
         digitalWrite(PinLuzEnt,HIGH)  ; // Apaga Luz entrada 
         Boton4 = 0 ;
         aviso() ;
         swSonido4 = true ;
         swCalcuMin4 = false ;
         SwPir3 = true ;  
        }
   } //// Fin horario cuatro


   //// ------------------------------------------------------------- Inicio alarma uno 
   if (now.hour()==Almh1 && now.minute()==Almm1 && Almh1!=0)
     {
      if (SwAlarma1==true)
        {
         aviso() ;
         SwAlarma1=false ; 
        }
      }
   else 
      {
       SwAlarma1=true ;
      }

   //// ------------------------------------------------------------- Inicio alarma dos 
   if (now.hour()==Almh2 && now.minute()==Almm2 && Almh2!=0)
     {
      if (SwAlarma2==true)
        {
         aviso() ;
         SwAlarma2=false ; 
        }
      }
   else 
      {
       SwAlarma2=true ;
      }

   //// ------------------------------------------------------------- Inicio alarma tres 
   if (now.hour()==Almh3 && now.minute()==Almm3 && Almh3!=0)
     {
      if (SwAlarma3==true)
        {
         aviso() ;
         SwAlarma3=false ; 
        }
      }
   else 
      {
       SwAlarma3=true ;
      }
}   


void Servo1Levanta()
  {
//   myservo1.attach(13);
//
//   int pos;
//   for (pos = 0; pos <= 100; pos += 1) { 
//    myservo1.write(pos);             
//    delay(10);                       
//   }
//   for (pos = 100; pos >= 0; pos -= 1) { 
//    myservo1.write(pos);             
//    delay(10);                       
//   }
//
////   for (pos = 0; pos <= 130; pos += 1) { 
////    myservo2.write(pos);              
////    delay(5);                       
////   }
////   
////   for (pos = 130; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
////    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
////    delay(5);                       // waits 15ms for the servo to reach the position
////   }
//
//   myservo1.detach();
//   //myservo2.detach();
}

void totalMinutos1 () {
  DateTime now1 = rtc.now();
  hrel1    = now1.hour()   ;  
  mrel1    = now1.minute() ;

  if (mrel1 == mrelant1) 
    { }
  else
    { AcuMin1  = AcuMin1 + 1 ;
      mrelant1 = mrel1 ;
    }  
}

void totalMinutos2 () {
  DateTime now1 = rtc.now();
  hrel2    = now1.hour()   ;  
  mrel2    = now1.minute() ;

  if (mrel2 == mrelant2) 
    { }
  else
    { AcuMin2  = AcuMin2 + 1 ;
      mrelant2 = mrel2 ;
    }  
}

void totalMinutos3 () {
  DateTime now1 = rtc.now();
  hrel3    = now1.hour()   ;  
  mrel3    = now1.minute() ;

  if (mrel3 == mrelant3) 
    { }
  else
    { AcuMin3  = AcuMin3 + 1 ;
      mrelant3 = mrel3 ;
    }  
}  

void totalMinutos4 () {
  DateTime now1 = rtc.now();
  hrel4    = now1.hour()   ;  
  mrel4    = now1.minute() ;

  if (mrel4 == mrelant4) 
    { }
  else
    { AcuMin4  = AcuMin4 + 1 ;
      mrelant4 = mrel4 ;
    }  
} 




void aviso() {
  digitalWrite(PinAlarma,HIGH)   ; // Prende PinAlarma
  delay(400) ;
  digitalWrite(PinAlarma,LOW)    ; // Apaga la Alarma
}


void aviso1() {
  if (swSonido1 == true)
    {
     digitalWrite(PinAlarma,HIGH)  ; // Prende PinAlarma
     delay(400) ;
     digitalWrite(PinAlarma,LOW)   ; // Apaga la Alarma
     swSonido1 = false ;
    }
}

void aviso2() {
  if (swSonido2 == true)
    {
     digitalWrite(PinAlarma,HIGH)  ; // Prende PinAlarma
     delay(400) ;
     digitalWrite(PinAlarma,LOW)   ; // Apaga la Alarma
     swSonido2 = false ;
    }
}

void aviso3() {
  if (swSonido3 == true)
    {
     digitalWrite(PinAlarma,HIGH)  ; // Prende PinAlarma
     delay(400) ;
     digitalWrite(PinAlarma,LOW)   ; // Apaga la Alarma
     swSonido3 = false ;
    }
}

void aviso4() {
  if (swSonido4 == true)
    {
     digitalWrite(PinAlarma,HIGH)  ; // Prende PinAlarma
     delay(400) ;
     digitalWrite(PinAlarma,LOW)   ; // Apaga la Alarma
     swSonido4 = false ;
    }
}

void estadoBoton(){
    if (swIniRel1 == true)
    {
     digitalWrite(PinLuzSala, HIGH); // Apaga Rele-"dispositivo Uno" por primera vez
     digitalWrite(PinLuzEnt,HIGH)  ; // Apaga Rele-"dispositivo Dos" por primera vez
     swIniRel1 = false ;
     Boton1 = 0 ;      
    };

  if (Boton1 == 1)
    {
     EstBoton1 =( "<center>" "<h2>" "<label2>" "Boton1 / Prendido " "</label2>" "</h2>" "</center>") ;
    }
  else
    {
     EstBoton1 =( "<center>" "<h2>" "<label>" "Boton1 / Apagado "  "</label>" "</h2>" "</center>") ;
    }  

  if (Boton2 == 1)
    {
     EstBoton2 =( "<center>" "<h2>" "<label2>" "Boton2 / Prendido " "</label2>" "</h2>" "</center>") ;
    }
  else
    {
     EstBoton2 =( "<center>" "<h2>" "<label>" "Boton2 / Apagado "  "</label>" "</h2>" "</center>") ;
    }  

  if (Boton3 == 1)
    {
     EstBoton3 =( "<center>" "<h2>" "<label2>" "Boton3 / Prendido " "</label2>" "</h2>" "</center>") ;
    }
  else
    {
     EstBoton3 =( "<center>" "<h2>" "<label>" "Boton3 / Apagado "  "</label>" "</h2>" "</center>") ;
    }  

  if (Boton4 == 1)
    {
     EstBoton4 =( "<center>" "<h2>" "<label2>" "Boton4 / Prendido " "</label2>" "</h2>" "</center>") ;
    }
  else
    {
     EstBoton4 =( "<center>" "<h2>" "<label>" "Boton4 / Apagado "  "</label>" "</h2>" "</center>") ;
    } 

}

void leerPir1(){
   Sw_Movto = digitalRead(PinMovto) ; 
   Serial.print("El movimiento es ..: ");
   Serial.print(Sw_Movto);  

   if (Sw_Movto==1)
     {
       digitalWrite(PinLuces, HIGH)   ; // Prende Luces pecebre  
       // Hoy14Dic
       // digitalWrite(PinCascada,HIGH)  ; // Prende cascada
       digitalWrite(PinFiguras,HIGH)  ; // Prende lavandera   
       digitalWrite(PinAlarma,HIGH)   ; // Prende PinAlarma
       swMovimiento=true ;
       //Servo1Levanta() ;  
     }
   else
     { 
      if (swMovimiento==true)
        {
         swMovimiento=false ;
         digitalWrite(PinLuces,   LOW) ; // Apaga Luces pecebre 
         // Hoy14Dic 
         // digitalWrite(PinCascada, LOW) ; // Apaga cascada
         digitalWrite(PinFiguras, LOW) ; // Apaga lavandera          
         digitalWrite(PinAlarma,  LOW) ; // Apaga PinAlarma
      }
     }
}

void leerPir2(){
   Sw_Movto2  = digitalRead(PinMovto2) ; 
   if (Sw_Movto2==1)
     {
      digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno Luz sala
      swMovimiento2=true ;
     }
   else
     { 
      if (swMovimiento2==true)
        {
         swMovimiento2=false ;
         digitalWrite(PinLuzSala, HIGH); // Apaga dispositivo Uno Luz sala
        }
     }
}

void leerPir3(){
   Sw_Movto3  = digitalRead(PinMovto3) ; 
   if (Sw_Movto3==1)
     {
      digitalWrite(PinLuzEnt, LOW); // Prende dispositivo Tres Luz entrada
      swMovimiento3=true ;
     }
   else
     { 
     if (swMovimiento3==true)
       {
        swMovimiento3=false ;
        digitalWrite(PinLuzEnt, HIGH); // Apaga dispositivo Tres Luz entrada
       }
     }
}
