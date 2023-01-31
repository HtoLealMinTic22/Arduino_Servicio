
// Tx-12 Trasmite del Celular a Arduino, Rx-13 trasmite de Arduino a Celular 
// Trabaja con dos octo-acopladores, en el llenado del tanque General de limpieza
// llenaPileta / Ene/20/20

// Cambia la fecha de inicio de proceso de floculacion, solo cuando la cantidad de agua que entra al 
// recipiente inicial y el tiempo es diferente de (0), eliminando asi el FeedBack de agua por 
// reciduos pequeños de agua despues de terminar el baño.

// llenaPileta / Ene/27/20
// Mantiene apagada la electro balvula principal, solo se prende cuando el flotador del recipiente inicial indica que hay 
// agua en el mismo. Al mantener esta electrovalvula apagada se disminuye el FeedBack de agua, porque el agua no se 
// devuelve.

// llenaPileta
// Al comenzar a llenar la pileta desocupa 15 seg. de residuos y posterormente, comienza a 
// vaciar el agua en la pileta.  

// llenaPileta Feb/03/20
// Al terminar de llenar la 

// Transmite los datos a la pileta para prender los motores Paso a Paso, conectando los dos 
// Arduinos Feb/10/20, Feb/26/20
// Transmisor o Emisor (A1-A2)

// Feb/28/2020 - Prende las valvulas solo cuando se inicia el proceso del baño
//               Elimina Elec_bomba1/Elec_bomba2

// Mar/03/2020 - Elimina Bomba6 LLena tanque limpieza (5) (Final)
// Mar/12/2020 - Las valvulas se manejan con la Opcion de reciclar o no agua.
// Abr/01/2020 - Se cambio el pin del tanque General(PinTanque) de A3 a D5
// Jul/02/2020 - Se desinstalo el segundo Nema-17, las dos valvulas de paso se sincronizan para que al mismo 
//             - tiempo se abran y cierren respectivamente.
// Jul/06/2020 - Se reviso la modificacion de la fecha actual por AppInventor - Ok
// Jul/06/2020 - Se reviza el paso de agua cuando no se recicla
// Jul/06/2020 - Eliminando residuos Iniciales
// Sep/18/2020 - Se elimina la Elec_Bomba3 eliminacion de reciduos(9)
// Nov/13/2020 - Se activa la nueva PCB
// Dic/04/2020 - Se cambio la Electro-Valvula por motor de Paso y se
//               activan los controles de desocupar Tq._Gral. con el 
//               mensaje de "Residuos ON"
// Dic/09/2020 - Se activa la eliminacion de residuos con el motor de Paso.
// Ene/04/2021 - Antes de comenzar a llenar la pileta, elimina los primeros residuos
// Ene/05/2021 - Prende el agitador cuando hay paso de agua y asi lava el tanque General
// Ene/12/2021 - Se comienza hacer el llenado del tanque General, cuando se eliminan los residuos iniciales
// Jun/21/2021 - Se cambio el giro del motor Paso a paso, de la opcion reciclar o no. 
//             - No prende el motor de aguitar si no esta reciclando el agua.
//             - No prende la bomba que llena el tanque General, si no esta reciclando el agua.
//             - Pin No. 9 esta libre
// Sep/22/2021 - Se cambio la PCB, incluyendo un Driver adicional para el llenado de la pileta
// Oct/06/2021 - Se continua la instalacion de la nueva PCB
// Jun/22/2022 - Se cierra la valvula que alimenta la pileta antes de iniciar el proceso de residuos
// AGO/17/2022 - Se suspende la opcion de Residuos Iniciales (Resini)
// Sep/13/2022 - Se asigna el color fuscia al LED, cuando no hay quimico
// Oct/07/2022 - Se pasa a 64 Bits.
// Oct/07/2022 - Bomba3 ajitado del quimico(9) 
// Ene/20/2023 - Se modifica el tiempo de decantacion del agua Tq-Gral a 720 minutos(12 horas)
// Ene/20/2023 - Se cambiaron los mensaje de PuertoDos.print
//              - To-(Tanque General Open) 
//              - Tc-(Tanque General Cierra) 
//              - Si-(Si recicla) 
//              - Nr-(No recicla) 
//              - Re-(Abre Residuos ) 
//              - Ri-(Residuos Iniciales) 
// Ene/26/2023  - Se cambio la velocidad del PuertoDos.begin(9600);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <PololuLedStrip.h>

//        --------------------------------------------------------------------------------------------
//        -----------------------------------   Distribucion de Pines --------------------------------
//        --------------------------------------------------------------------------------------------
//  2 - PinFlujo Mide el caudal de agua
//  3 - Led RGB Reciclar On / Reciclar Off
//  4 - Define el pin 4 Flotador2 llenado del tanque del Quimico (4)
//  5 - PinTanque / Flotador LLenado del tanque limpieza
//  6 - Bomba5 LLena tanque limpieza (6) (Inicial)- OctoAcoplador LOW - Apaga / HIGH - Prende
//  7 - Sensor_PasoAgua -  
//  8 - Bomba2 ajitado del agua (8)
//  9 - Bomba3 ajitado del quimico(9) 
// 10 - 
// 11 - Bomba4 Alimentador de los quimicos al tanque general(11)
// 12 - Tx - Bluetooth
// 13 - Rx - Bluetooth
  
// A0 - PuertoDos de comunicacion con Arduino-2 (Tx)
// A1 - PuertoDos de comunicacion con Arduino-2 (Rx)
// A2 - Boton Reciclar On / Reciclar Off 
// A3 -                                  
// A4 - SDA - Pantalla LCD
// A5 - CSL - Pantalla LCD

PololuLedStrip<3> ledStrip;
SoftwareSerial PuertoUno(12,13);   // Tx-12 Trasmite del Celular a Arduino, Rx-13 trasmite de Arduino a Celular 
SoftwareSerial PuertoDos(A0,A1);   // transmite datos a la tarjeta-Pileta

//RTC_DS1307 RTC;
RTC_DS3231 rtc;

LiquidCrystal_I2C lcd(0x27,16,2);

#define PinFlujo        2  // PinFlujo Mide el caudal de agua
#define PinFlota2       4  // Define el pin 4 Flotador2 llenado del tanque del Quimico (4)
#define PinTanque       5  // PinTanque - Flotador LLenado del tanque limpieza
#define PinLLenoAux6    6  // Bomba5 LLena tanque limpieza (6) (Inicial)
#define sensor_PasoAgua 7  // Sensor_PasoAgua -  


volatile int vr_PasoAgua ;
volatile int vr_LLenoAux ;
volatile int vr_LLenoTanque ;
volatile int vr_PinFlota2 ;
volatile int anorel=0 ;
volatile int mesrel=0 ;
volatile int diarel=0 ;
volatile int hrel  =0 ;
volatile int mrel  =0 ;
volatile int vr_TgLLeno =0 ;
volatile int ti = 0 ;
volatile int hto =0 ;

volatile int tgMinutos =0 ;
volatile int tanorel=0 ;
volatile int tmesrel=0 ;
volatile int tdiarel=0 ;
volatile int thorarel=0 ;
volatile int tMinrel=0 ;

volatile int AcuAno = 0 ;
volatile int AcuMes = 0 ;
volatile int Acudia = 0 ;
volatile int AcuHora= 0 ;
volatile int AcuMin = 0 ;

volatile long anomov =0 ;
volatile int mesmov =0 ;
volatile int diamov =0 ;
volatile int horamov=0 ;
volatile int minumov=0 ; 
volatile int vrLimpia=0 ; 
volatile int Mezclar ;

volatile float caudal_L_m ;
volatile int NumPulsos;       //variable para la cantidad de pulsos recibidos
float factor_conversion=7.11; //para convertir de frecuencia a caudal
float volumen=0;
long dt=0;                    //variación de tiempo por cada bucle
long t0=0;

unsigned long time  ;

volatile String mensaAnt ;
volatile int contVeces=0 ; 
volatile int rpmcont ;
volatile int Calc;

#define LED_COUNT 1
rgb_color colors[LED_COUNT];

char buffer[70]    ;
char vector[15] ;
int indVector ;
volatile boolean swstr = false ;
volatile boolean swFlota2Agua = false ;
volatile boolean swLLenando   = false ;
volatile boolean swEnviadoAbre= false ;
volatile boolean swEnviadoCie = false ;
volatile boolean swInicioBano = false ;
volatile boolean swTgCier     = false ;

String inputEnt ;
String frase ;
String fraseStr ;
char   frasechar ;
String fraseInt ;
int    fraseStrVal ; 
int    LedReci = 0;

volatile int Pin_recicla = A2  ;  // Pin reciclar agua

volatile int bomba2      =  8  ;  // Bomba2 ajitado del agua (8)
volatile int bomba3      =  9  ;  // Bomba3 ajitado del quimico(9)
volatile int bomba4      = 11  ;  // Bomba4 Alimentador de los quimicos con el agua(11)
volatile int bomba5      =  6  ;  // Bomba5 LLena tanque limpieza (6) (Inicial)

volatile long duracion1, distancia1;   
volatile long duracion2, distancia2;   
volatile long porcellen1, porcellen2 ;   
volatile int  MsApagar = 0 ;

volatile boolean swlleno     = false ;
volatile boolean swLimpieza  = false ;
volatile boolean swMsM1      = false ; 
volatile boolean swMezclador = false ;
volatile boolean SwActTubAgua= true  ;
volatile int     Sw_recicla ;

void ContarPulsos ()  
{ 
  NumPulsos++;               //incrementamos la variable de pulsos
} 


void setup()
{  
   lcd.init();                      // initialize the lcd 
   lcd.backlight();
  
   delay(1000); 
   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
 
   // Si se ha perdido la corriente, fijar fecha y hora
   //if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2019, 1, 21, 3, 0, 0));
   //}
   // rtc.begin() ;
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // rtc.setDate(2019, 1, 21, 9, 59, 0);

   Serial.begin (115200);       // inicializa el puerto seria a 115200 baudios
   PuertoUno.begin(9600);
   PuertoDos.begin(9600);

   // Interrupcion para el Sensor de flujo
   rpmcont=0;
   attachInterrupt(digitalPinToInterrupt(2), ContarPulsos, RISING); // (2) Conecta al Pin-2  / (3) Conecta al Pin-3 (Interupcion Externa)
   interrupts();        // Solo una vez                                           // solo los pines 2 y 3 hacen interupciones externas
                                                                   // llama la funcion "rpm"
    // Reloj
   Wire.begin();
   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
   Mezclar = EEPROM.read(9) ; //

   if (Mezclar==0)
     {swMezclador = false; }
   else
     {swMezclador = true ; }  

   if (vrLimpia == 0)
     {swLimpieza = false; }
   else
     {swLimpieza = true ; }  

   DateTime now = rtc.now();
   anorel = now.year()  ;
   mesrel = now.month() ;
   diarel = now.day()   ;
   hrel   = now.hour()  ;
   mrel   = now.minute();

   pinMode(PinFlota2, INPUT);    // Flotador2 llenado del tanque del Quimico (4)
 //pinMode(Ptrig1, OUTPUT);      // define el pin 5 como salida  (triger)
   pinMode(PinLLenoAux6, INPUT); // define el pin 6 LLenado de la pileta 
   pinMode(PinTanque, INPUT);    // define el pin 5 LLenado del tanque limpieza
   pinMode(Pin_recicla, INPUT);  // define el pin 2 
           
   pinMode(bomba2,OUTPUT);       // Bomba2 aguitado del agua (8)
   pinMode(bomba3,OUTPUT);       // Bomba3 ajitado del quimico(9)
   pinMode(bomba4,OUTPUT);       // Bomba4 Alimentador de los quimicos con el agua(11)
   pinMode(bomba5,OUTPUT);       // Bomba5 LLena tanque limpieza (6) (Inicial)
   
   pinMode(sensor_PasoAgua, INPUT) ;
 
   digitalWrite(bomba2, HIGH)      ; // Apaga bomba2 aguitado del agua (8)
   digitalWrite(bomba3, HIGH)      ; // Apaga Bomba3 ajitado del quimico(9)
   digitalWrite(bomba4, HIGH)      ; // Apaga Bomba4 Alimentador de los quimicos con el agua(11)
    
   totalMinutos() ;
   DuracionBano() ;

   Sw_recicla = digitalRead(Pin_recicla) ; 
   if (Sw_recicla == LOW) 
     {
      mensaAnt = "Si" ;
      PuertoDos.print("Si");    
     }
   else
     {
      PuertoDos.print("Nr"); 
      mensaAnt = "Nr" ;
     }
   PuertoDos.print("Inicio");    

}

void loop()
{
   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
   Mezclar = EEPROM.read(9) ; //

   if (Mezclar==0)
     {swMezclador = false;
     //Serial.println("  NO mezclar agua "); 
     }
   else
     {swMezclador = true ; 
      // Serial.println("  SI mezclar agua "); 
     }  

   if (vrLimpia == 0)
     {swLimpieza = false; }
   else
     {swLimpieza = true ; }  

   serialEvent();

   Sw_recicla = digitalRead(Pin_recicla) ; 
   if (Sw_recicla == LOW)  
     { //Serial.print(" Reciclar  ");  
      colors[LedReci] = rgb_color(51, 204, 0);  // Led en verde
      ledStrip.write(colors, LED_COUNT);

      lcd.setCursor(0,0) ;
      lcd.print("Ahorro          ") ;  

      calculoLts() ;
      DuracionBano() ;

      if (mensaAnt == "Si")
        {
        }
      else  
        {
         Serial.println("Abro la Valvula-Recicla ");        
         PuertoDos.print("Si"); 
         mensaAnt = "Si" ;
        }
   }
  else
    { 
      //Serial.print("No recicla ");  
      digitalWrite(bomba5, LOW)       ; // Apaga bomba5 llena tanque limpieza (6) (Inicial)
  
      colors[LedReci] = rgb_color(204, 0, 0); // Led en rojo
      ledStrip.write(colors, LED_COUNT);    

      DuracionBano();
      for (int ldc=0; ldc<=5; ldc++){
        lcd.setCursor(ldc,0) ;
        lcd.print(" ") ;                
        lcd.print("- NO recicla - ") ;   
        delay(600);     
      }

      if (mensaAnt != "Nr")
        {
         PuertoDos.print("Nr"); 
         Serial.println("Apago la Valvula-Recicla ");
         mensaAnt = "Nr" ;
        }
      
    }  
//
 vr_LLenoTanque  = digitalRead(PinTanque) ; 
 if (vr_LLenoTanque == LOW)  
   { Serial.print("  Tq-Gral/Limpieza Con agua ");  
   }
 else
   { 
    Serial.println("-- Tanque General de limpieza vacio --") ; 
    //swLimpieza     = 0 No / 1 Si
    //Serial.print("Valor de swLimpieza es..(1) : ") ;    
    //Serial.println(swLimpieza) ;    
    
    if (swLimpieza == true )
      {
        // Desocupar los residuos del tanque
        Serial.println("-- Eliminando residuos --") ; 

        vrLimpia = 0 ;
        EEPROM.write(8, vrLimpia);  // Ya se hiso la limpieza de los residuos
        delay(100) ;
        swLimpieza = false ;
        
        delay(7000) ;                // 7 Segundos, espera a desocupar el tanque por 7 seg.
        digitalWrite(bomba2, HIGH);  // Apaga bomba2 aguitado del agua (8)

        delay(5000) ;
        desocupaTanque() ;

        Serial.println("-- Envio Residuos ON1 -- ") ;  

        Mezclar=0;
        EEPROM.write(9, Mezclar);  // Opcion de mezclar o aguitar  */
        swMezclador = false ;
       }    
   }

 
  // Coloca la pileta siempre sin agua      ************* O  J  O ***********
  // vr_LLenoAux = digitalRead(PinLLenoAux6) ; 
  // vr_LLenoAux = 1; // Colocar fijo siempre
  if (vr_LLenoAux != LOW)
     { //Serial.print("  Aux-vacio  ")   ;
      }
  else
     { //Serial.print("  Aux-Con Agua  ");
     }    


//vr_LLenoTanque  = digitalRead(PinTanque) ; 
//if (vr_LLenoTanque == LOW) 

  vr_PinFlota2 = digitalRead(PinFlota2) ; // Flotador2 llenado del tanque del Quimico (4)
  if (vr_PinFlota2 != LOW)
     { 
       Serial.print("  Flotador2 Sin Quimico  ");   
       lcd.setCursor(0,0) ;
       lcd.print(" - Dispensador - ");      
       lcd.setCursor(0,1) ;
       lcd.print("  SIN  Quimico  ");      

       colors[LedReci] = rgb_color(255, 0, 255); // Led en fuscia
       ledStrip.write(colors, LED_COUNT); 
       
       delay(2000) ;        
     }
  else
     { 
      Serial.print("  Flotador2 Con Quimico ");  
     }    


 // Sensa la fecha del ultimo paso de agua al sistema
 // con agua - 0 LOW / sin agua - 1 HIGH
 vr_PasoAgua = digitalRead(sensor_PasoAgua) ;
 // vr_PasoAgua = 1 ;
 if (vr_PasoAgua == HIGH )
     { 
      Serial.println("  Tubo sin agua = 1 ");  
      digitalWrite(bomba5, LOW);  // Apaga bomba5 llena tanque limpieza (6) (Inicial)
      digitalWrite(bomba2, HIGH); // Apaga bomba2 aguitado del agua (8)       
     }
 else
     { 
      Serial.println("  Tubo con agua = 0  "); 
      swInicioBano = true ;
      swEnviadoCie = false ;

      Sw_recicla = 0 ;
      swEnviadoCie == false ;

      // Hoy21Jun
      Sw_recicla = digitalRead(Pin_recicla) ; 
      if (Sw_recicla == LOW)  
        { //Serial.print(" Reciclar  ");
         digitalWrite(bomba5, HIGH); // Prende bomba5 llena tanque limpieza (6) (Inicial)
         digitalWrite(bomba2, LOW) ; // Prende bomba2 aguitado del agua (8)
        }
      else
        {
         digitalWrite(bomba5, LOW) ; // Apaga bomba5 llena tanque limpieza (6) (Inicial)
         digitalWrite(bomba2, HIGH); // Apaga bomba2 aguitado del agua (8)
        }

       if (SwActTubAgua==true && volumen > 0.70 )
         {
          //Serial.println(" Actualizando Hora-Mto"); 
          DateTime now = rtc.now();
          anomov  = now.year()  ;
          mesmov  = now.month() ;
          diamov  = now.day()   ;
          horamov = now.hour()  ;
          minumov = now.minute();
        
          EEPROM.write(1, mesmov) ;  // Mes movimiento
          EEPROM.write(2, diamov) ;  // Dia movimiento
          EEPROM.write(6, horamov);  // Hora movimiento
          EEPROM.write(7, minumov);  // Minu movimiento 
          
          SwActTubAgua = false ;        
          swLimpieza   = false ;

          Mezclar=1;
          EEPROM.write(9, Mezclar);  // Opcion de mezclar o aguitar  */
          swMezclador  = true  ;


          // hoy26ene
          // DesActivar limpieza        
          vrLimpia = 0 ;
          EEPROM.write(8, vrLimpia);  // Minutos movimiento
          swLimpieza = false ;

          //lcd.setCursor(6,1) ;
          //lcd.print("Time.:   ") ; 
       }
     }  

 if (swMsM1 == true)
   {
    vr_TgLLeno = 1 ;
    MsApagar   = 3 ;
    swMsM1     = false ;
   }
 else
   { vr_TgLLeno = 0 ;
     MsApagar   = 0 ;
     swMsM1     = false ;      
   }

 verTime();
 verhour() ;
 DateTime now = rtc.now();
 anorel = now.year()  ;
 mesrel = now.month() ;
 diarel = now.day()   ;
 hrel   = now.hour()  ;
 mrel   = now.minute();

 sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",diarel,mesrel,anorel,hrel,mrel,diamov,mesmov,MsApagar,horamov,minumov,vrLimpia,vrLimpia,porcellen1) ;
   
 PuertoUno.print(" ");    // Comandos Buffer
 PuertoUno.print(buffer); // Comandos Buffer

 Serial.print(" ");
 Serial.print(buffer);
 Serial.println("");

 delay(1000);

 // Totaliza el tiempo desde la entrada de agua
 totalMinutos ();
 if (tgMinutos <= 60)      
   {
    llenaTqLimpieza () ;
   }
 else
    {
     // aguitar y mezclar el agua entre 30 minutos y 5 horas
     if (swMezclador == true )
       {
        // aguitar y mezclar el agua entre 30 minutos y 5 horas              
        mezclaQuimico () ; 

        // hoy26ene
        //Activar limpieza        
        vrLimpia = 1 ;
        EEPROM.write(8, vrLimpia);  // Minutos movimiento
        swLimpieza = true ;



//        DateTime now = rtc.now();
//        anomov  = now.year()  ;
//        mesmov  = now.month() ;
//        diamov  = now.day()   ;
//        horamov = now.hour()  ;
//        minumov = now.minute();
//         
//        EEPROM.write(1, mesmov) ;  // Mes movimiento
//        EEPROM.write(2, diamov) ;  // Dia movimiento
//        EEPROM.write(6, horamov);  // Hora movimiento
//        EEPROM.write(7, minumov);  // Minu movimiento
//        swMezclador = false ;
       }
     
     if (tgMinutos >= 720) 
       {
         llenaPileta () ;
       }
    }  
}

void verTime() {
  DateTime now = rtc.now();
}

void verhour() {
   lcd.setCursor(0,1) ;
   DateTime now = rtc.now();
   if (now.hour() <= 9)
     {
      lcd.print(now.hour(), DEC);
      lcd.print(':');

      if (now.minute() <= 9)
        { lcd.setCursor(2,1) ;
          lcd.print(now.minute(), DEC);
          lcd.print(" ");      
        }
      else
        { lcd.print(now.minute(), DEC); }
      }
   else
     {
      lcd.print(now.hour(), DEC);
      lcd.print(':');
      if (now.minute() <= 9)
        { lcd.setCursor(3,1) ;
          lcd.print(now.minute(), DEC);
          lcd.print(" ");      
        }
      else
        { lcd.print(now.minute(), DEC);
        }
      }
}

void DuracionBano() {
   lcd.setCursor(6,1) ;
   //lcd.print("Time.:") ; 
   if (tgMinutos<=9)  
     {
      lcd.print(" Time.:  ") ; 
      lcd.setCursor(15,1) ;

     }
   else
     { if (tgMinutos<=99)  
        {
         lcd.print(" Time.:") ; 
         lcd.setCursor(14,1) ;
        }
        else
           {if (tgMinutos<=999)  
              {
               lcd.print("Time.: ") ; 
               lcd.setCursor(13,1) ;
              }
            else
              {
               lcd.print("Time.: ") ; 
               lcd.setCursor(12,1) ;
              }    
           }
     }
     
   lcd.print(tgMinutos,DEC) ;   
}

 //  lcd.print("1234567890123456") ; 


void calculoLts() {
  float frecuencia = ObtenerFrecuecia();       //obtenemos la frecuencia de los pulsos en Hz
  caudal_L_m = frecuencia/factor_conversion;   //calculamos el caudal en L/m
  dt=millis()-t0;                              //calculamos la variación de tiempo
  t0=millis();

  volumen=volumen+(caudal_L_m/60)*(dt/1150);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1200);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1050);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1000);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/900);     // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1100);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1050);  // volumen(L)=caudal(L/s)*tiempo(s)
                                                // Entre mayor sea el numero disminuye el volumen
  lcd.setCursor(8,0) ;
  lcd.print(volumen) ;  
  lcd.print(" Lts") ;  
}

int ObtenerFrecuecia() 
{
  int frecuencia;
  NumPulsos = 0;        // Ponemos a 0 el número de pulsos
  // interrupts();      // Bloquea la LCD (no colocar)- Habilitamos las interrupciones
  delay(1000);          // muestra de 1 segundo
  //  noInterrupts();   // Bloquea la LCD (no colocar)- Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; // Hz(pulsos por segundo)
  return frecuencia;
}

void serialEvent(){
 indVector = 0 ;
 swstr = false ;
 PuertoUno.listen();
 
   while (PuertoUno.available()) 
    {
     if (PuertoUno.available() > 0)
       {
       char letra = PuertoUno.read();
        if (letra != '*' && swstr == false )
         {fraseInt += letra ;
          frasechar+= letra ;         
         }
       else
         { 
          if (letra == '*' )
            {swstr = true ; }
          else
            { fraseStr+= letra ;
            }  
         }  
      } 
   }

 Serial.print("Recibido :");
 Serial.print(fraseInt);
 Serial.print("/");   
 Serial.println(fraseStr);  

   if (fraseInt.length()> 0 )
     {
      fraseStrVal =  fraseInt.toInt() ; 
      // ON / Agitar agua
      if (fraseStr=="on")    
        {
         if (swTgCier == false)
           {
            PuertoDos.print("Tc"); 
            mensaAnt = "Tc" ;            
            swTgCier = true ;
           }

         digitalWrite(bomba2, LOW) ;   // Prende bomba2 aguitado del agua (8)
         delay(45000) ;                // 45 Segundos
         digitalWrite(bomba2, HIGH);   // Apaga  bomba2 aguitado del agua (8)

         DateTime now = rtc.now();
         anomov  = now.year()  ;
         mesmov  = now.month() ;
         diamov  = now.day()   ;
         horamov = now.hour()  ;
         minumov = now.minute();

         EEPROM.write(1, mesmov) ;  // Mes movimiento
         EEPROM.write(2, diamov) ;  // Dia movimiento
         EEPROM.write(6, horamov);  // Hora movimiento
         EEPROM.write(7, minumov);  // Minu movimiento  */

         swLimpieza   = false ;
         SwActTubAgua = false ;        
         
         vrLimpia = 0 ;
         EEPROM.write(8, vrLimpia);  // Ya se hiso la limpieza de los residuos
         swLimpieza = false ;
        }  

      // OFF - Agitar agua
      if (fraseStr=="of")    // Dispensador Off
        {
         digitalWrite(bomba2, HIGH);   // Apaga bomba2 aguitado del agua (8)
        }
        
      if (fraseStr=="vo")    // Desocupar tanque Gral. ON
        {
         desocupaTanque() ;
        }  

      if (fraseStr=="vf")    // Desocupar taque Gral. OFF
        { 
         if (swTgCier == false)
           {
            PuertoDos.print("Tc");              
            mensaAnt = "Tc" ;            
            swTgCier = true ;
           }
         digitalWrite(bomba2, HIGH) ;      // Apaga bomba2 aguitado del agua (8)
        }  

      if (fraseStr=="lr")    // Dispensador Off
        {if(fraseInt=="No")
            {// DesActivar limpieza
             vrLimpia=0 ;
             EEPROM.write(8, vrLimpia) ;   
             swLimpieza = false ;
            }
        }
        
      if (fraseStr=="lr")    // Dispensador On
        {if(fraseInt=="Si")
            {// Activar limpieza
             vrLimpia=1 ;
             EEPROM.write(8, vrLimpia) ;   
             swLimpieza = true ;
            }  
        }
  
      ////  ----------------------------------  Fecha movimiento -----------
      fraseStrVal =  fraseInt.toInt() ; 
      if (fraseStr=="am")
        { EEPROM.write(0, fraseStrVal); 
          anomov = fraseStrVal ;  }
    
      if (fraseStr=="dm")
        { EEPROM.write(2, fraseStrVal); 
          diamov = fraseStrVal ; }
   
      if (fraseStr=="hm")
        { EEPROM.write(6, fraseStrVal);
          horamov = fraseStrVal ; 
        }
          
      if (fraseStr=="zm")
        { EEPROM.write(7, fraseStrVal);  
          minumov = fraseStrVal ; }

      if (fraseStr=="mm")
           {
            if (fraseInt=="Ene")
               { mesmov = 1 ; }
            else
               {
                if (fraseInt=="Feb")
                  { mesmov = 2 ; }
                else
                  {
                   if (fraseInt=="Mar")
                     { mesmov = 3 ; }
                   else
                     {
                      if (fraseInt=="Abr")
                        { mesmov = 4 ; }
                      else
                        {
                         if (fraseInt=="May")
                           { mesmov = 5 ; }
                         else
                           {
                            if (fraseInt=="Jun")
                               { mesmov = 6 ; }
                            else
                              {
                               if (fraseInt=="Jul")
                                 { mesmov = 7 ; }
                               else
                                 {
                                  if (fraseInt=="Ago")
                                    { mesmov = 8 ; }
                                  else
                                    {
                                     if (fraseInt=="Sep")
                                       { mesmov = 9 ; }
                                     else
                                       {
                                        if (fraseInt=="Oct")
                                          { mesmov = 10 ; }
                                        else
                                           {
                                            if (fraseInt=="Nov")
                                              { mesmov = 11 ; }
                                            else
                                              {
                                               mesmov = 12 ; 
                                              }  
                                           }  
                                       }  
                                    } 
                                 }  
                               }     
                           }  
                        }  
                     }  
                  } 
               }
            EEPROM.write(1,mesmov) ;  // Mes movimiento
           } // fin mes 
       delay(1000) ;

      /////////  --------------------- Fecha actual ------------------------------
      fraseStrVal =  fraseInt.toInt() ; 
       if (fraseStr=="da")
        {
          diarel = fraseStrVal ;
          diarel = fraseStrVal ;

          DateTime now = rtc.now();
          rtc.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
        }
      else
        {if (fraseStr=="me")
           {if (fraseInt=="Ene")
               { mesrel = 1 ; }
            else
               {if (fraseInt=="Feb")
                  { mesrel = 2 ; }
                else
                  {if (fraseInt=="Mar")
                     { mesrel = 3 ; }
                   else
                     {if (fraseInt=="Abr")
                        { mesrel = 4 ; }
                      else
                        {if (fraseInt=="May")
                           { mesrel = 5 ; }
                         else
                           {if (fraseInt=="Jun")
                               { mesrel = 6 ; }
                            else
                              {if (fraseInt=="Jul")
                                 { mesrel = 7 ; }
                               else
                                 {if (fraseInt=="Ago")
                                    { mesrel = 8 ; }
                                  else
                                    {if (fraseInt=="Sep")
                                       { mesrel = 9 ; }
                                     else
                                       {if (fraseInt=="Oct")
                                          { mesrel = 10 ; }
                                        else
                                           {if (fraseInt=="Nov")
                                              { mesrel = 11 ; }
                                            else
                                              { mesrel = 12 ; }  
                                           }  
                                       }  
                                    } 
                                 }  
                               }     
                           }  
                        }  
                     }  
                  } 
               }
            DateTime now = rtc.now();
            rtc.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
            // fin mes         
           }
        else
           {if (fraseStr=="aa")
               {
                anorel = fraseStrVal ;
                DateTime now = rtc.now();
                rtc.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
               }
            else
               {if (fraseStr=="ha")
                 {hrel = fraseStrVal ; }
               else
                 {if (fraseStr=="ma")
                    {mrel = fraseStrVal ; }
                 }  
               DateTime now = rtc.now();
               rtc.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
               }
           }
       }
       DateTime now = rtc.now();
       rtc.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));  
  }
   fraseInt = "" ;
   fraseStr = "" ;
   frase    = "" ;
}

void AguaTanqueLim () {
}

void limpieza(){
}

void mezclaQuimico ()
 {
  //Serial.print("Valor de vrLimpia es.. : ") ;    
  //Serial.println(vrLimpia) ;    
  if (vrLimpia == 1)
    {}
  else
    {
     Serial.println(" Mezclando quimico "); 
     if (swTgCier == false)
       {
        PuertoDos.print("Tc"); 
        mensaAnt = "Tc" ;
        swTgCier = true ;
       }
     digitalWrite(bomba2, HIGH)      ; // Apaga bomba2 aguitado del agua (8)

     //Activar limpieza        
     vrLimpia = 1 ;
     EEPROM.write(8, vrLimpia);  // Minutos movimiento
     swLimpieza = true ;

     int horaMezcla = 0 ;
     if (swMezclador == true )
       {

        // hoy07oct
        // ajita el agua con el quimico 
        digitalWrite(bomba3, LOW) ;  // Prende Bomba3 ajitado del quimico(9) 
        delay(15000) ;                
        digitalWrite(bomba3, HIGH) ; // Apaga  Bomba3 ajitado del quimico(9)  

        //  ti debe ser igual a for de ti
        if (ti<4000)         
          {
           digitalWrite(bomba4, LOW) ;  // Prende Bomba4 Alimentador de los quimicos con el agua(11)
           // hoy11ago
           delay(8000) ;                // 8000 Segundos / 100 ml.
                                        // 6000 Segundos /  40 ml. 
                                        // 5000 Segundos /  32 ml.
           digitalWrite(bomba4, HIGH) ; // Apaga Bomba4 Alimentador de los quimicos con el agua(11)
          } 

        digitalWrite(bomba2, LOW) ; // Prende bomba2 aguitado del agua (8)
                                    // 30000 Equivale a 1,03 Seg
                                    // 20000 Equivale a    Seg
                                    // 10000 Equivale a 15 Seg
        for (ti=0; ti<=20000; ti++)               
           {
            Serial.print(" Esto es ti.... : "); 
            Serial.println(ti);             
        }    
            
        horaMezcla = 0 ;
        Mezclar=0;
        EEPROM.write(9, Mezclar);  // Opcion de mezclar o aguitar  */
        swMezclador = false ;
        digitalWrite(bomba2, HIGH) ; // Apaga bomba2 aguitado del agua (8)
       }
     else
       { 
        digitalWrite(bomba2, HIGH) ; // Apaga bomba2 aguitado del agua (8)
        swMsM1 = false ;
       }
  }   
}

void llenaTqLimpieza ()
{
  //Serial.println("llena tanque de Limpieza") ; 
  vr_LLenoTanque  = digitalRead(PinTanque) ; 
  
  if (swTgCier == false)
    {
     PuertoDos.print("Tc"); 
     mensaAnt = "Tc" ;
     swTgCier = true ;
    }
  swlleno = true ;  
}

void llenaPileta() {
    //Serial.print("-- Consecutivo llena pileta..: ") ; 
    //Serial.println(hto) ; 
    vr_LLenoTanque  = digitalRead(PinTanque) ; 
    if ( vr_LLenoTanque == LOW )
      {
       Serial.println("-- Tq-Gral limpieza con agua -- ") ; 
       if (swLLenando == false)
         {
          Serial.println("-- Eliminando residuos Iniciales --") ; 
          //Antes de comenzar a llenar la pileta, elimina los primeros residuos

          if (swTgCier == false)
            {
             PuertoDos.print("Tc"); 
             mensaAnt = "Tc" ;
             swTgCier = true ;
            }
          digitalWrite(bomba2, HIGH);       // Apaga bomba2 aguitado del agua (8)

          //hoy17ago 
          //Serial.println("Abro la Valvula-Residuos ");    
          delay(500) ;    
          //PuertoDos.print("Ri"); 
          //mensaAnt = "Ri" ;
          //delay(10000) ;  // Espera 10 seg. a que la eliminacion de residuos iniciales termine
          
          PuertoDos.print("To") ; // Abre el Tanque General
          mensaAnt = "To" ;
          swTgCier = false ;

          digitalWrite(bomba2,      HIGH) ; // Apaga bomba2 aguitado del agua (8)
          delay(5000) ;
          swLLenando = true ;
         }
       else
         {
         }  
      }
    else
      {
       //Serial.println("-- Tq-Gral limpieza vacio -- ") ; 
      }  
}

void totalMinutos () {
  AcuAno    = 0 ;
  AcuMes    = 0 ;
  Acudia    = 0 ;
  AcuHora   = 0 ;
  AcuMin    = 0 ;
  tgMinutos = 0 ;

  tanorel = anorel ;
  tmesrel = mesrel ;
  tdiarel = diarel ;
  thorarel= hrel   ;
  tMinrel = mrel   ;

  if (tMinrel >= minumov )               // Minutos
    { AcuMin = tMinrel - minumov ; }
  else
    { AcuMin   = 60 + tMinrel - minumov ;
      thorarel = thorarel-1 ;
    }  

  if (thorarel >= horamov )             // Horas
    { AcuHora = thorarel-horamov ; }
  else
    { AcuHora = 24 + thorarel - horamov ;
      tdiarel = tdiarel - 1 ;
    }      
    
  if (tdiarel >= diamov )        // Dias
    { Acudia = tdiarel - diamov ; }
  else
    { 
     tmesrel = tmesrel - 1 ;
     if (tmesrel==1 || tmesrel==3 || tmesrel==5 || tmesrel==7 || tmesrel==8 || tmesrel==10 || tmesrel==12)
       { tdiarel = tdiarel + 31 ; }
     else
       { if (tmesrel==4 || tmesrel==6 || tmesrel==9 || tmesrel==11)
           {tdiarel = tdiarel + 30 ; }
         else
           { tdiarel = tdiarel + 28 ;}  
       }  
     Acudia = tdiarel - diamov ;
    }     
     
  if (tmesrel >= mesmov )    // Meses
    { AcuMes = tmesrel - mesmov ; }
  else
    { AcuMes = 12 + tmesrel - mesmov ;
      tanorel = tanorel - 1 ; }
    tgMinutos = AcuMin + (AcuHora*60) + (Acudia*24*60) + (AcuMes*30*24*60) ;
  }

void desocupaTanque () {
  //No colocar  
  //if (swTgCier == false)
  //  {
  //   PuertoDos.print("Tc"); 
  //   mensaAnt = "Tc" ;
  //   swTgCier = true ;
  //   //digitalWrite(Elec_bomba4, HIGH) ; // Apaga Elec_Bomba4 Alimentacion de la pileta (10)
  //  }

  // Cierra primero la valvula que llena la pileta, antes de comenzar residuos
  PuertoDos.print("Tc"); 
  mensaAnt = "Tc" ;
  swTgCier = true ;
  delay(3000) ;               // 3 Segundos

  digitalWrite(bomba2, HIGH);       // Apaga  bomba2 aguitado del agua (8)
  digitalWrite(bomba2, LOW);        // Prende bomba2 aguitado del agua (8)

  //Serial.println("Abro la Valvula-Residuos ");        
  PuertoDos.print("Re"); 
  mensaAnt = "Re" ;

    delay(38000) ;               // 35 Segundos
  digitalWrite(bomba2, HIGH);  // Apaga bomba2 aguitado del agua (8)
}