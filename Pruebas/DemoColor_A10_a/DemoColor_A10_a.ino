
// Creado ChepeCarlos
// Tutorial Completo en https://nocheprogramacion.com
// Canal Youtube https://youtube.com/alswnet?sub_confirmation=1

template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#if defined(ESP32)
   #include <WiFi.h>
   #include <ESPmDNS.h>
   #include <WebServer.h>
   // #include <WiFiMulti.h> // "Se elimina por trabajar una red Estatica"
   // WiFiMulti wifiMulti;   // "Se elimina por trabajar una red Estatica"
   WebServer server(80);

#elif defined(ESP8266)
   #include <ESP8266WiFi.h>
   #include <ESP8266mDNS.h>
   #include <ESP8266WebServer.h>

   // #include <ESP8266WiFiMulti.h> // "Se elimina por trabajar una red Estatica"
   // ESP8266WiFiMulti wifiMulti;   // "Se elimina por trabajar una red Estatica"
   ESP8266WebServer server(80);

#endif

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#include "data.h"

int pinLed = 2;
#define Pin 4
#define CantidadLed 50
Adafruit_NeoPixel tira = Adafruit_NeoPixel(CantidadLed, Pin, NEO_GRB + NEO_KHZ800);


volatile int anorel=2023 ;
volatile int mesrel=1 ;
volatile int diarel=24;
volatile int hrel1 =17;
volatile int mrel1 =57;



const uint32_t TiempoEsperaWifi = 5000;

unsigned long TiempoActual   = 0 ;
unsigned long TiempoAnterior = 0 ;
const long    TiempoCancelacion = 500;

boolean Estado = false;
boolean Arcoiris = false;
int Nivel = 50;
int Rojo = 255;
int Verde = 0;
int Azul = 0;

long HueActual = 0;
String Valor = "";

volatile int mod1=0 ;
volatile int hora1 ;
volatile float pes1=0 ;

volatile int h1  =0 ;
volatile int h2  =0 ;
volatile int h3  =0 ;
volatile int h4  =0 ;

volatile int m1  =0 ;
volatile int m2  =0 ;
volatile int m3  =0 ;
volatile int m4  =0 ;

volatile int dr1  =0 ;
volatile int dr2  =0 ;
volatile int dr3  =0 ;
volatile int dr4  =0 ;

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

IPAddress ip_local(192,168,10,70);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

void mensajeBase() {
   server.sendHeader("Access-Control-Allow-Origin", "*");
   server.send(200, "text/html", 
                           FeActual+String(anorel)+"/"+ String(mesrel)+"/"+ String(diarel)+ "  --  "+
                           String(hrel1)+":"+ String(mrel1) + Pagina + pagHoras +
                           String(h1)+"/"+ String(m1)+"/"+ String(dr1)


                           
               );
}

void mensajeError() {
  String mensaje = "<h1>404</h1>";
  mensaje += "Pagina No encontrada</br>";
  mensaje += "Intenta otra pagina</br>";
  server.send(404, "text/html", mensaje);
}

void funcionEncender() {
  Estado = true;
  digitalWrite(pinLed, Estado);
  Serial.println("Encender LED");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Endendiendo LED");
}

void funcionApagar() {
  Estado = false;
  digitalWrite(pinLed, Estado);
  Serial.println("Apagar LED");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Apagando LED");
}

void funcionNivel() {

  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    Nivel = atoi(Valor.c_str());
    tira.setBrightness(Nivel);
    Serial << "Cambiando nivel " << Nivel << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Nivel Cambiado");
}

void funcionColor() {
  Serial.println("Cambiando color");
  if (server.hasArg("r")) {
    Valor = server.arg("r");
    Rojo = atoi(Valor.c_str());
    Serial << "Rojo " << Rojo << "\n";
  }
  if (server.hasArg("g")) {
    Valor = server.arg("g");
    Verde = atoi(Valor.c_str());
    Serial << "Verde " << Verde << "\n";
  }
  if (server.hasArg("b")) {
    Valor = server.arg("b");
    Azul = atoi(Valor.c_str());
    Serial << "Azul " << Azul << "\n";
  }

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando color");
}

void funcionArcoiris() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      Arcoiris = true;
    } else {
      Arcoiris = false;
    }

    Serial << "Estado Arcoiris " << Arcoiris << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando Arcoiris");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nIniciando Server Web");




  //  Multi-Conexion - Inicio 
  //  wifiMulti.addAP(ssid_1, password_1);
  //  wifiMulti.addAP(ssid_2, password_2);
  //
  //  WiFi.mode(WIFI_STA);
  //  Serial.print("Conectando a Wifi ..");
  //  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED) {
  //    Serial.print(".");
  //  }
  //  Multi-Conexion - Final


  //  Conexion fija a una sola IP - Inicio ------
  if (!WiFi.config(ip_local, gateway, subnet)) {
    Serial.println("Error en configuracion");
  }
  WiFi.mode(WIFI_STA);
  Serial.print("Conectando a Wifi ..");
  WiFi.begin(ssid_1, password_1);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  //  Conexion fija a una sola IP - Final -------
  
  Serial.println(".. Conectado");
  Serial.print("SSID: ");
  Serial.print(WiFi.SSID());
  Serial.print(" ID: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("elarbol")) {
    Serial.println("Error configurando mDNS!");
    while (1) {
      delay(1000);
    }
  }


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




  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, 0);


#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  Serial.println("Inicial NeoPixel");

  server.on("/" , mensajeBase);

  server.on("/vivo", funcionEncender);
  server.on("/muerto", funcionApagar);

  server.on("/nivel", funcionNivel);
  server.on("/color", funcionColor);
  server.on("/arcoiris", funcionArcoiris);

  server.onNotFound(mensajeError);


  server.begin();
  Serial.println("Servidor HTTP iniciado");

  tira.begin();
  tira.setBrightness(Nivel);
  tira.show();

  Pagina.replace("%ip", WiFi.localIP().toString());

  
}

void loop() {
  server.handleClient();

  #if defined(ESP8266)
     MDNS.update();
  #endif

//  if (Estado) 
//    {
//      funcionEncender();
//      if (Arcoiris) 
//        {
//         ColorArcoiris();
//        }
//      else
//        {
//         uint32_t ColorActual = tira.Color(Rojo, Verde, Azul);
//         ColorSimple(ColorActual);
//        }
//    }
//  else
//    {
//     funcionApagar();
//     tira.clear();
//     tira.show();
//    }
  delay(10);

}

void ColorArcoiris() {
  tira.rainbow(HueActual);
  HueActual += 256;
  if (HueActual > 65536) {
    HueActual = 0;
  }
  tira.show();
}

void ColorSimple(uint32_t ColorActual) {
  for (int i = 0; i < tira.numPixels(); i++) {
    tira.setPixelColor(i, ColorActual);
  }
  tira.show();
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

//      // Horario Cuatro
//      if (horaEnt4 != ""){
//        h4  = horaEnt4.toInt() ;
//        EEPROM.write(120, h4);  
//        EEPROM.commit();
//        delay(10); 
//      }

//      if (minuEnt4 != ""){
//        m4  = minuEnt4.toInt() ;
//        EEPROM.write(121, m4);  
//        EEPROM.commit();
//        delay(10); 
//      }

//      if (duraEnt4 != ""){
//        dr4 = duraEnt4.toInt() ;
//        EEPROM.write(122, dr4);  
//        EEPROM.commit();
//        delay(10);
//      }
      
}
