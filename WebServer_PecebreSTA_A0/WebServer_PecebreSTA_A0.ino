// W E B - S E R V E R 
// Crea un SERVIDOR y el mismo le asigna una IP - 192.168.0.50 
// Para ingresar con esta IP: 192.168.4.1  se  debe entrar a los Wifi y concetar el "Tempo_Gral", esta es necesaria cuando 
// se va a guardar por primera vez el Usuario y la contraseña de la red donde nos queremos conectar.
// Para ingresar con esta IP: 192.168.0.50 se  debe entrar a los dispositovos  Wifi y concetar el "10339240"
// No necesita el servidor apache
// Graba los datos con el Wifi al que se conecte
// El ESP8266 puede asignar la Ip automaticamente o la que determine el usuario
// Se ejecuta sin el "ADIN" o con El.
// Al llamarlo desde el "ADIN", se hace desde sus opciones aplicaciones como "Temporizador". pero para ello el "XAMPP", debe estar activo.
// Se puede ejecutar sin el "XAMPP"
// Se ejecuta en el navegador con 192,168,0,50/Sala_On 
// Graba la Clave y Wifi en memoria RAM
// Utiliza servo-motor en los dispositivos


// Inicio de la configuracion del Modulo Esp8266
// 1. Carge el programa
// 2. Busque dentro de la lista de las Redes de Internet del Computador, Tablet o Celular la llamada "Tempo_Gral"
// 3. Seleccionela y conectela "Tempo_Gral"
// 4. Ingrese al navegador y digite la URL (192.168.4.1)
// 5. Al ingresar a la APP, busque el boton "ESCANEAR - WIFI"
// 6. Refresque la pantalla
// 7. Digite el Usuario y la clave de acceso de su Red y presione "GUARDAR DATOS"
// 8. En el COM9 aparece la direccion Ip en la cual se debe conectar para manejar la APP

// Para poder trabajar con el PC o subir el codigo, se debe desconectar los negativos comunes 
// y el rx/tx, de lo contrario bloquea el ESP8266.
// El voltaje de los dispositivos es de 5.0 v.

// Feb/01/2021 // Se instala la opcion de "Aviso"
// Abr/14/2021 // Se instala el segundo P.I.R.
// May/20/2021 // Se instala el tercer  P.I.R. 
//             // Se instala el boton cuatro  
// Oct/19/2021 // Se condiciono el tercer sensor P.I.R., para prender solo despues de las 4:00 pm.
// Dic/14/2021 // Se cambio para que con la alarma no prenda ni apague la cascada y pescebre
// Dic/17/2021 // Se desactiva el sensor de alarma cuando se prende la cascada y el pesebre
// 01/Feb/2024 -  Se cambio el reloj de 3231 por el 1307
  

/* Para generar y guardaR la clave y el nombre del WiFi, siga los siguientes pasos :
   a.- En la tablet o Web busque el "Temporizador", dentro de la lista de dispositivos Wi-Fi disponibles 
   b.- Ingrese a el dispositivo y "conectelo". 
   c.- Ingrese al navegadory digite la URL : 192.168.4.1
   d.- El sistema le indicara que no esta "CONFIGURADO".
   e.- Desplace la pantalla hacia arriba, hasta llegr a "Usuario" y "Clave".
   f.- Digite la informacion correspondiente en los campos y presione la tecla "GUARDAR DATOS".
*/

/* Para cargar el codigo se debe desconectar al negativo que va desde el modulo a la entrada de 5v.*/

// Wire - permite ejecurar la comunicacion I2C

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>

//const char* ssidhto     = "Sistema Adin 2G";
//const char* passwordhto = "Leal083096";


Servo myservo1;  // Pastor 
Servo myservo2;  // Lavandera

boolean Estado = false;
boolean Arcoiris = false;
String Valor = "";

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
const int PinMovto    =  2 ;            // D4 - GPIO2  P.I.R. Uno  - Pecebre  
const int PinCascada  = 14 ;            // D5 - GPIO14 Cascada    
const int PinMovto3   = 13 ;            // D7 - GPIO13 P.I.R. Tres - Luz entrada
const int PinMovto2   = 15 ;            // D8 - GPIO15 P.I.R. Dos  - Luz sala
const int PinFiguras  = 12 ;            // D6 - GPIO12 Figuras
const int PinLuzEnt   =  3 ;            // RX - GPIO3  Luz entrada
const int PinAlarma   =  1 ;            // TX - GPIO1  PinAlarma  

//int PinPastor3  =  15 ;  

// RTC_DS3231 RTC;
RTC_DS1307 RTC;

// WiFiClient espClient ;
ESP8266WebServer server(80);


String strUrl ;
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
String mensaje=" - Mensaje de inicio - " ;

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

    ".pollo {position: relative; display: inline-block; width: 150px; height: 42px; background-color: #2196f3; border-radius: 34px; } "   
    ".vivo .pollo { background-color: #2196f3; }"
    ".pollo:hover { }"
    ".huevo {position: absolute; content: ""; height: 38px; width: 60px; left: 1px; bottom: 2px; border-radius: 34px; background-color: white; } "
    ".vivo .huevo {background-color: rgb(245, 182, 8); color:rgb(189, 7, 7); left: 88px; } "
    
  "</style>"
"</head>"
"<body>"

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





"<center>"
"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:420px; height:210px; display: inline-block; margin:1px; ' > "

  "<center>"
    
    "<h3>"
    "<label1><b>- Boton sala  -</label1>"  
    "<br><br>"
    "</h3>"

      "<h3>Arcoiris</h3>"
      "   <div id='cambiarArcoiris' class='vivo'>"
      "     <span class='pollo'>"
      "       <span class='huevo'></span>"
      "     </span>"
      "   </div>"




   
    "<input class='boton'  type='submit' value='GUARDAR DATOS' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >"
    
  "</center>" 
  "<iframe id='pantalla' name='pantalla' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>"
 
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
  "<div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 460px; height:390px; display: inline-block; margin: 30px; ' > "
    "<form action ='configHor' method='get' target='pantalla'>"
       "<center>"
        "<h2><label1>- PROGRAMAR HORARIOS -</label1></h2>"    
        "<fieldset align='center' style='border-style:solid; border-color:#336666; width:430px; height:280px; '"
         "<br>"

         "<h2>"
          "<label>Luz sala.........: </label>"  
          "<input type='number' name='hora1' id='hora1' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px' align='center'> "
          "<input type='number' name='minu1' id='minu1' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
          "<input type='number' name='dura1' id='dura1' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
          "<br>" 
         "</h2>"

         "<h2>"
          "<label>Cascada/Luces</label>"     
          "<input type='number' name='hora2' id='hora2' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> "
          "<input type='number' name='minu2' id='minu2' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
          "<input type='number' name='dura2' id='dura2' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
          "<br>" 
         "</h2>"

         "<h2>"
           "<label>Figuras..........: </label>"  
           "<input type='number' name='hora3' id='hora3' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> "
           "<input type='number' name='minu3' id='minu3' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
           "<input type='number' name='dura3' id='dura3' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
           "<br>" 
         "</h2>"

         "<h2>"
           "<label>Luz entrada..: </label>"  
           "<input type='number' name='hora4' id='hora4' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> "
           "<input type='number' name='minu4' id='minu4' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> "
           "<input type='number' name='dura4' id='dura4' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> "
           "<br>" 
         "</h2>"

         "<br>"      
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

String pagAlarma = "<!DOCTYPE html>"
"<center>"
"<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:420px; height:315px; display: inline-block; margin:1px; ' > "
"<h3> <label1>- PROGRAMAR ALARMAS -</label1></h3>"   
"<form action ='configAlm' method='get' target='pantalla'>"
 "<center>"
  "<fieldset align='center' style='border-style:solid; border-color:#336666; width:330px; height:205px; '"
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
 "<br><br><br><br>"
"</div>"
"</center>"
;  // fin String pagAlarma

String paginafin = 
"</html>" ;

String PagWebFin =
"</h2>"
"</form> "
"<br><br>"
  
  "<center>"
    "<a href=\"//192.168.0.12:8080/ProyectoAdin/SHW5b/index.php \"\" width='25%' ><button style='width:95px;height:35px'>Retorno </button></a>"
  "</center>"
"</body>"
"</html>";   // el ";" indica el final de la linea
// -------------------- Final Pagina Web --------------------------


void setup()
{
    Serial.begin(115200);
    // Serial.println("\n");

    EEPROM.begin(4096);

    /// Reloj  
    if (!RTC.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
    }
 
    // Si se ha perdido la corriente, fijar fecha y hora
    // No funciona el "RTC.lostPower()"
    // if (RTC.lostPower()) {
      // Fijar a fecha y hora de compilacion
      // RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // RTC.adjust(DateTime(2019, 1, 21, 3, 0, 0));
    // }
   
    /// Reloj  
    //  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Wire.begin();     // D1-Clock / D2-Data  
    DateTime now = RTC.now();
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
    pinMode(PinAlarma,  OUTPUT)  ;    // TX - GPIO1 PinAlarma

    WiFi.softAP("Tempo_Gral") ;  // Se inicia como Access-Point, se asigna el nombre del WiFi "Tempo_Gral"
    server.on("/", [](){         // se dispara cuando digitamos 192.168.0.50,ejecuta la pagina.
                          // se debe conectar el "Temporizador", en la lista de Wi-Fi disponibles          
    server.send(200, "text/html", mensaje + PagWeb +
                           FeActual+String(anorel)+"/"+ String(mesrel)+"/"+ String(diarel)+ "  --  "+
                           String(hrel1)+":"+ String(mrel1)+
                           Titulo1+String(h1)+":"+String(m1)+" / "+String(dr1)+" Min"+PagWebHora1 + 
                           Titulo2+String(h2)+":"+String(m2)+" / "+String(dr2)+" Min"+PagWebHora2 +
                           Titulo3+String(h3)+":"+String(m3)+" / "+String(dr3)+" Min"+PagWebHora3 +
                           Titulo4+String(h4)+":"+String(m4)+" / "+String(dr4)+" Min"+PagWebHora4 +
                           pagHoras + EstBoton1+ EstBoton2+ EstBoton3+  EstBoton4 +
                           
                           AlmTitulo1+String(Almh1)+":"+String(Almm1)+
                           " / "+String(Almh2)+":"+String(Almm2)+ 
                           " / "+String(Almh3)+":"+String(Almm3)+PagWebHora1 + pagAlarma +
                           pagina + PagWebFin); }); 

    server.on("/configDate"  , wifi_Date); 
    server.on("/configWifi"  , wifi_conf); 
    server.on("/configHor", graba_Horas); 
    server.on("/configAlm", graba_Alm);     
    // server.on("/BuscaWifi", escanear); 
    
    server.on("/botonSala", salaOnOff); 
    server.on("/Prender1", Sala_On); 
    server.on("/Apagar1" , Sala_Of); 
    server.on("/Prender2", Sala_On2); 
    server.on("/Apagar2" , Sala_Of2); 
    server.on("/Prender3", Sala_On3); 
    server.on("/Apagar3" , Sala_Of3); 
    server.on("/Prender4", Sala_On4); 
    server.on("/Apagar4" , Sala_Of4); 
    
    server.begin();
    
    Serial.println(" - WebServer Inicializado - ");
    Serial.println(lee(70));
    Serial.println(lee(1));   
    Serial.println(lee(30));
    intento_conexion();
}


void loop()
{
   Serial.print("Hola ESP8266");
     //  Serial.print(WiFi.localIP());
   estadoBoton() ;  
   server.handleClient();
   delay(100);
   
   // Lee la fecha y hora actual 
   DateTime now = RTC.now();   
   anorel = now.year()  ;
   mesrel = now.month() ;
   diarel = now.day()   ;
   hrel1  = now.hour()  ;
   mrel1  = now.minute();

   // hoy31Ene
   // Pescebre y Cascada
   if (SwPir1==true){
    leerPir1();
   }

   // Luz sala
   if (SwPir2==true){
    leerPir2();
   }
   
   ModoAutomatico() ;  

   // if (swFigPrender == true){
   //   Servo1Levanta() ;    
   // }
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

    IPAddress ip(192,168,0,51);
    IPAddress gateway(192,168,0,1);
    IPAddress subnet(255,255,255,0);
    WiFi.config(ip,gateway,subnet) ;

    int cuenta= 0 ;
    WiFi.begin(ssid, pass) ;  // Intenta conectar en modo estacion (STA)

    //hoy1509
    //WiFi.begin(ssidhto, passwordhto) ;  // Intenta conectar en modo estacion (STA)

    
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
    //strUrl = (WiFi.localIP()) ;
    graba(70,"configurado") ;   
    graba(1,getssid) ;
    graba(30,getpass) ;
    //server.send(200, "text/html",String("<h2>Conexion Exitosa</h2>" + getssid + "Url es...:" + strUrl ));   
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
     //server.send(200, "text/html",String("Conexion exitosa1a..: " + WiFi.localIP() ));
     //server.send(200, "text/html",String("<h2>Conexion exitosa a..: " + getssid + " La URL es ..:" + WiFi.localIP() + "</h2>" ));  
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

  String horaEnt4 = (server.arg("hora4"));
  String minuEnt4 = (server.arg("minu4"));
  String duraEnt4 = (server.arg("dura4"));  
  
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

      // hoy20May
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
  int EstLuzSala =  digitalRead(PinLuzSala) ;
  if (EstLuzSala==HIGH)
    {
     digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno
     SwPir2 = false ;
     Boton1 = 1 ;
     aviso() ;      
    }
}


void salaOnOff() { 
    if (server.hasArg("estado")) {
      Valor = server.arg("estado");
    if (Valor == "vivo") {
      Arcoiris = true;
    } else {
      Arcoiris = false;
    }

    //Serial << "Estado Arcoiris " << Arcoiris << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando Arcoiris");
  
  
  
  //int EstLuzSala =  digitalRead(PinLuzSala) ;
  //if (EstLuzSala==HIGH)
  //  {
  //   digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno
  //   SwPir2 = false ;
  //   Boton1 = 1 ;
  //   aviso() ;      
  //  }
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


// ----------- Prueba
//  int EstLuzSala =  digitalRead(PinLuzSala) ;
//  if (EstLuzSala==HIGH)
//    {
//     digitalWrite(PinLuzSala, LOW); // Prende dispositivo Uno
//     SwPir2 = false ;
//     Boton1 = 1 ;
//     aviso() ;      
//    }
// ------------- Fin 



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
   DateTime now = RTC.now();

   //// ------------------------------------------------------------- Inicio horario uno
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

   // hoy31ene
   // hrel1 = 19 ;  // ---- P R O V I S I O N A L 
   // ------------------------------------------------------ Inicio horario cuatro - Luz entrada
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
   else
     {
       
      // Calculo de tiempo de duracion
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
      }  // Fin calculo de tiempo de duracion


      // hoy31ene
      // No esta programado 
      // Definir horario solo nocturno para el P.I.R., Luz de entrada
      if (hrel1>=6 && hrel1<18)
        { SwPir3=false  ;}
      else
        {
         SwPir3=true   ;
        }
 
      if (SwPir3==true){
        leerPir3();
      }
     } // Fin else
   //// Fin horario cuatro


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
  DateTime now1 = RTC.now();
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
  DateTime now1 = RTC.now();
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
  DateTime now1 = RTC.now();
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
  DateTime now1 = RTC.now();
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

void wifi_Date(){
  String diaDateEnt  = server.arg("diaDate") ; 
  String mesDateEnt  = server.arg("mesDate") ; 
  String anoDateEnt  = server.arg("anoDate") ; 
  String horaDateEnt = server.arg("horaDate") ; 
  String minDateEnt  = server.arg("minDate") ; 

  if (horaDateEnt != "")
    {
     int hrelx = horaDateEnt.toInt()   ; 
     DateTime nowx = RTC.now();
     int mrelx = nowx.minute() ;
     RTC.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }

  if (minDateEnt != "")
    {
     DateTime nowx = RTC.now(); 
     int hrelx = nowx.hour() ; 
     int mrelx = minDateEnt.toInt()  ;
     RTC.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }
    
  if (diaDateEnt != "")
    {
     DateTime nowx = RTC.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();  
     int diarel = diaDateEnt.toInt() ;
     RTC.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    }

  if (mesDateEnt != "")
    {
     mesrel = mesDateEnt.toInt() ;
     DateTime nowx = RTC.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();    
     RTC.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));     
    }  

  if (anoDateEnt != "")
    {
     anorel = anoDateEnt.toInt() ; 
     DateTime nowx = RTC.now();
     int hrelx = nowx.hour()  ;
     int mrelx = nowx.minute();    
     RTC.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));          
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



void cambiarArcoiris(){
//        const elementoEstado = document.getElementById("botonArcoiris");
//
//        if(elementoEstado.className == "vivo"){
//          elementoEstado.className = "muerto";
//        } else{
//          elementoEstado.className = "vivo";
//        }
//        console.log("Cambiando Arcoiris a " + elementoEstado.className)
//        consultaGET("http://%ip/arcoiris?estado=" + elementoEstado.className);
}
