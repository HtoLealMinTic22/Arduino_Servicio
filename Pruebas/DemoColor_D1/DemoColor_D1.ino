// ESP32 - Version 1.0.4
// Tarjeta ESP32 Dev Module
// Se debe presionar el "Boot", cuando comienza a subir el programa
// Modo ESTACION (STA)

// 04/feb/23 - se instalo en github



// Creado ChepeCarlos
// Tutorial Completo en https://nocheprogramacion.com
// Canal Youtube https://youtube.com/alswnet?sub_confirmation=1
// nombre GitHub / Pecebre_STA(3)

template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#if defined(ESP32)
   #include <WiFi.h>
   #include <ESPmDNS.h>
   #include <WebServer.h>    // permite construir una pagina 
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

#include <RTClib.h>
#include <Wire.h>
#include "data.h"
RTC_DS3231 rtc;

const uint8_t pinLed    = 15;
const uint8_t pinLedCas = 4;
const uint8_t pinLedFig = 5;
const uint8_t pinLedEnt = 2;
const uint8_t numeroRele = 4;
const uint8_t relays[numeroRele] = {pinLed, pinLedCas, pinLedFig, pinLedEnt};

//int pinLed    = 15;
//int pinLedCas = 4;
//int pinLedFig = 5;
//int pinLedEnt = 2;

//                           // 22 - Clock  - Azul       
//                           // 21 - Data   - Verde 

#define Pin 4
#define CantidadLed 50
Adafruit_NeoPixel tira = Adafruit_NeoPixel(CantidadLed, Pin, NEO_GRB + NEO_KHZ800);

volatile int anorel=0 ;
volatile int mesrel=0 ;
volatile int diarel=0;
volatile int hrel1 =0;
volatile int mrel1 =0;

const uint32_t TiempoEsperaWifi = 5000;
unsigned long TiempoActual   = 0 ;
unsigned long TiempoAnterior = 0 ;
const long    TiempoCancelacion = 500;

boolean Estado = false;
boolean Arcoiris = false;
boolean Figuras = false;
boolean LuzEnt  = false;

int Nivel=50;
int Rojo=255;
int Verde=0;
int Azul=0;

long HueActual=0;
String Valor="";

volatile int mod1=0 ;
volatile int hora1 ;

volatile int horaLuz=0;
volatile int minuLuz=0;
volatile int duraLuz=0;

volatile int horaCas=0;
volatile int minuCas=0;
volatile int duraCas=0;

volatile int horaFig=0;
volatile int minuFig=0;
volatile int duraFig=0;

volatile int hora4=0;
volatile int minu4=0;
volatile int dura4=0;

volatile int fechaDiaAct=0;
volatile int fechaMesAct=0;
volatile int fechaAnoAct=0;
volatile int fechaHorAct=0;
volatile int fechaMinAct=0;

volatile int hrelx = 0 ;
volatile int mrelx = 0 ; 

String varCampo="";
volatile int varMemo;
volatile int apuntaMemo;

volatile float pes1=0 ;

volatile int h1=0;
volatile int h2=0;
volatile int h3=0;
volatile int h4=0;

volatile int m1=0;
volatile int m2=0;
volatile int m3=0;
volatile int m4=0;

volatile int dr1=0;
volatile int dr2=0;
volatile int dr3=0;
volatile int dr4=0;

// Definir variables alarma --
volatile int almhor1=0;
volatile int almmin1=0;
volatile int almhor2=0;
volatile int almmin2=0;
volatile int almhor3=0;
volatile int almmin3=0;

volatile int Almh1=0;
volatile int Almh2=0;
volatile int Almh3=0;

volatile int Almm1=0;
volatile int Almm2=0;
volatile int Almm3=0;

volatile int Almt1=0;
volatile int Almt2=0;
volatile int Almt3=0;
// ---------------------------

IPAddress ip_local(192,168,10,70);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);

void mensajeBase() {
   server.sendHeader("Access-Control-Allow-Origin", "*");
   server.send(200, "text/html", 
                           FeActual+String(anorel)+"/"+ String(mesrel)+"/"+ String(diarel)+ "  --  "+
                           String(hrel1)+":"+ String(mrel1) +  FinFeActual +
                           Parametros +
                           Titulo1+String(h1)+":"+String(m1)+" / "+String(dr1)+" Min"+ PagWebHoras +
                           Titulo2+String(h2)+":"+String(m2)+" / "+String(dr2)+" Min"+ PagWebHoras +
                           Titulo3+String(h3)+":"+String(m3)+" / "+String(dr3)+" Min"+ PagWebHoras +
                           Titulo4+String(h4)+":"+String(m4)+" / "+String(dr4)+" Min"+ PagWebHora4 +
                           FinFeActual + Pagina +
                           
                           AlmTitulo1+String(Almh1)+":"+String(Almm1)+
                           " / "+String(Almh2)+":"+String(Almm2)+ 
                           " / "+String(Almh3)+":"+String(Almm3)+PagWebHora1 + pagAlarma );}
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
  server.send(200, "text/plain", "Encendiendo LED");   // El servidor le envia al cliente 
                                                       // que ya se encendio "Encendiendo LED"
}
void botonUno() {
  Estado = true;
  digitalWrite(pinLed, Estado);
  Serial.println("Encender LED");
  server.sendHeader("Access-Control-Allow-Origin", "*"); 
  server.send(200, "text/plain", "Encendiendo LED");   // El servidor le alcliente // que ya se encendio
}
void funcionApagar() {
  Estado = false;
  digitalWrite(pinLed, Estado);
  Serial.println("Apagar LED");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Apagando LED");}

void funcionHorasGen() {
    EEPROM.commit();
    delay(1000); 
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Dato Modificado"); }

void actuActual() {
    hrelx = hrel1 ;
    mrelx = mrel1 ;
    rtc.adjust(DateTime(anorel, mesrel, diarel, hrelx, mrelx, 0));
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Fecha actualizada"); }

void funcionHoraLuz() {
  if (server.hasArg("valor"))
    { Valor = server.arg("valor");   // "atoi", convierte una cadena en entero
      horaLuz = atoi(Valor.c_str()); // "c_str()", en que parte de la cadena comienza el estring
      Serial << "Cambiando HoraLuz " << horaLuz << "\n";
      h1 = horaLuz ;
      EEPROM.write(100, h1); 
      funcionHorasGen(); }}

void funcionMinuLuz() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    minuLuz = atoi(Valor.c_str());  
    Serial << "Cambiando MinuLuz " << minuLuz << "\n";
    m1 = minuLuz ;
    EEPROM.write(101, minuLuz);  
    funcionHorasGen(); }}

void funcionDuraLuz() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    duraLuz = atoi(Valor.c_str());  
    Serial << "Cambiando DuraLuz " << duraLuz << "\n";
    dr1 = duraLuz ;
    EEPROM.write(108, duraLuz);  
    funcionHorasGen(); }}

void funcionHoraCas() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    horaCas = atoi(Valor.c_str());  
    Serial << "Cambiando HoraCas " << horaCas << "\n";
    h2 = horaCas ;
    EEPROM.write(102, horaCas);  
    funcionHorasGen(); }}

void funcionMinuCas() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    minuCas = atoi(Valor.c_str()); 
    Serial << "Cambiando HoraCas " << minuCas << "\n";
    m2 = minuCas ;
    EEPROM.write(103, minuCas);  
    funcionHorasGen(); }}

void funcionDuraCas() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    duraCas = atoi(Valor.c_str());  
    Serial << "Cambiando DuraCas " << duraCas << "\n";
    dr2 = duraCas ;
    EEPROM.write(109, duraCas);  
    funcionHorasGen(); }}

void funcionHora3() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    horaFig = atoi(Valor.c_str());  
    Serial << "Cambiando HoraFig " << horaFig << "\n";
    h3 = horaFig ;
    EEPROM.write(104, horaFig);  
    funcionHorasGen(); }}

void funcionMinu3() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    minuFig = atoi(Valor.c_str()); 
    Serial << "Cambiando minuFig " << minuFig << "\n";
    m3 = minuFig ;
    EEPROM.write(105, minuFig);  
    funcionHorasGen(); }}

void funcionDura3() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    duraFig = atoi(Valor.c_str()); 
    Serial << "Cambiando duraFig " << duraFig << "\n";
    dr3 = duraFig ;
    EEPROM.write(110, duraFig);  
    funcionHorasGen(); }}

void funcionHora4() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    hora4 = atoi(Valor.c_str());  
    Serial << "Cambiando Hora4 " << hora4 << "\n";
    h4 = hora4 ;
    EEPROM.write(120, hora4);  
    funcionHorasGen(); }}

void funcionMinu4() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    minu4 = atoi(Valor.c_str());  
    Serial << "Cambiando minu4 " << minu4 << "\n";
    m4 = minu4 ;
    EEPROM.write(121, minu4);  
    funcionHorasGen(); }}

void funcionDura4() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    dura4 = atoi(Valor.c_str());  
    Serial << "Cambiando dura4 " << dura4 << "\n";
    dr4 = dura4 ;
    EEPROM.write(122, dura4);  
    funcionHorasGen(); }}

void funcionDiaAct() {                                              // Fecha ACTUAL - Dia
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    fechaDiaAct = atoi(Valor.c_str()); 
    Serial << "Cambiando fechaDiaAct " << fechaDiaAct << "\n";
    diarel = fechaDiaAct;
    actuActual();} }   

void funcionMesAct() {                                              // Fecha ACTUAL - Mes
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    fechaMesAct = atoi(Valor.c_str()); 
    Serial << "Cambiando fechaDiaAct " << fechaDiaAct << "\n";
    mesrel = fechaMesAct;
    actuActual();} }

void funcionAnoAct() {                                              // Fecha ACTUAL - Año
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    fechaAnoAct = atoi(Valor.c_str()); 
    Serial << "Cambiando fechaAnoAct " << fechaAnoAct << "\n";
    anorel = fechaAnoAct;
    actuActual();} }

void funcionHorAct() {                                              // Fecha ACTUAL - Hora
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    fechaHorAct = atoi(Valor.c_str()); 
    Serial << "Cambiando fechaHorAct " << fechaHorAct << "\n";
    hrel1 = fechaHorAct;
    actuActual();} }

void funcionMinAct() {                                              // Fecha ACTUAL - Minutos
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    fechaMinAct = atoi(Valor.c_str()); 
    Serial << "Cambiando fechaMinAct " << fechaMinAct << "\n";
    mrel1 = fechaMinAct;
    actuActual();} }

void funcionAlmHor1() {                                              // Alarmas UNO 
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almhor1 = atoi(Valor.c_str());  
    Serial << "Cambiando almhor1 " << almhor1 << "\n";
    Almh1 = almhor1 ;
    EEPROM.write(111, almhor1);  
    funcionHorasGen(); }}

void funcionAlmMin1() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almmin1 = atoi(Valor.c_str());  
    Serial << "Cambiando almmin1 " << almmin1 << "\n";
    Almm1 = almmin1 ;
    EEPROM.write(112, almmin1);  
    funcionHorasGen(); }}

void funcionAlmHor2() {                        // Alarmas DOS 
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almhor2 = atoi(Valor.c_str());  
    Serial << "Cambiando almhor2 " << almhor2 << "\n";
    Almh2 = almhor2 ;
    EEPROM.write(114, almhor2);  
    funcionHorasGen(); }}

void funcionAlmMin2() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almmin2 = atoi(Valor.c_str());  
    Serial << "Cambiando almmin2 " << almmin2 << "\n";
    Almm2 = almmin2 ;
    EEPROM.write(115, almmin2);  
    funcionHorasGen(); }}

void funcionAlmHor3() {                        // Alarmas TRES
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almhor3 = atoi(Valor.c_str());  
    Serial << "Cambiando almhor3 " << almhor3 << "\n";
    Almh3 = almhor3 ;
    EEPROM.write(117, almhor3);  
    funcionHorasGen(); }}

void funcionAlmMin3() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    almmin3 = atoi(Valor.c_str());  
    Serial << "Cambiando almmin3 " << almmin3 << "\n";
    Almm3 = almmin3 ;
    EEPROM.write(118, almmin3);  
    funcionHorasGen(); }}

void funcionArcoiris() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      Arcoiris = true;
      digitalWrite(pinLedCas, Arcoiris);
    } else {
      Arcoiris = false;
      digitalWrite(pinLedCas, Arcoiris);}
    Serial << "Estado Arcoiris " << Arcoiris << "\n";
    }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "cambiando Arcoiris");}

void funcionFiguras() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      Figuras = true;
      digitalWrite(pinLedFig, Figuras);} 
    else {Figuras = false;
          digitalWrite(pinLedFig, Figuras);}
    Serial << "Estado Arcoiris " << Figuras << "\n";}
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "cambiando Figuras");}

void funcionLuzEnt() {
  if (server.hasArg("estado")) {
    Valor = server.arg("estado");
    if (Valor == "vivo") {
      LuzEnt = true;
      digitalWrite(pinLedEnt, LuzEnt);} 
    else {LuzEnt = false;
          digitalWrite(pinLedEnt, LuzEnt);}
    Serial << "Estado Arcoiris " << LuzEnt << "\n";}
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "cambiando Luz de Entrada");}

void funcionNivel() {
  if (server.hasArg("valor")) {
    Valor = server.arg("valor");
    Nivel = atoi(Valor.c_str());    // "atoi", convierte una cadena en entero
                                    // "c_str()", en que parte de la cadena comienza el estring
    tira.setBrightness(Nivel);
    Serial << "Cambiando nivel " << Nivel << "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Nivel Cambiado");
}
void ColorArcoiris() {
  tira.rainbow(HueActual);
  HueActual += 256;
  if (HueActual > 65536) {HueActual = 0;}
  tira.show();
}
void ColorSimple(uint32_t ColorActual) {
  for (int i = 0; i < tira.numPixels(); i++) {
    tira.setPixelColor(i, ColorActual); }
  tira.show();}



void setup() {
    Serial.begin(115200);
    Serial.println("\nIniciando Server Web");
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
    Wire.begin();     // 22-Clock / 21-Data  
    DateTime now = rtc.now();
    anorel = now.year()  ;
    mesrel = now.month() ;
    diarel = now.day()   ;
    hrel1  = now.hour()  ;
    mrel1  = now.minute();

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
    horaLuz = h1;
  
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

    for (uint8_t k1=0; k1<numeroRele; k1++ ){
       pinMode(relays[k1], OUTPUT);
       digitalWrite(relays[k1], true);
    };

    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    Serial.println("Inicial NeoPixel");

    server.on("/" , mensajeBase);         // cualquier peticion que nos hagan los clientes 
                                          // se responde con un "server.on"
                                          // "mensajeBase", es la pagina Web
    server.on("/vivo",  funcionEncender);
    server.on("/muerto",funcionApagar);

    server.on("/arcoiris", funcionArcoiris);
    server.on("/figuras",  funcionFiguras);
    server.on("/luzentra", funcionLuzEnt);
 
    server.on("/horaLuz", funcionHoraLuz);
    server.on("/minuLuz", funcionMinuLuz);
    server.on("/duraLuz", funcionDuraLuz);

    server.on("/horaCas", funcionHoraCas);
    server.on("/minuCas", funcionMinuCas);
    server.on("/duraCas", funcionDuraCas);

    server.on("/hora3", funcionHora3);
    server.on("/minu3", funcionMinu3);
    server.on("/dura3", funcionDura3);

    server.on("/hora4", funcionHora4);
    server.on("/minu4", funcionMinu4);
    server.on("/dura4", funcionDura4);

    server.on("/diaActEnt", funcionDiaAct);     // Fecha ACTUAL  
    server.on("/mesActEnt", funcionMesAct);
    server.on("/anoActEnt", funcionAnoAct);  
    server.on("/horActEnt", funcionHorAct);    
    server.on("/minActEnt", funcionMinAct);

    server.on("/hora1Alm", funcionAlmHor1);    // Alarmas UNO 
    server.on("/minu1Alm", funcionAlmMin1);
    server.on("/hora2Alm", funcionAlmHor2);    // Alarmas DOS
    server.on("/minu2Alm", funcionAlmMin2);
    server.on("/hora3Alm", funcionAlmHor3);    // Alarmas TRES
    server.on("/minu3Alm", funcionAlmMin3);

    server.on("/nivel", funcionNivel);
    //server.on("/control", botonUno);
    server.onNotFound(mensajeError);

    server.begin();                            // se inicia el servidor para que comience a escuchar peticiones
    Serial.println("Servidor HTTP iniciado");

    tira.begin();
    tira.setBrightness(Nivel);
    tira.show();
    Pagina.replace("%ip", WiFi.localIP().toString());
}

void loop() {
  server.handleClient(); // gestiona los requerimientos http, que lleguen
  #if defined(ESP8266)
     MDNS.update();
  #endif
  delay(10);

  DateTime now = rtc.now();
  anorel = now.year()  ;
  mesrel = now.month() ;
  diarel = now.day()   ;
  hrel1  = now.hour()  ;
  mrel1  = now.minute();
}
