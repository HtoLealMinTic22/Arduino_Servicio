//#include <ESP_Mail_Client.h>


// Tarjeta "ESP 32", ESP32 Dev Module/921600
// No utilizar el Pin-13, tiene conflicto con el 26
// 20/dic/2023 - Define el llenado de los desechos 
// 27/dic/2023 - Se cambia el Pin - PinPalaUnoAtras a "19" y funciona con 
//             - tiempoAtras=31000;   
// 28/dic/2023 - El Pin - PinPalaUnoAtras  19, trabaja con PullDown, el 
//               Pin 12, no funciona como PullDown 
// 12/feb/2024 - Subido a GitHub
//             - Tres
//             - Nueve
// 12/feb/2024 - Diez
// 12/feb/2024 - Once
// 12/feb/2024 - trece





#include <ESP32Servo.h>
#include <analogWrite.h>



// Sonidos Buzzer
// #include <pitches.h>

// -----    Definicion de PINES  ------------------------
// int IN1                  15 
// int IN2                   2 
// int IN3                   4 
// int IN4                   5 
// #define                  13 
// #define PinPalaUnoAtras  19          
// #define PinPalaDosAtras  26
// #define PinPalaUnoAdel   14   
// No - SDA                 21
// No - SCL                 22
// #define LedLLenado       22
// #define pinServo         23
// #define pinMovto         27 
// #define PinLLenado       32


//#define SMTP_HOST "smtp.gmail.com"
//#define SMTP_PORT esp_mail_smtp_port_465
//#define AUTHOR_EMAIL "adinhto@gmail.com"
//#define AUTHOR_PASSWORD "HtoPao/9662"


// #define AUTHOR_EMAIL    "joshtoleal@gmail.com"
// #define AUTHOR_PASSWORD "gmjoshto0830"



const int pinMovto      = 27 ;   // InfraRojo
int Estado   =  0  ; 

// Servo motor SG-5010 - 5v.
Servo servo;
const    int pinServo =  23 ;
volatile int posServo = 120 ;

volatile int EstadoLLenado ;
volatile int EstPinUnoAtr ;


// Driver 4988 - Con 3.3v 
//const uint8_t steps     = 18;
//const uint8_t direccion = 19;


// Driver L198
int IN1 = 15 ;
int IN2 = 2 ;
int IN3 = 4 ;
int IN4 = 5 ;

#define PinPalaUnoAtras  19      
#define PinPalaUnoAdel   14   
#define PinPalaDosAtras  26 
#define PinLLenado       32
#define LedLLenado       22

//el numero depende del voltaje que le este llegando al driver 4988, a mayor numero mas lento o se bloquea
//volatile int     veloPaso   = 500   ; // Ok - mas rapido / Velocidad motor paso 
//volatile int     veloPaso   = 800   ; // Velocidad motor paso 
//volatile int     veloPaso   = 1000  ; // Velocidad motor paso 
//volatile int     veloPaso   = 1500  ; // Velocidad motor paso 
//volatile int     veloPaso   = 2000  ; // Velocidad motor paso 
//volatile int     veloPaso   = 3000  ; // Velocidad motor paso 
//volatile int     veloPaso   = 4000  ; // ok - Mas lento / Velocidad motor paso 
//volatile int     veloPaso   = 5000  ; // ok - Mas lento / Velocidad motor paso 
//volatile int     veloPaso   = 7000  ; // ok - Mas lento / Velocidad motor paso 
volatile int       veloPaso   = 8000  ; // ok - Mas lento / Velocidad motor paso 
volatile boolean swEnciPaso = false ;

volatile int tiempoAdelante  =  18000;
volatile int tiempoAtras     =  7000;
volatile int numeroPasos     =  30;
volatile int hayObstaculo    = HIGH;       // No hay Obstaculos
volatile boolean swavanceDos = true;
volatile boolean swLimpieza  = false;

volatile boolean sw_AdelaPalaUno  = true ; 
volatile boolean sw_AtrasPalaUno  = true ; 
volatile boolean sw_RetroPalaDos  = true ; 


volatile int timeAdelMot2  =  40000;
volatile int timeAtrasMot2 =  40000;

// Declaramos una variable para referenciar la sesión SMTP
// SMTPSession smtp;

// Prototipo de la funcion callback para obtener el estado del envio del correo
// void smtpCallback(SMTP_Status status);

void setup()  { 
   Serial.begin(115200);

   // Motor con Driver L298
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);   
   pinMode(IN3, OUTPUT);
   pinMode(IN4, OUTPUT); 

   // 5.0v. - SG5010 
   //servo.attach(23,  100, 2500); // Ok se mueve  95°
   //servo.attach(23,  500, 2500); // Ok se mueve  95°
   //servo.attach(23, 1500, 2500); // Ok se mueve  45°
   servo.attach(23,  690, 2500); // Ok se mueve  90°  

   // Motor de Pasos
   // pinMode(direccion, OUTPUT);
   // pinMode(steps, OUTPUT);

   pinMode(pinMovto, INPUT) ;

   pinMode(PinPalaUnoAtras, INPUT_PULLUP ) ;
   pinMode(PinPalaUnoAdel,  INPUT_PULLUP ) ;
   pinMode(PinPalaDosAtras, INPUT_PULLUP ) ;
   pinMode(PinLLenado,      INPUT_PULLUP ) ;
   pinMode(LedLLenado, OUTPUT ) ;
   
 } 
 
void loop()  { 
    revisaLLenado();
        
    delay(500);
    Estado = digitalRead(pinMovto) ; 
    if (Estado == HIGH)
      {
       Serial.println(" <-- Con movimiento --> ");
       delay(50) ;
       for (int conteo=0; conteo<=5000; conteo++){
          Serial.print("El conteo es : ");
          Serial.println(conteo); 
          swLimpieza = true;
        }
      }
    else
      {
       Serial.println(" <-- Sin movimiento --> ");
       swLimpieza = false;
      } 

   // swLimpieza = true ;  // -----------  PROVISIONAL      ------------
   // ------------------------    Proceso de limpieza    ------------
   if (swLimpieza == true){
     
     // hoy27dic
     posServo=0;
     servoMovDos();

     avanceUno();                // Primer ciclo
     delay(500);
     swLimpieza = false;
  }
  delay(1000);
}

void adelanteMot1(){
    // Motor UNO - Anti-Horario
    delay(100);

    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW); 
    delay(tiempoAdelante);
    delay(100);
}

void adelanteMot2(){
  // Motor DOS - Anti-Horario
  delay(100);

  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW); 
  delay(tiempoAdelante);
  delay(100);
}

void atrasMot1(){
   // Motor UNO - Horario
   delay(100);

   digitalWrite(IN1, LOW); 
   digitalWrite(IN2, HIGH); 
   delay(tiempoAtras);
}

void atrasMot1_Dos(){
   // Motor UNO - Horario
   delay(100);

   digitalWrite(IN1, LOW); 
   digitalWrite(IN2, HIGH); 
   delay(tiempoAtras);
}

void atrasMot2(){
  // Motor DOS - Horario
  delay(100);

  Serial.println("Atras Mot-DOS ");    
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH); 
  delay(tiempoAtras);
}

void adelante(){
  // Motor UNO - Anti-Horario
  delay(100);

  Serial.println("Adelante Mot-UNO ");
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  delay(tiempoAdelante);
}

void apagado(){
    // Apaga los dos motores
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, LOW); 

    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW); 
    
    delay(1000);
}

void avanceUno(){

    // Abre laterales
    tiempoAdelante= 5500;
    adelanteMot1();
    apagado();

    // Mueve segunda pala ------------------------
    tiempoAdelante= 55000;
    adelanteMot2();
    apagado();

    // retrocede la pala Dos 
    sw_RetroPalaDos = true ;
    while (sw_RetroPalaDos == true){
        if (digitalRead(PinPalaDosAtras))
          {
           tiempoAtras=1;
           atrasMot2();    
          }
        else
          {
           sw_RetroPalaDos = false ;
           //Serial.print("---  Parada de Retroceso ----  ");
           digitalWrite(IN3, LOW); 
           digitalWrite(IN4, LOW); 
          }
    } // Final While


    // Comienza a mover la arena ------------------
    tiempoAdelante= 33000;
    adelanteMot1();
    apagado();
        
    tiempoAtras = 7000;
    atrasMot1();
    apagado();

    // ----------------------------
    sw_AdelaPalaUno = true ;
    while (sw_AdelaPalaUno == true){
        if (digitalRead(PinPalaUnoAdel))
          {
           tiempoAdelante=1;
           adelanteMot1(); 
          }
        else
          {
           sw_AdelaPalaUno = false ;
           digitalWrite(IN1, LOW); 
           digitalWrite(IN2, LOW); 
           delay(2000);
           }
    } // Final While
    // ----------------------------
    
    tiempoAtras = 20000;
    atrasMot1();
    apagado();


    // ------------------    Ajitar la arena 
    posServo=20;
    for (int ajitaArena=0; ajitaArena<=4; ajitaArena++ )
       {
        posServo=80;
        servoMovUno();
        delay(2000);

        posServo=0;
        servoMovDos();
        delay(2000);
        }

    posServo=30;
    servoMovUno();
    delay(1000);
    // Final ajitar la arena  ----------------------

    // hoy27dic
    // Ultimo retroceso pala Uno 
    sw_AtrasPalaUno = true ;
    while (sw_AtrasPalaUno == true){
        if (digitalRead(PinPalaUnoAtras))
          {
            tiempoAtras=1;
            atrasMot1_Dos();
          }
        else
          {
           sw_AtrasPalaUno = false ;
           digitalWrite(IN1, LOW); 
           digitalWrite(IN2, LOW); 
           // digitalWrite(PinEnabled, LOW); // Prende el Driver 4988 - Apaga(off) el boton de "ENABLED" del Driver 4988
          }
    } // Final While

    // hoy27dic
    // tiempoAtras=31000;   
    // tiempoAtras=30000;   
    // tiempoAtras=25000;
    // tiempoAtras=20000;
    // tiempoAtras=13000;
    // atrasMot1_Dos();
  
    
    apagado();

    // --- Desocupar con servo 
    // Desocupa 5 veces
    for (int numdes=0; numdes<=4; numdes++ ){
       posServo=140;
       servoMovUno();
       delay(4000);

       posServo=0;
       servoMovDos();
       delay(2000);
    }
    // --- Final desocupar con servo 
    
}

void servoMovUno(){
  // Pala Arriba                                
  //servo.attach(23,  690, 2500); // Ok se mueve  90°  
  for (int pos=0; pos<=posServo; pos++){
    servo.write(pos);
    delay(2);  // Entre mas pequeño mas rapido
  }
  //servo.detach();
}

void servoMovDos(){
  
  // Pala Arriba                                
  //servo.attach(23,  690, 2500); // Ok se mueve  90°  
  for (int pos=posServo; pos>=0; pos--){
    servo.write(pos);
    delay(2);  // Entre mas pequeño mas rapido
  }
  //servo.detach();  
}

void revisaLLenado(){
    EstadoLLenado = digitalRead(PinLLenado) ; 
    if (EstadoLLenado==HIGH)
      { digitalWrite(LedLLenado, HIGH) ; }
    else
      { digitalWrite(LedLLenado, LOW)  ; }
}
