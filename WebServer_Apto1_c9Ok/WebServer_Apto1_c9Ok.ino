// W E B - S E R V E R 
// Crea un SERVIDOR y el mismo le asigna una IP - 192.168.0.60 /
// Para ingresar con esta IP: 192.168.4.1  se  debe entrar a los Wifi y concetar el "Tempo_Apto1", esta es necesaria cuando 
// se va a guardar por primera vez el Usuario y la contraseña de la red donde nos queremos conectar.
// Para ingresar con esta IP: 192.168.0.60 se  debe entrar a los dispositovos  Wifi y concetar el "10339240"
// No necesita el servidor apache
// Graba los datos con el Wifi al que se conecte
// El ESP8266 puede asignar la Ip automaticamente o la que determine el usuario
// Se ejecuta sin el "ADIN" o con El.
// Al llamarlo desde el "ADIN", se hace desde sus opciones aplicaciones como "Temporizador". pero para ello el "XAMPP", debe estar activo.
// Se puede ejecutar sin el "XAMPP"
// Se ejecuta en el navegador con 192.168.0.60/Sala_On 
// Graba la Clave y Wifi en memoria RAM
// Utiliza servo-motor en los dispositivos


// Inicio de la configuracion del Modulo Esp8266
// 1. Carge el programa
// 2. Busque dentro de la lista de las Redes de Internet del Computador, Tablet o Celular la llamada "Tempo_Apto1"
// 3. Seleccionela y conectela "Tempo_Apto1"
// 4. Ingrese al navegador y digite la URL (192.168.4.1)
// 5. Al ingresar a la APP, busque el boton "ESCANEAR - WIFI"
// 6. Refresque la pantalla
// 7. Digite el Usuario y la clave de acceso de su Red y presione "GUARDAR DATOS"
// 8. En el COM9 aparece la direccion Ip en la cual se debe conectar para manejar la APP

// Para poder trabajar con el PC o subir el codigo, se debe desconectar los negativos comunes 
// y el rx/tx, de lo contrario bloquea el ESP8266.
// El voltaje de los dispositivos es de 5.0 v.

// Feb/01/2021 // Se instala la opcion de "Aviso"
// Mar/25/2021 // Se instala la opcion de reloj con matriz LED 8x8


/* Para generar y guardaR la clave y el nombre del WiFi, siga los siguientes pasos :
   a.- En la tablet o Web busque el "Temporizador", dentro de la lista de dispositivos Wi-Fi disponibles 
   b.- Ingrese a el dispositivo y "conectelo". 
   c.- Ingrese al navegadory digite la URL : 192.168.4.1
   d.- El sistema le indicara que no esta "CONFIGURADO".
   e.- Desplace la pantalla hacia arriba, hasta llegr a "Usuario" y "Clave".
   f.- Digite la informacion correspondiente en los campos y presione la tecla "GUARDAR DATOS".
*/

/* Para cargar el codigo se debe desconectar al negativo que va desde el modulo a la entrada de 5v.*/

// Wire - Permite ejecurar la comunicacion I2C
// SPI  - Permite comunicar varios circuitos integrados entre si, distancias cortas y altas velocidades
//        (Serial Perifial Interface).
//        sincrono Maestro - Esclavo.


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>

// Matriz 8X8 - Reloj
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Sonidos Buzzer
#include <pitches.h>
//#include <themes.h>

#define NOTE_A3 220
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G3 196
#define NOTE_G4 392

//##############**"HE IS A PIRATE" Theme song of Pirates of caribbean**##############//
int Pirates_note1[] = {
       NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
       NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, 
       NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,  
       NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, 
       NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4
};   

int Pirates_duration1[] = {
4,8,4,8,4,8,8,8,8,4,8,4,8,4,8,8,8,8,4,8,
4,8,4,8,8,8,8,4,4,8,8,4,4,8,8,4,4,8,8,8
};

// set to 1 if we are implementing the user interface pot, switch, etc
#define USE_UI_CONTROL 0

#if USE_UI_CONTROL
  #include <MD_UISwitch.h>
#endif

// Turn on debug statements to the serial output
#define DEBUG 0

#if DEBUG
  #define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
  #define PRINTS(x) Serial.print(F(x))
  #define PRINTX(x) Serial.println(x, HEX)
#else
  #define PRINT(s, x)
  #define PRINTS(x)
  #define PRINTX(x)
#endif

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// Definicion Pines de la Matrix 8x8
#define CLK_PIN   D5
#define DATA_PIN  D7
#define CS_PIN    D8

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

volatile boolean SwSinMov     = false ;
volatile boolean swFigPrender = false ;
volatile boolean swCalcuMin1  = false ;
volatile boolean swCalcuMin2  = false ;
volatile boolean swCalcuMin3  = false ;
volatile boolean swMovimiento = false ;
volatile boolean swMovimiento2= false ;
volatile boolean swIniRel1    = true  ;
volatile boolean SwPir1       = true  ;
volatile boolean SwPir2       = true  ;

volatile boolean swSonido1 = true ;
volatile boolean swSonido2 = true ;
volatile boolean swSonido3 = true ;

volatile boolean SwAlarma1 = true ;
volatile boolean SwAlarma2 = true ;
volatile boolean SwAlarma3 = true ;

volatile int Boton1 ;
volatile int Boton2 ;
volatile int Boton3 ;

volatile int LuzEnt  ;
volatile int Sw_Movto1 ;
volatile int Sw_Movto2 ;

volatile int Vr_ADC_senLuz = 0 ;

const int pulsoMin  = 650 ;   // pulso em ms para girar a 0°
const int pulsoMax  = 2550 ;  // pulso em ms para girar a 180°
int angulo  ;
int angulo2 ;

int contconexion = 0 ;
unsigned long previoMillis = 0 ;
const int ledArbol    = 16 ;            // D0 - GPIO16 Luz entrada - Rele
//                                      // D1 - Clock  - Azul       
//                                      // D2 - Data   - Verde      
const int PinLuces    =  0 ;            // D3 - GPIO0  Luz baño - Rele  
const int PinMovto1   =  2 ;            // D4 - GPIO2  - P.I.R. - PinMovto1
const int PinMovto2   = 12 ;            // D6 - GPIO12 - P.I.R. - PinMovto2
//        CLK_PIN                       // D5
//        DATA_PIN                      // D7
//        CS_PIN                        // D8
const int PinOtros      =  3 ;          // RX - GPIO3  PinOtros
const int PinAlarma     =  1 ;          // TX - GPIO1  PinAlarma      
// S1,S2, S3 -  "No sirve"

RTC_DS3231 rtc;

// WiFiClient espClient ;
ESP8266WebServer server(80);


char strUrl ;
char ssid[20] ; 
char pass[20] ; 
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

String  mesStr  = " " ;
String  Horastr = " " ;
String  Minutostr = " " ;
String Fe_Actual ="";

// Scrolling parameters
#if USE_UI_CONTROL
const uint8_t SPEED_IN = A5;
const uint8_t DIRECTION_SET = 8;  // change the effect
const uint8_t INVERT_SET = 9;     // change the invert

const uint8_t SPEED_DEADBAND = 5;
#endif // USE_UI_CONTROL

uint8_t scrollSpeed = 5 ;    // A menor numero Mayor velocidad.
textEffect_t scrollEffect  = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;


uint16_t scrollPause = 30000; // Pausa para volver a iniciar el letrero(30 Seg.)

// Global message buffers shared by Serial and Scrolling functions
#define  BUF_SIZE  75
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "15:20" };
//char newMessage[BUF_SIZE] = { "Jue 15/Mar/21   10:13 " };
bool newMessageAvailable = true;

volatile int len_Fe_Actual = Fe_Actual.length()+1 ;
const char* Fe_Char= "" ;

#if USE_UI_CONTROL

MD_UISwitch_Digital uiDirection(DIRECTION_SET);
MD_UISwitch_Digital uiInvert(INVERT_SET);

void doUI(void)
{
  // set the speed if it has changed
  {
    int16_t speed = map(analogRead(SPEED_IN), 0, 1023, 10, 150);

    if ((speed >= ((int16_t)P.getSpeed() + SPEED_DEADBAND)) ||
      (speed <= ((int16_t)P.getSpeed() - SPEED_DEADBAND)))
    {
      P.setSpeed(speed);
      scrollSpeed = speed;
      PRINT("\nChanged speed to ", P.getSpeed());
    }
  }

  if (uiDirection.read() == MD_UISwitch::KEY_PRESS) // SCROLL DIRECTION
  {
    PRINTS("\nChanging scroll direction");
    scrollEffect = (scrollEffect == PA_SCROLL_LEFT ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
    P.setTextEffect(scrollEffect, scrollEffect);
    P.displayClear();
    P.displayReset();
  }

  if (uiInvert.read() == MD_UISwitch::KEY_PRESS)  // INVERT MODE
  {
    PRINTS("\nChanging invert mode");
    P.setInvert(!P.getInvert());
  }
}
#endif // USE_UI_CONTROL

void readSerial(void)
{

  static char *cp = newMessage;

  while (Serial.available())
  {
    *cp = (char)Serial.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2)) // end of message character or full buffer
    {
      *cp = '\0'; // end the string
      // restart the index for next filling spree and flag we have a message waiting
      cp = newMessage;
      newMessageAvailable = true;
    }
    else  // move char pointer to next position
      cp++;
  }
}

volatile int Almhrel1 =0 ;
volatile int Almhrel2 =0 ;
volatile int Almhrel3 =0 ;


volatile int hrelant1 =0 ;
volatile int hrelant2 =0 ;
volatile int hrelant3 =0 ;

volatile int mrel1 =0 ;
volatile int mrel2 =0 ;
volatile int mrel3 =0 ;

volatile int Almmrel1 =0 ;
volatile int Almmrel2 =0 ;
volatile int Almmrel3 =0 ;

volatile int mrelant1 =0 ;
volatile int mrelant2 =0 ;
volatile int mrelant3 =0 ;

volatile int vr_TgLLeno =0 ;
volatile int ti =0 ;

volatile int tgMinutos1 =0 ;
volatile int tgMinutos2 =0 ;
volatile int tgMinutos3 =0 ;

volatile int tanorel=0 ;
volatile int tmesrel=0 ;
volatile int tdiarel=0 ;

volatile int thorarel1=0 ;
volatile int thorarel2=0 ;
volatile int thorarel3=0 ;

volatile int tMinrel1=0 ;
volatile int tMinrel2=0 ;
volatile int tMinrel3=0 ;

volatile int AcuAno = 0 ;
volatile int AcuMes = 0 ;
volatile int Acudia = 0 ;
volatile int AcuHora= 0 ;

volatile int AcuMin1 = 0 ;
volatile int AcuMin2 = 0 ;
volatile int AcuMin3 = 0 ;

volatile int AcuHora1= 0 ;
volatile int AcuHora2= 0 ;
volatile int AcuHora3= 0 ;

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

volatile int dr1  =0 ;
volatile int dr2  =0 ;
volatile int dr3  =0 ;

volatile int h1  =0 ;
volatile int h2  =0 ;
volatile int h3  =0 ;

volatile int m1  =0 ;
volatile int m2  =0 ;
volatile int m3  =0 ;

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

// const char* ssid1 = "10339240";
// const char* pass1 = "F1B48343C2ED0";
String  EstBoton1 ;
String  EstBoton2 ;
String  EstBoton3 ;
String mensaje=" - Mensaje de inicio - " ;

// No sirve "<meta http-equiv='Refresh' content='1'>"


String PagWeb =
"<!DOCTYPE html>"
"<html lang='es'>"

"<head>"
 "<meta charset='UTF-8'>"
 "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
 "<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
 "<title>Tempo-Apto.</title> "

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
"<fieldset align='center' style='border-style:solid; border-color:#336666; width:460px; height:160px; '"

"<form action ='guardar_horario' method='get'> " ;

String Titulo1 = "<h2>"
"<label>Luz Entrada.......: </label>"  ;

String AlmTitulo1 = "<h2>"
"<br><br>"
"<label>Alarma 1...: </label>"  ;

String PagWebHora1 = "<h2>"
"</form> "
"</h2>";

String Titulo2 = "<h2>"
"<label>Luz Baño..........: </label>"  ;

String AlmTitulo2 = "<h2>"
"<label>Alarma 2...: </label>"  ;


String PagWebHora2 = "<h2>"
"</form> "
"</h2>";

String Titulo3 = "<h2>"
"<label>Otros.............: </label>"    ;

String AlmTitulo3 = "<h2>"
"<label>Alarma 3...: </label>"  ;


String PagWebHora3 = "<h2>"
"</fieldset"
"<br>"
"</center>" 
"</h2>";

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Temporizador Luces</title> "
"<meta charset='UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<meta http-equiv='X-UA-Compatible' content='ie=edge'>"
  "<style type ='text/css'> body,td,th {color:#036;} label1{color: rgb(240,6,6);} label3{color: rgb(233, 82, 157);} label2{color: rgb(250,246,9,0.952);} body{background-color:#a0cbf3;} "
  "</style>"
"</head>"
"<body>"

"<br><br>"
"<form action ='BuscaWifi' method='get' target='pantalla'>"
 "<center>"
   "<fieldset align='center' style='border-style:solid; border-color:#336666; width:400px; height:50px; '"

   "<br>" 
   "<input class='boton'  type='submit' value='ESCANEAR - WIFI'  style='width:70%;height:50px'  />"
   "<iframe id='pantaEsc' name='pantaEsc' src='' width:50px height:50px frameborder='0' scrolling='no' >  </iframe>"

   "</fieldset>" 
 "</center>" 
 "</form>"

"<center>"
"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 430px; height:200px; display: inline-block; margin: 30px; ' > "
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
    "<br><br>" 
    "<input class='boton'  type='submit' value='GUARDAR DATOS' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
    "</fieldset>" 
  "</center>" 
  "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
"</form>" 
"</div>"
"</center>"

"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 430px; height:210px; display: inline-block; margin: 30px; ' > "
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
"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 460px; height:370px; display: inline-block; margin: 30px; ' > "
"<form action ='configHor' method='get' target='pantalla'>"
 "<center>"
   "<h2><label1>- PROGRAMAR HORARIOS -</label1></h2>"    
   "<fieldset align='center' style='border-style:solid; border-color:#336666; width:430px; height:260px; '"
   "<br>"

   "<h2>"
   "<label>Luz Entrada...: </label>" 
   "<input type='number' name='hora1' id='hora1' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px' align='center'   > "
   "<input type='number' name='minu1' id='minu1' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
   "<input type='number' name='dura1' id='dura1' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
   "<br><br>" 
   "</h2>"

   "<h2>"
   "<label>Luz Baño........: </label>"  
   "<input type='number' name='hora2' id='hora2' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> "
   "<input type='number' name='minu2' id='minu2' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
   "<input type='number' name='dura2' id='dura2' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
   "<br><br>" 
   "</h2>"

   "<h2>"
   "<label>Otros..............: </label>"  
   "<input type='number' name='hora3' id='hora3' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> "
   "<input type='number' name='minu3' id='minu3' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
   "<input type='number' name='dura3' id='dura3' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
   "<br>" 
   "</h2>"
     
   "<input class='boton'  type='submit' value='ACTUALIZAR' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
   "<iframe id='pantaHor' name='pantaHor' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
   "<br><br>" 
   "</fieldset>" 
   
 "</center>" 
 "</form>" 
"</div>"
"</center>"
"<br><br>" 
"<center>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
  "<form action ='Prender1' method='get' target='pantalla'>"
     "<h2> <label1> Luz Entrada</label1></h2>"  
     "<input class='boton' type='submit' id='SalaOn' value='ON'  onclick='prueba() ' style='width:50%;height:60px ;background-color:red   ;border-radius:10px'  />"
  "</form>"

  "<form action ='Apagar1' method='get' target='pantalla'>"
     "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px'  />"
  "</form>"
"</div>"
"<br><br>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:380px; height:210px; display: inline-block; margin:1px; ' > "
"<form action ='Prender2' method='get' target='pantalla'>"
  "<h2> <label1>Luz baño</label1></h2>"  
  "<input class='boton'  type='submit' value='ON'  style='width:50%;height:60px ;background-color:rgba(250,246,9,0.952) ;border-radius:10px' />"
"</form>"

"<form action ='Apagar2' method='get' target='pantalla'>"
  "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px '  />"
"</form>"
"</div"
"<br><br>"

"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:368px; height:210px; display: inline-block; margin:1px; ' > "
"<form action ='Prender3' method='get' target='pantalla'>"
   "<h2> <label1>Otros</label1></h2>"  
   "<input class='boton'  type='submit' value='ON'  style='width:50%;height:60px ;background-color:rgba(240,166,166, 0.8) ;border-radius:10px'  />"
"</form>"

"<form action ='Apagar3' method='get' target='pantalla'>"
  "<input class='boton'  type='submit' value='OFF'  style='width:50%;height:60px ;border-radius:10px'  />"
"</form>"
"</div>"
"<br><br>"
"</body>"
;

String pagAlarma = "<!DOCTYPE html>"
"<center>"
"<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:20px; width: 430px; height:350px; display: inline-block; margin:1px; ' > "
"<h3> <label1>- PROGRAMAR ALARMAS -</label1></h3>"   
"<form action ='configAlm' method='get' target='pantalla'>"
 "<center>"

   "<fieldset align='center' style='border-style:solid; border-color:#336666; width:400px; height:270px; '"
   "<br>"

   "<h2>"
   "<label>Alarma 1....: </label>"  
   "<input type='number' name='Almhora1' id='Almhora1' placeholder='Hora'    onfocus='FocoColor(this)' min='0' max='24'  style='width:15%;height:35px' align='center'   > "
   "<input type='number' name='Almminu1' id='Almminu1' placeholder='Minutos' onfocus='FocoColor(this)' min='0' max='59'  style='width:15%;height:35px'> "
   "<br>" 
   "</h2>"

   "<h2>"
   "<label>Alarma 2... : </label>"     
   "<input type='number' name='Almhora2' id='Almhora2' placeholder='Hora'    onfocus='FocoColor(this)' min='0' max='24'  style='width:15%;height:35px'> "
   "<input type='number' name='Almminu2' id='Almminu2' placeholder='Minutos' onfocus='FocoColor(this)' min='0' max='59'  style='width:15%;height:35px'> "
   "<br>" 
   "</h2>"

   "<h2>"
   "<label>Alarma 3....: </label>"  
   "<input type='number' name='Almhora3' id='Almhora3' placeholder='Hora'    onfocus='FocoColor(this)' min='0' max='24'  style='width:15%;height:35px'> "
   "<input type='number' name='Almminu3' id='Almminu3' placeholder='Minutos' onfocus='FocoColor(this)' min='0' max='59'  style='width:15%;height:35px'> "
   "<br>" 
   "</h2>"
     
   "<input class='boton'  type='submit' value='ACTUALIZAR' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'  >"
   "<iframe id='AlmpantaHor' name='AlmpantaHor' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
   "<br>" 
   "</fieldset>" 
   
 "</center>" 
 "</form>" 
"</div>"
"</center>"
;

String paginafin = 
"</html>" ;

String PagWebFin =
"</h2>"
"</form> "
"<center>"
"<a href=\"//192.168.0.12:8080/ProyectoAdin/SHW5b/index.php \"\" width='25%' ><button style='width:95px;height:35px'>Retorno </button></a>"
"</center>"
"</body>"
"</html>";   // el ";" indica el final de la linea
// -------------------- Final Pagina Web --------------------------


void setup()
{
    Serial.begin(115200);
    Serial.print("\n[Parola Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline");
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

    Almh1  = EEPROM.read(111) ; // Hora alarma 1
    Almm1  = EEPROM.read(112) ; // Min. alarma 1   
    Almt1  = EEPROM.read(113) ; // Tiempo alarma 1   
    
    Almh2  = EEPROM.read(114) ; // Hora alarma 2   
    Almm2  = EEPROM.read(115) ; // Min. alarma 2      
    Almt2  = EEPROM.read(116) ; // Tiempo alarma 2       
    
    Almh3  = EEPROM.read(117) ; // Hora alarma 3    
    Almm3  = EEPROM.read(118) ; // Min. alarma 3      
    Almt3  = EEPROM.read(119) ; // Tiempo alarma 3      

    #if USE_UI_CONTROL
       uiDirection.begin();
       uiInvert.begin();
       pinMode(SPEED_IN, INPUT);

       doUI();
    #endif // USE_UI_CONTROL

    P.begin();
    P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
    
    pinMode(ledArbol, OUTPUT)  ;    // D0 - GPIO16 - Luz entrada - Rele
    pinMode(PinLuces, OUTPUT)  ;    // D3 - GPIO0  - Luz baño - Rele 
    pinMode(PinMovto2,INPUT)   ;    // D6 - GPIO12
    
    pinMode(PinOtros,OUTPUT)   ;    // RX - GPIO3 PinOtros
    pinMode(PinAlarma,OUTPUT)  ;    // TX - GPIO1 PinAlarma

    WiFi.softAP("Tempo_Apto1") ;  // Se inicia como Access-Point, se asigna el nombre del WiFi "Tempo_Apto1"
    server.on("/", [](){  // se dispara cuando digitamos 192.168.0.50,ejecuta la pagina.
                          // se debe conectar el "Temporizador", en la lista de Wi-Fi disponibles          
    server.send(200, "text/html", mensaje + PagWeb +
                           "Fecha Actual ....: "+String(anorel)+"/"+ String(mesrel)+"/"+ String(diarel)+ "  --  "+
                           String(hrel1)+":"+ String(mrel1)+
                           Titulo1+String(h1)+":"+String(m1)+" / "+String(dr1)+" Min"+PagWebHora1 + 
                           Titulo2+String(h2)+":"+String(m2)+" / "+String(dr2)+" Min"+PagWebHora2 +
                           Titulo3+String(h3)+":"+String(m3)+" / "+String(dr3)+" Min"+PagWebHora3 +
                           pagHoras + EstBoton1 + EstBoton2+ EstBoton3 +
                           AlmTitulo1+String(Almh1)+":"+String(Almm1)+PagWebHora1 + 
                           AlmTitulo2+String(Almh2)+":"+String(Almm2)+PagWebHora1 + 
                           AlmTitulo3+String(Almh3)+":"+String(Almm3)+PagWebHora1 + pagAlarma +
                           pagina + PagWebFin); }); 

    server.on("/configDate", wifi_Date); 
    server.on("/config"  , wifi_conf); 
    server.on("/configHor", graba_Horas); 
    server.on("/configAlm", graba_Alm);     
    server.on("/BuscaWifi", escanear); 
    server.on("/Prender1", Sala_On); 
    server.on("/Apagar1" , Sala_Of); 
    server.on("/Prender2", Sala_On2); 
    server.on("/Apagar2" , Sala_Of2); 
    server.on("/Prender3", Sala_On3); 
    server.on("/Apagar3" , Sala_Of3); 
    
    server.begin();
    
    Serial.println(" - WebServer Inicializado - ");
    Serial.println(lee(70));
    Serial.println(lee(1));   
    Serial.println(lee(30));
    intento_conexion();
}


void loop()
{
   estadoBoton() ;  
   server.handleClient();
   delay(100);

   //Sensor P.I.R. (U N O )
   if (SwPir1==true){
    leerPir1();
   }

   // Sensor P.I.R. (D O S  )
   if (SwPir2==true){
    leerPir2();
   }

  // Letrero LED 8x8
  ActuFecha_Actual();  
  #if USE_UI_CONTROL
     doUI();
  #endif // USE_UI_CONTROL

  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      //Serial.print("Nuevo - displayAnimate "); 
      //delay(10000) ; // Tiempo en que queda la pantalla en negro
      
     strcpy(curMessage, newMessage);
     newMessageAvailable = false;
    }
    P.displayReset();
  }
  readSerial();
 
  ModoAutomatico() ;  
}

void intento_conexion() {
  String getssid ;   
  String getpass ;   

  if (lee(70).equals("configurado")) {
    mensaje = 
    "<center>"
    "<h2>"
    "<label1>"
    "- CONFIGURADO - Ok  " 
    "</label1>"
    "</h2>"
    "</center>" ;
    server.send(200, "text/html", pagina + mensaje + paginafin); 
    
    ssid_leido=lee(1) ;
    pass_leido=lee(30);

    Serial.println(ssid_leido);
    Serial.println(pass_leido);

    ssid_tamano=ssid_leido.length()+1 ; // calcula la cantidad de caracteres que tiene ssid y clave
    pass_tamano=pass_leido.length()+1 ;

    ssid_leido.toCharArray(ssid,ssid_tamano) ;
    pass_leido.toCharArray(pass,pass_tamano) ;

    IPAddress ip(192,168,0,60);
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    WiFi.config(ip,gateway,subnet) ;

    int cuenta= 0 ;
    WiFi.begin(ssid, pass) ;  // Intenta conectar en modo estacion (STA)

    // Averigua cual es el status del WiFi
    // Trata de concetarse a la Red Wifi existente, en el hogar.
    while (WiFi.status() != WL_CONNECTED ) {
      delay(500);
      cuenta++ ;
      Serial.print(".");
      if (cuenta>20){
        Serial.println(" - Fallo de conexion - ");
        graba(70,"noconfigurado") ;
        server.send(200, "text/html",String("<h2>La conexion no se pudo realizar </h2>"));      
        return ;
      }  
    }
    Serial.print(WiFi.localIP());
    strUrl = (WiFi.localIP()) ;
    graba(70,"configurado") ;   
    graba(1,getssid) ;
    graba(30,getpass) ;
    server.send(200, "text/html",String("<h2>Conexion Exitosa</h2>" + getssid + "Url es...:" + strUrl ));   
    // server.send(200, "text/html",String("<h2>Conexion Exitosa</h2>" + getssid + "Url es...:" + WiFi.localIP())  );   
    }
  else 
    {
     mensaje=
     "<center>"
     "<h2>"
     " " 
     "</h2>"
     "</center>";
     // "El Temporizador No está - CONFIGURADO - " 
    }
}


void wifi_conf() {
  int cuenta=0;

  String getssid  = server.arg("ssid") ;  // recibe los valores de la Web
  String getpass  = server.arg("pass") ;

   
     //getssid=arregla_simbolos(getssid) ;
     //getpass=arregla_simbolos(getpass) ;  
  
     ssid_tamano = getssid.length() + 1 ;
     pass_tamano = getpass.length() + 1 ;

     getssid.toCharArray(ssid,ssid_tamano) ;
     getpass.toCharArray(pass,pass_tamano) ;

     Serial.println(ssid);  
     Serial.println(pass);  

     WiFi.begin(ssid, pass) ;
     while (WiFi.status() != WL_CONNECTED ) {
       delay(500);
       Serial.print(".");
       cuenta++ ;

       if (cuenta>20) {
         graba(70, "noconfigurado") ;
         server.send(200, "text/html",String("<h2>La Conexion no pudo realizarse </h2>" ));   
         return ;
       }
     }
     Serial.print(WiFi.localIP());  
     graba(70,"configurado") ;
     graba(1,getssid) ;
     graba(30,getpass) ;
       server.send(200, "text/html",String("Conexion exitosa1a..: " + WiFi.localIP() ));
     //    server.send(200, "text/html",String("<h2>Conexion exitosa a..: " + getssid + " La URL es ..:" + WiFi.localIP() + "</h2>" ));  
}

void graba_Horas() {
  String horaEnt1 = (server.arg("hora1"));
  String minuEnt1 = (server.arg("minu1"));
  String duraEnt1 = (server.arg("dura1"));
  
  String horaEnt2 = (server.arg("hora2"));
  String minuEnt2 = (server.arg("minu2"));  
  String duraEnt2 = (server.arg("dura2"));  
  
  String horaEnt3 = (server.arg("hora3"));
  String minuEnt3 = (server.arg("minu3"));
  String duraEnt3 = (server.arg("dura3"));
  
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


void config(){
//   Serial.println(server.arg("ssid"));  // Recibimos los valores que envia el GET, del formulario Web
//   graba(0,server.arg("ssid")) ;
//   Serial.println(server.arg("pass"));
//   graba(30,server.arg("pass")) ;
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
   digitalWrite(ledArbol, LOW); // Apaga D0 - GPIO16 - Luz entrada - Rele
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
  int EstArbol =  digitalRead(ledArbol) ;
  if (EstArbol==LOW)
    {
     digitalWrite(ledArbol, HIGH); // Prende D0 - GPIO16 - Luz entrada - Rele
     Boton1 = 1 ;
     zumbido1();
     SwPir1   = false ;
    }
}

void Sala_Of(){
  int EstArbol =  digitalRead(ledArbol) ;
  if (EstArbol==HIGH)
    {
     digitalWrite(ledArbol, LOW); // Apaga D0 - GPIO16 - Luz entrada - Rele
     Boton1 = 0 ;
     zumbido1();
     SwPir1   = true ;     
    }
}

void Sala_On2() { 
  int EstPinLuces =  digitalRead(PinLuces) ;
  if (EstPinLuces==LOW)
    {
     digitalWrite(PinLuces,  HIGH) ; // Prende D3 - GPIO0  Luz baño - Rele 
     Boton2 = 1 ;
     zumbido1();
     SwPir2   = false ;     
    }
}

void Sala_Of2(){
  int EstPinLuces =  digitalRead(PinLuces) ;
  if (EstPinLuces==HIGH)
    {
     digitalWrite(PinLuces, LOW)  ; // Apaga D3 - GPIO0  Luz baño - Rele 
     Boton2 = 0 ;  
     zumbido1();
     SwPir2   = true ;     
    }
}

void Sala_On3(){ 
  int EstPinLava =  digitalRead(PinOtros) ;
  if (EstPinLava==LOW)
    {
     digitalWrite(PinOtros,HIGH) ; // Prende lavandera 
     swFigPrender = true ;
     Boton3 = 1 ;
     zumbido1();
    }
}

void Sala_Of3(){
  int EstPinLava =  digitalRead(PinOtros) ;
  if (EstPinLava==HIGH)
    {
     digitalWrite(PinOtros,LOW); // Apaga lavandera 
     swFigPrender = false ;
     Boton3 = 0 ;
     zumbido1();
    }
}

void ModoAutomatico() {
   DateTime now = rtc.now();
   if (now.hour()==h1 && now.minute()==m1 && dr1 !=0 ) {
      digitalWrite(ledArbol, HIGH); // Prende D0 - GPIO16 - Luz entrada - Rele
      Boton1 = 1 ;
      aviso1() ;
     
      minumov1 = m1 ;
      horamov1 = h1 ; 
      swCalcuMin1 = true ;
      AcuHora1    = 0 ;
      AcuMin1     = 0 ;

      hrelant1 = h1 ;
      mrelant1 = m1 ;
      SwPir1   = false ;
   }

   if (swCalcuMin1==true ) {
      totalMinutos1() ;

      if (AcuMin1>=dr1 )
        {
         digitalWrite(ledArbol, LOW); // Apaga D0 - GPIO16 - Luz entrada - Rele
         Boton1 = 0 ;
         aviso() ;
         swSonido1 = true ;
         swCalcuMin1 = false ;
         SwPir1 = true ;
        }
   } //// Fin horario uno 
        
   //// ------------------------------------------------------------- Inicio horario dos 
   if (now.hour()==h2 && now.minute()==m2 && dr2 !=0) {
      aviso2() ;
      delay(2000) ;
      digitalWrite(PinLuces,  HIGH) ; // Prende D3 - GPIO0  Luz baño - Rele 
      Boton2 = 1 ;
           
      minumov2 = m2 ;
      horamov2 = h2 ; 
      swCalcuMin2 = true ;
      AcuHora2    = 0 ;
      AcuMin2     = 0 ;

      hrelant2 = h2 ;
      mrelant2 = m2 ;
      SwPir2   = false ;
   }

   if (swCalcuMin2==true ) {
      totalMinutos2() ;
      if (AcuMin2>=dr2)
        {
         digitalWrite(PinLuces,  LOW) ; // Apaga D3 - GPIO0  Luz baño - Rele 
         Boton2 = 0 ; 
         aviso() ;       
         swCalcuMin2 = false ;
         SwPir2 = true ;
        }
   }   //// Fin horario dos
     

   //// ------------------------------------------------------Inicio horario tres      
   if (now.hour()==h3 && now.minute()==m3 && dr3 !=0) {
      digitalWrite(PinOtros,HIGH) ; // Prende lavandera 
      Boton3 = 1 ;
      aviso3() ;
          
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
         digitalWrite(PinOtros,LOW) ; // Apaga lavandera 
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

   //// ------------------------------------------------------------- Inicio alarma uno 
   if (now.hour()==Almh1 && now.minute()==Almm1 && Almh1!=0)
     {
      if (SwAlarma1==true )
        {
         zumbido() ;
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
         zumbido() ;
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
         zumbido() ;
         SwAlarma3=false ; 
        }
      }
   else 
      {
       SwAlarma3=true ;
      }

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

//   Serial.print(" Hora Inicial.. : ");
//   Serial.print(horamov1);
//   Serial.print("/");
//   Serial.print(minumov1);
//   Serial.print(" Duracion.. : ");
//   Serial.print(dr1);
// 
//   Serial.print(" Hora Actual.. : ");
//   Serial.print(hrel1);
//   Serial.print("/");   
//   Serial.print(mrel1);
//
//   Serial.print(" -- Total Minutos ...: ");
//   Serial.println(AcuMin1);
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

void aviso() {
  zumbido1() ;
}


void aviso1() {
  if (swSonido1 == true)
    {
     zumbido() ; 
     swSonido1 = false ;
    }
}

void aviso2() {
  if (swSonido2 == true)
    {
     zumbido() ;  
     swSonido2 = false ;
    }
}

void aviso3() {
  if (swSonido3 == true)
    {
     zumbido() ; 
     swSonido3 = false ;
    }
}

void alarma1() {
     int EstAlarma =  digitalRead(PinAlarma) ;
     if (EstAlarma==LOW)
       {
        digitalWrite(PinAlarma,HIGH)    ; // Prende PinAlarma
        delay(600) ;

        digitalWrite(PinAlarma,LOW)     ; // Apaga PinAlarma
        delay(59400) ;
       }
}

void melodia1() {
  for(int Almi=100 ; Almi<1500 ; Almi++){ // loop for to increase the sound frequency
    tone(PinAlarma, Almi);
    delay(10); //You can change the delay if you want to make it longer or shorter
  }
  
 for(int Almi=1500 ; Almi>100 ; Almi--){
   tone(PinAlarma, Almi);
   delay(10);
 }
 noTone(PinAlarma);
}

void zumbido() {
  for (int durat1=2000; durat1<3500; durat1=durat1+100 ) {
     tone(PinAlarma, durat1); 
     delay(150);
  }

  for (int durat1=3500; durat1>2000; durat1=durat1-100 ) {
     tone(PinAlarma, durat1); 
     delay(150);
  }
  noTone(PinAlarma);
}

void zumbido1() {
  tone(PinAlarma, 1800); // tone() is the main function to use with a buzzer, it takes 2 or 3 parameteres (buzzer pin, sound frequency, duration)
  delay(1000);
  
  tone(PinAlarma, 2000); // You can also use noTone() to stop the sound it takes 1 parametere which is the buzzer pin
  delay(1000);
  noTone(PinAlarma);
}

void zumbido2() {
  tone(PinAlarma, 1500); // You can also use noTone() to stop the sound it takes 1 parametere which is the buzzer pin
  delay(200);
  noTone(PinAlarma);
  
  // Sonido Bajo
  // tone(PinAlarma, 81, 1000); // You can also use noTone() to stop the sound it takes 1 parametere which is the buzzer pin
  // delay(1300);
}

void pulso() {
  for (dura1=0; dura1<5; dura1++ )
   {
    tone(PinAlarma, 15660, 250);
    delay(3000) ;
   }
   noTone(PinAlarma);
}

void Play_Pirates()
{ 
  for (int thisNote = 0; thisNote < 24; thisNote++) {

    int noteDuration = 1000 / Pirates_duration1[thisNote];//convert duration to time delay
    tone(PinAlarma, Pirates_note1[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower
    delay(pauseBetweenNotes);
    noTone(PinAlarma); //stop music on pin 8 
    }
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


void estadoBoton(){
    if (swIniRel1 == true)
    {
     digitalWrite(ledArbol, HIGH); // Apaga Rele-"dispositivo Uno" por primera vez
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

}

void leerPir1(){
   Sw_Movto1 = digitalRead(PinMovto1) ; // D4 - GPIO2  - P.I.R. - PinMovto1
   //Sw_Movto1 = 0 ;  //         P R O V I S I O N A L 
   Serial.print("El movimiento es ..: ");
   Serial.print(Sw_Movto1);  

   if (Sw_Movto1==1)
     {
       Serial.println(" Con movimiento  ");
       digitalWrite(ledArbol, HIGH); // Prende D0 - GPIO16 - Luz entrada - Rele
       swMovimiento=true ;
     }
   else
     { 
      if (swMovimiento==true)
        {
         swMovimiento=false ;
         Serial.println(" Sin movimiento  ");
         digitalWrite(ledArbol, LOW); // Apaga D0 - GPIO16 - Luz entrada - Rele 
        }
     }
}

void leerPir2(){
   Sw_Movto2 = digitalRead(PinMovto2) ; // D6 - GPIO12 P.I.R. - PinMovto2
   //Sw_Movto2 = 0 ;  //         P R O V I S I O N A L 
   Serial.print("El movimiento 2 es ..: ");
   Serial.print(Sw_Movto2);  

   if (Sw_Movto2==1)
     {
       Serial.println(" Con movimiento 2  ");
       digitalWrite(PinLuces, HIGH); // Prende D6 - GPIO0  Luz baño - Rele  
       swMovimiento2=true ;
     }
   else
     { 
      if (swMovimiento2==true)
        {
         swMovimiento2=false ;
         Serial.println(" Sin movimiento 2  ");
         digitalWrite(PinLuces, LOW); // Apaga D6 - GPIO0  Luz baño - Rele 
        }
     }
}

void ActuFecha_Actual(){
   DateTime now = rtc.now();
   anorel = now.year()  ;
   mesrel = now.month() ;
   diarel = now.day()   ;
   hrel1  = now.hour()  ;
   //hrel1  = 19 ;

   // Despues de las 20 horas no muestra la fecha, solo la hora
   if (hrel1>=20 || (hrel1>=1 && hrel1<7))
     { SwSinMov = true  ; }
   else
     { SwSinMov = false ; }

   // Definir horarios solo nocturno para los dos P.I.R.
   if (hrel1>=6 && hrel1<17)
     { SwPir1=false ;
       SwPir2=false ;}
   else
     { SwPir2=true ;}
     
   if (hrel1>12){hrel1=hrel1-12;}
   if (hrel1<=9) 
     { Horastr = "0" + String(hrel1);}
   else
     { Horastr =       String(hrel1);}  

   mrel1  = now.minute();
   //mrel1= 51 ;
   if (mrel1<=9) 
     {Minutostr = "0" + String(mrel1) ;}
   else
     {Minutostr =       String(mrel1) ;}  

   anorel = anorel - 2000 ;
   switch(mesrel){
     case 1:
       mesStr="Ene" ;
       break;
     case 2:
       mesStr="Feb" ;
       break;
     case 3:
       mesStr="Mar" ;
       break;
     case 4:
       mesStr="Abr" ;
       break;
     case 5:
       mesStr="May" ;
       break;
     case 6:
       mesStr="Jun" ;
       break;
     case 7:
       mesStr="Jul" ;
       break;
     case 8:
       mesStr="Ago" ;
       break;
     case 9:
       mesStr="Sep" ;
       break;
     case 10:
       mesStr="Oct" ;
       break;
     case 11:
       mesStr="Nov" ;
       break;
     case 12:
       mesStr="Dic" ;
       break;
   }

  if (SwSinMov== false)
    {Fe_Actual = String(diarel)+"/"+mesStr+"/"+String(anorel)+"  "+Horastr+":"+Minutostr+" " ; 
     textEffect_t scrollEffect = PA_SCROLL_LEFT;}
  else
    {Fe_Actual = " "+Horastr+":"+Minutostr+" " ;
     //textEffect_t scrollEffect  = PA_OPENING; }
     textEffect_t scrollEffect  = PA_FADE;}
  
  
  if (hrel1==1) {Fe_Actual = Fe_Actual+" "; }
  if (hrel1== 6 && mrel1==  1){Fe_Actual = Fe_Actual+" "; }    
  if (hrel1== 6 && mrel1== 11){Fe_Actual = Fe_Actual+" "; }    

  if (hrel1==10 && mrel1==  1){Fe_Actual = Fe_Actual+" "; }
  if (hrel1==10 && (mrel1>=10 && mrel1<=19)){Fe_Actual = Fe_Actual+" "; }  
  
  if (hrel1==11 && mrel1==  1){Fe_Actual = Fe_Actual+" "; }
  if (hrel1==11 && mrel1== 11){Fe_Actual = Fe_Actual+" "; }
  if (hrel1==11 && mrel1== 41){Fe_Actual = Fe_Actual+" "; }  
  if (hrel1==11 && mrel1== 51){Fe_Actual = Fe_Actual+" "; }    
  if (hrel1==11 && (mrel1>=10 && mrel1<=19)){Fe_Actual = Fe_Actual+" "; }  
  
  if (hrel1==12 && mrel1==  1){Fe_Actual = Fe_Actual+" "; }
  if (hrel1==12 && (mrel1>=10 && mrel1<=19)){Fe_Actual = Fe_Actual+" "; }  
    
  int len_Fe_Actual = Fe_Actual.length()+1 ;
  char array_Fe_Actual[len_Fe_Actual] ;
  
  Fe_Actual.toCharArray(array_Fe_Actual,len_Fe_Actual) ;
  Fe_Char = array_Fe_Actual ;

  strncpy(newMessage, array_Fe_Actual, sizeof(newMessage));
  Serial.print("Fe-newMessage..: "); 
  Serial.println(newMessage); 
  
  Fe_Actual ="";
  newMessageAvailable = true;
}
