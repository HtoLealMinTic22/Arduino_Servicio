#include <Arduino.h>
#ifdef ESP32
   #include <WiFi.h>
   #include <AsyncTCP.h>
#elif defined(ESP8266)
   #include <ESP8266WiFi.h>
   #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

volatile int anorel=0;
volatile int mesrel=0;
volatile int diarel=0;
volatile int hrel1 =0;
volatile int mrel1 =0;
volatile int srel1 =0;

const char* ssid     = "Sistema Adin 2G";
const char* password = "Leal083096";

const char FeActual[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
  <head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='ie=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  </head>

  <body>
   <h1>Contador Actual : </h1>
   <h4 id='diarel'> 0 </h4>
  
   <input type='number' name='Almhora1' id='Almhora1' placeholder='Hora'    min='0' max='24'  style='width:15%;height:35px'>
   <form method='get' target='pantalla'>
    
     <input type='text'   name='message'  id='message'   placeholder='mensaje'                    style='width:75%;height:35px'>
     <br><br>
       <input class='boton'  type='submit' value='Enviar' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'>
     <br><br>
   </form>

   <script>
      const timeout = 5000 ;
      let   contador= 0;

      document.addEventListener('DOMContentLoaded', () => {
        console.log('Documento esta LISTO');
        setTimeout(updateDiaHoy, timeout); }) ;

      function updateDiaHoy(processTemp){
        console.log('Dia de HOY');

        const xhr = new XMLHttpRequest();
        //let xhr = new XMLHttpRequest();
        xhr.readystatechange = () => {
        //if (xhr.Status === 200){  
        //if (xhr.readyState === 4 ){        
        if (xhr.readyState === 4 &&  xhr.Status === 200){
           //contador= contador+1 ;
           //diarel.innerText = contador;
           diarel.innerText = xhr.responseText;
          }
        };
        
        contador= contador+1 ;
        diarel.innerText = contador;
        
        xhr.open("GET", "/HoyActual");
        xhr.send();
        setTimeout(updateDiaHoy, timeout); 
        };

  </script>
   
  </body>
</html>
)rawliteral";


const char* PARAM_MESSAGE = "get/message";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/html", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }


    /// Reloj  
    /// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Wire.begin();     // 22-Clock / 21-Data  
    DateTime now = rtc.now();
    anorel = now.year()  ;
    mesrel = now.month() ;
    //diarel = now.day()   ;
    hrel1  = now.hour()  ;
    mrel1  = now.minute();
    diarel = now.second();

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());




    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", FeActual); });


    server.on("/HoyActual", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", String(diarel,1));  });

    // Send a GET request to <IP>/get?message=<message>
    // http://192.168.10.170/get?message=Humberto
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
         
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/html", "Hello, este es un 'GET' : " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
//    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
//        String message;
//        if (request->hasParam(PARAM_MESSAGE, true)) {
//            message = request->getParam(PARAM_MESSAGE, true)->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/html", "Hello, POST: " + message);
//    });

    server.onNotFound(notFound);
    server.begin();
}

void loop() {
    DateTime now = rtc.now();
    //anorel = now.year()  ;
    //mesrel = now.month() ;
    // diarel = now.day()   ;
    hrel1  = now.hour()  ;
    mrel1  = now.minute();
    diarel  = now.second();
    Serial.print("Segundos :");
    Serial.println(diarel);
    delay(1000);

}
