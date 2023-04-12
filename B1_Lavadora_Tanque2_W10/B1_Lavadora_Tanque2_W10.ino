// Este codigo se utiliza solo para el proceso de limpieza del del agua, recibe el agua de la 
// lavadora y la limpia.
// Abr/18/2022 - May/05/2022 - Se instalo el motor de Paso, para eliminar residuos
// Jun/08/2022 - Se instalo PWM, para llenar tanque auxiliar 
// Jun/13/2022 - Se cambio el pin bomba1(11) a PinPWM(10)
// Dic/16/2022 - Se implemento el calculo de minutos desde la ultima lavada
// Ene/31/2023 - Se instalo en Github
// Abr/12/2023 - Se instalo en la tarjeta 
// Abr/12/2023 - Prueba dos
  

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Conecxion entre las dos tarjetas Arduino
// Tx-12 Trasmite del Celular a Arduino, Rx-13 trasmite de Arduino a Celular 
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Servo.h>
#include <EEPROM.h>
#include <RTClib.h>

SoftwareSerial PuertoUno(12,13);    // Tx-12 Trasmite del Celular a Arduino, Rx-13 trasmite de Arduino a Celular 
RTC_DS1307 RTC;

LiquidCrystal_I2C lcd(0x27,20,4); 

#define PinTanque 2  // Pin 2 LLenado del tanque General
#define Pecho1 4
#define Ptrig1 5
#define PinLLenoAux6 6
#define sensor_PasoAgua 7  // Sensor de tubo con paso de agua

#define dirPinV1        A0 // Direccion Valvula uno
#define stepPinV1       A1 // Step Valvula uno
//#define PWM           A2 // PWM
const int POT         = A2  ;// Potenciometro
#define EncienPaso      3  // Switch encendido Driver 4988 - eliminacion de reciduos

#define PasoXRevV1     650 // Numero de vueltas

volatile int       veloPaso = 1500  ; // Velocidad motor paso 
volatile boolean swEnciPaso = false ;
volatile boolean swTotalMin = false ;
int EstValUno ;

char buffer[70]    ;
char vector[15] ;
int indVector ;
volatile boolean swstr = false ;

// PinTanque       2 - LLenado del tanque General
// EncienPaso      3 - Switch encendido Driver 4988 - eliminacion de reciduos
// Pecho1          4 - Sensor distancia Echo
// Ptrig1          5 - Sensor distancia Triger
// PinLLenoAux6    6 - LLenado del tanque auxiliar 
// sensor_PasoAgua 7 - Sensor con paso de agua
// bomba4          8 - Mezclador de los quimicos y el agua(8)
// bomba2          9 - Aguitado del agua              
// PinPWM         10 - Pin PWM - Alimentacion de tanque auxiliar (10)
// bomba1         11 - Bomba1 Alimentacion de tanque auxiliar 
// PuertoUno      12 - Puerto (Tx-BlueTooh)
// PuertoUno      13 - Puerto (Rx_BlueTooh)
// dirPinV1   A0 - Valvula uno
// stepPinV1  A1 - Valvula uno
// PWMTq_Aux  A2 - PWM - PWM_Tq_Aux / PWM tanque auxiliar (A2)
// SDA        A4 - RELOJ DS 1307
// SCL        A5 - RELOJ DS 1307

String inputEnt ;
String frase ;
String fraseStr ;
char   frasechar ;
String fraseInt ;
int    fraseStrVal ; 

volatile int vr_PasoAgua ;
volatile int vr_LLenoAux ;
volatile int vr_LLenoTanque ;
volatile int anorel=0 ;
volatile int mesrel=0 ;
volatile int diarel=0 ;
volatile int hrel  =0 ;
volatile int mrel  =0 ;
volatile int CampoErr =0 ;
volatile int vr_TgLLeno =0 ;
volatile int ti =0 ;

volatile long anomov =0 ;
volatile int mesmov =0 ;
volatile int diamov =0 ;
volatile int horamov=0 ;
volatile int minumov=0 ; 
volatile int vrLimpia=0 ; 


volatile int bomba4  =  8 ;  // Bomba4 Mezclador de los quimicos y el agua(8)
volatile int bomba2  =  9 ;  // Bomba2 aguitado del agua                (9) // IN2 Ok
volatile int PinPWM  = 10 ;  // PinPWM Alimentacion de tanque auxiliar (10)
//volatile int bomba1  = 11 ;  // Bomba1 Alimentacion de tanque auxiliar (11) // IN4 Ok

volatile long duracion1, distancia1;   
volatile long duracion2, distancia2;   
volatile long porcellen1, porcellen2 ;   
volatile int  MsApagar = 0 ;

volatile boolean swlleno     = false ;
volatile boolean swLimpieza  = false ;
volatile boolean swMsM1      = false ; 
volatile boolean swMezclador = false ;
volatile boolean SwActTubAgua= true  ;

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

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  Serial.begin (9600);       // inicializa el puerto seria a 9600 baudios
  PuertoUno.begin(9600);
 
   // Reloj
   Wire.begin();
   RTC.begin();
   delay(100) ;
   //RTC.adjust(DateTime(2000, 1, 1, 3, 0, 0));
   //RTC.adjust(DateTime(__DATE__, __TIME__));
   //diamov= 18 ;
   //EEPROM.write(2, diamov) ;  // Dia movimiento

   // Activar limpieza
   // vrLimpia=1 ;
   //EEPROM.write(8, vrLimpia) ;   

   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
   //        EEPROM.read(9) ; // Motor de Paso 
 
   if (vrLimpia==0)
     {swLimpieza = false; }
   else
     {swLimpieza = true ; }  

   DateTime now = RTC.now();
   anorel = now.year()  ;
   mesrel = now.month() ;
   diarel = now.day()   ;
   hrel   = now.hour()  ;
   mrel   = now.minute();

   pinMode(Pecho1, INPUT);        // define el pin 6 como entrada (echo)
   pinMode(Ptrig1, OUTPUT);       // define el pin 7 como salida  (triger)
   pinMode(PinLLenoAux6, INPUT);  // define el pin 6 LLenado del tanque auxiliar 
   pinMode(PinTanque, INPUT);     // Pin 2 LLenado del tanque General

// pinMode(bomba1,OUTPUT);         // Bomba1   Alimentacion de tanque auxiliar (11)
   pinMode(PinPWM,OUTPUT);         // PinPWM - Alimentacion de tanque auxiliar (10)
   analogWrite(PinPWM,LOW);        // PinPWM - Alimentacion de tanque auxiliar (10)
   
   pinMode(bomba2,OUTPUT);         // Bomba2 aguitado del agua (8)
   pinMode(EncienPaso,OUTPUT);     // Switch encendido Driver 4988 - eliminacion de reciduos
   digitalWrite(EncienPaso, LOW);  // Apagado Switch encendido Driver 4988 - eliminacion de reciduos-Switch driver de Motor de Paso
     
   pinMode(bomba4,OUTPUT);         // Bomba4 Mezclador de los quimicos y el agua(10)
   pinMode(sensor_PasoAgua, INPUT) ;

   digitalWrite(bomba2, HIGH);     // Apaga bomba2 aguitado del agua (8)
   digitalWrite(bomba4, HIGH);     // Apaga bomba4 Mezclador de los quimicos y el agua(11)
   swMezclador= true ;

   pinMode(stepPinV1,OUTPUT);
   pinMode(dirPinV1, OUTPUT);

}


void loop()
{
  
   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
 
/*
// --------------------------------
   anomov  =  2012;  
   mesmov  =  12;  
   diamov  =  15;  
   horamov =  23;  
   minumov =  30;  
// --------------------------------
*/

   // vrLimpia= 0;         //  Apuntador de lectura
   if (vrLimpia==0)
     {swLimpieza = false; } // No hace limpieza del tanque Gral
   else
     {swLimpieza = true ; } // Si hace limipieza del tanque Gral.

  serialEvent();

  vr_LLenoTanque  = digitalRead(PinTanque) ; 
  //vr_LLenoTanque  = 0 ; // LLeno LOW                      // P R O V I S I O N A L   // 
  //vr_LLenoTanque  = 1 ; // Vacio HIGH
  if (vr_LLenoTanque== LOW)  
    { Serial.print("  Tgral Con agua H2O ");  
    }
  else
    { Serial.print("  Tgral Sin Agua ");  
    }  
  
   vr_LLenoAux = digitalRead(PinLLenoAux6) ; 
   //vr_LLenoAux = 1 ;                             // P R O V I S I O N A L   //
   if (vr_LLenoAux != LOW)
      { Serial.print("  Aux-vacio  ");  
        //swlleno = false ;  
      }
   else
      { Serial.print("  Aux-Con Agua  ");   
        //swlleno = true ;  
      }    
   
  // Sensa la fecha del ultimo paso de agua al sistema
  // vr_PasoAgua = 1;
  // con agua - 0 LOW / sin agua - 1 HIGH
  vr_PasoAgua = digitalRead(sensor_PasoAgua) ;
  if (vr_PasoAgua == HIGH)
      { Serial.println("  Tubo sin agua = 1 ");  
      }
  else
      { 
        Serial.println("  Tubo con agua = 0  "); 
        if (SwActTubAgua==true)
          {
           DateTime now = RTC.now();
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
           swMezclador  = true ;
           SwActTubAgua = false ;        
        }
        
        // Activar limpieza        
        vrLimpia = 1 ;
        EEPROM.write(8, vrLimpia);  // No hace limipieza del tanque Gral.
        //swLimpieza = true ;
      }  


  DateTime now = RTC.now();
  anorel = now.year()  ;
  mesrel = now.month() ;
  diarel = now.day()   ;
  hrel   = now.hour()  ;
  mrel   = now.minute();


// ----------------
/*
  anorel = 2022 ;
  mesrel = 12;
  diarel = 16 ;
  hrel   = 11 ;
  mrel   = 30;
*/
// ----------------

 // hoy16dic
  totalMinutos ();


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

 //porcellen1 = 6  ;
 //MsApagar = 1 ;
  AguaTanqueLim () ;
  //sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",diarel,mesrel,anorel,hrel,mrel,diamov,mesmov,MsApagar,horamov,minumov,vrLimpia,vrLimpia,porcellen1) ;
    sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",diarel,mesrel,anorel,hrel,mrel,diamov,mesmov,MsApagar,horamov,minumov,vrLimpia,vrLimpia,porcellen1) ;
  
  PuertoUno.print(" ");    // Comandos Buffer
  PuertoUno.print(buffer); // Comandos Buffer

  Serial.print(" ");
  Serial.print(buffer);
  Serial.println("");

  delay(1000);
 
//  AguaTanqueLim () ;
  
  /*
  if (mesmov <= mesrel )
    { 
    */
      
 // hoy16dic
     if (tgMinutos >= 720)       // 12 Horas     
        //if (diarel>diamov)
        {
          if (swMezclador == false)
            {
             llenatanque () ;
            }
          else            
            {
             // tiene mas de 12 horas y no se ha hecho la mezcla con el químico
             DateTime now = RTC.now();
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
             swMezclador  = true ;
             SwActTubAgua = false ;    
            } 
        }
     else
        {
         if (tgMinutos >= 120)          
       //if (diarel=diamov)      
           {
            analogWrite(PinPWM,LOW);        // Apaga PinPWM - Alimentacion de tanque auxiliar (10)
            digitalWrite(bomba2, HIGH);     // Apaga bomba2 aguitado del agua (8)
            digitalWrite(EncienPaso, LOW);  // Apaga Switch encendido Driver 4988 - eliminacion de reciduos

            //swMezclador = 0 false / 1 Verdadero
            if (swMezclador == true )
              //if (tgMinutos>=120 && swMezclador == true )
              //if (hrel == horaMezcla && swMezclador == true )
              {
               //  ti debe ser igual a for de ti
                if (ti<20000)         
                 {
                  digitalWrite(bomba4, LOW) ;  // Prende bomba4 Mezclador de los quimicos y el agua(10) 
                  //  7500 1/4 Litro
                  // 15000 1/2 Litro
                  delay(20000) ;
                  digitalWrite(bomba4, HIGH) ; // Apaga bomba4 Mezclador de los quimicos y el agua(10)
                 } 
                
                digitalWrite(bomba2, LOW) ; // Prende bomba4 Mezclador de los quimicos y el agua(10)
               // 20000 - 3.20 Sg
               // 25000 - 4.15 Sg
               // 30000 - 5.10 Sg
               // 40000 - 6.40 Sg
               for (double ti=0; ti<=20000; ti++)               
                  {
                   //horaMezcla = 0 ;
                   //Serial.println(ti);  
                   swMezclador = false ;
                  }
               /// Suspeder mensaje MsApagar = 3 ;
               /// Suspeder mensaje swMsM1 = true ;
              }
            else
              {
              if (bomba2==LOW)
                {
                 digitalWrite(bomba2, HIGH) ; // Apaga bomba4 Mezclador de los quimicos y el agua(10)
                 swMsM1 = false ;
                }
              }  
           }
         else
           {
           }  
        } 
  /* } */
  /*
  else  
    {
    }  */
  
  if (vr_LLenoTanque == HIGH && swLimpieza == true )
    {
     limpieza() ;
     vrLimpia=0 ;
     EEPROM.write(8, vrLimpia) ;   
     swLimpieza = false ;
    }
}


void serialEvent(){
 Serial.print("  Puerto UNO ");
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


// Serial.print("Recibido :");
// Serial.print(fraseInt);
// Serial.print("/");   
// Serial.println(fraseStr);  

   if (fraseInt.length()> 0 )
     {
      fraseStrVal =  fraseInt.toInt() ; 
      if (fraseStr=="on")    // Dispensador Off
        {
         analogWrite(PinPWM,LOW);          // Apaga PinPWM - Alimentacion de tanque auxiliar (10)
         
         digitalWrite(EncienPaso, LOW) ;   // Apaga Switch encendido Driver 4988 - eliminacion de reciduos
         swEnciPaso = true ;
         
         // Paso Uno
         digitalWrite(bomba2, LOW) ;   // Prende bomba2 aguitado del agua (8)
         delay(15000) ;
         digitalWrite(bomba2, HIGH);   // Apaga  bomba2 aguitado del agua (8)

         /// Rutina tubo con agua 
         DateTime now = RTC.now();
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
      
      if (fraseStr=="of")    // Dispensador Off
        {
         digitalWrite(bomba2, HIGH);   // Apaga  bomba2 aguitado del agua (8)
        }
        
      if (fraseStr=="vo")    // Desocupar taque General - ON
        {
         analogWrite(PinPWM,LOW);          // Apaga PinPWM - Alimentacion de tanque auxiliar (10)
         digitalWrite(bomba2, HIGH);       // Apaga  bomba2 aguitado del agua (8)
         digitalWrite(bomba2, LOW) ;       // Prende bomba2 aguitado del agua (8)
         abreVal1();
         delay(245000) ; 
         digitalWrite(bomba2, HIGH);   // Apaga bomba2 aguitado del agua (8)
         cierraVal1();
        }  

      if (fraseStr=="vf")    // Desocupar taque General - OFF
        {
         analogWrite(PinPWM,LOW);      // Apaga PinPWM - Alimentacion de tanque auxiliar (10)
         digitalWrite(bomba2, HIGH);   // Apaga  bomba2 aguitado del agua (8)
        }  

      if (fraseStr=="lr")    // Dispensador Off
        {
          if(fraseInt=="No")
            {
             // DesActivar limpieza
             vrLimpia=0 ;
             EEPROM.write(8, vrLimpia) ;   
             swLimpieza = false ;
            }
        }
        
      if (fraseStr=="lr")    // Dispensador Off
        {
          if(fraseInt=="Si")
            {
             // DesActivar limpieza
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
        { 
          EEPROM.write(6, fraseStrVal);
          horamov = fraseStrVal ; 
        }
          
      if (fraseStr=="zm")
        { 
          EEPROM.write(7, fraseStrVal);  
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
           } // fin me 
       delay(1000) ;


      /////////  --------------------- Fecha actual ------------------------------
      fraseStrVal =  fraseInt.toInt() ; 
       if (fraseStr=="da")
        { 
          diarel = fraseStrVal ;
          DateTime now1 = RTC.now();
          RTC.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
        }
      else
        {
         if (fraseStr=="me")
           {
            if (fraseInt=="Ene")
               { mesrel = 1 ; }
            else
               {
                if (fraseInt=="Feb")
                  { mesrel = 2 ; }
                else
                  {
                   if (fraseInt=="Mar")
                     { mesrel = 3 ; }
                   else
                     {
                      if (fraseInt=="Abr")
                        { mesrel = 4 ; }
                      else
                        {
                         if (fraseInt=="May")
                           { mesrel = 5 ; }
                         else
                           {
                            if (fraseInt=="Jun")
                               { mesrel = 6 ; }
                            else
                              {
                               if (fraseInt=="Jul")
                                 { mesrel = 7 ; }
                               else
                                 {
                                  if (fraseInt=="Ago")
                                    { mesrel = 8 ; }
                                  else
                                    {
                                     if (fraseInt=="Sep")
                                       { mesrel = 9 ; }
                                     else
                                       {
                                        if (fraseInt=="Oct")
                                          { mesrel = 10 ; }
                                        else
                                           {
                                            if (fraseInt=="Nov")
                                              { mesrel = 11 ; }
                                            else
                                              {
                                               mesrel = 12 ; 
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
            DateTime now1 = RTC.now();
            RTC.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
           } // fin me         
        else
           {
            if (fraseStr=="aa")
               {
                anorel = fraseStrVal ;
                DateTime now1 = RTC.now();
                RTC.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
               }
            else
               {
               /////////////////////////
               if (fraseStr=="ha")
                 {
                  hrel = fraseStrVal ;
                  DateTime now1 = RTC.now();
                  RTC.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
                 }
               else
                 {
                  if (fraseStr=="ma")
                    {
                     mrel = fraseStrVal ;
                     DateTime now1 = RTC.now();
                     RTC.adjust(DateTime(anorel, mesrel, diarel, hrel, mrel, 0));
                    }
                 }  
               
               }
           }
        }
   }

   fraseInt = "" ;
   fraseStr = "" ;
   frase    = "" ;
}

void AguaTanqueLim () {
  digitalWrite(Ptrig1, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig1, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig1, LOW);
  
  duracion1  = pulseIn(Pecho1, HIGH);
  distancia1 = (duracion1/2) / 29;            // calcula la distancia en centimetros

  if(distancia1<=15)
    {porcellen1 = 4 ; }
  else      {
     if(distancia1<=30)
       {porcellen1 = 3 ;}
     else
       {
        if(distancia1<=45)
          {
           porcellen1 = 2 ; 
          }
        else
          {
           porcellen1 = 1 ;
          }  
       }  
    }  

//porcellen1 = 1  ;
//Serial.print(" Dist-TQgral ..: ");// envia el valor de la distancia por el puerto serial
//Serial.print(distancia1);         // envia el valor de la distancia por el puerto serial
//Serial.print(" cm");              // le coloca a la distancia los centimetros "cm"
}


void limpieza ()
{
  //swLimpieza= false;
  if (swLimpieza == false)
    {
      // Serial.print("No estoy limpiando el tanque Gral.");  
    }
  else
    {  
     //vrLimpia = 0 ;
     //EEPROM.write(8, vrLimpia);  // Ya se hiso la limpieza de los residuos
     //swLimpieza = false ;
 
     // Serial.print("Limpiando el tanque Gral.Vacio");  
     // Apaga PinPWM
     analogWrite(PinPWM,LOW);     // Apaga PinPWM - Alimentacion de tanque auxiliar (10)

     //Serial.print("Aguitando  Agua ") ;
     digitalWrite(bomba2, LOW) ;   // Prende bomba2 aguitado del agua (8)
     if (SwActTubAgua == false)
       {}
     else
       {
        Serial.print("Iniciando proceso de residuos");  
        abreVal1();
        delay(250000) ; 
        digitalWrite(bomba2, HIGH);   // Apaga bomba2 aguitado del agua (8)
        cierraVal1();
       }  

     //Serial.println("Termine la limpieza del tanque Gral.Vacio");  
     swMezclador  = true ;
     delay(2000) ;
     
     vrLimpia = 0 ;
     EEPROM.write(8, vrLimpia);  // Ya se hiso la limpieza de los residuos
     swLimpieza = false ;

     vrLimpia= EEPROM.read(8) ; // 
     if (vrLimpia==0)
       {
        swLimpieza = false; 
        //Serial.println("Actualice el estado de swLimpieza - Nooooooo");  
       } 
    else
       {
        swLimpieza = true ; 
       } 
    } 
}

void llenatanque () {
  AguaTanqueLim () ;
  vr_LLenoTanque  = digitalRead(PinTanque) ; 
  
  //Serial.print("Prende PWM - Alimenta tanque auxiliar");  
  //vr_LLenoTanque = 0  ;  // 0 LLeno-Low / 1 Vacio-High  -----  // P R O V I S I O N A L //
  //vr_LLenoAux    = 1   ; // 0 LLeno-Low / 1 Vacio-High  -----  // P R O V I S I O N A L //
  
  if ( vr_LLenoTanque== LOW  && vr_LLenoAux != LOW  )
    {
     // Depende del dato que le envia el potenciometro
     analogWrite(PinPWM, analogRead(POT/8)); // Prende PinPWM - Alimentacion de tanque auxiliar (10)
     
     digitalWrite(bomba2, HIGH); // Apaga bomba2 aguitado del agua (8)
     swLimpieza = true ;  
    }
  else
    {
     analogWrite(PinPWM,LOW);    // Apaga PinPWM - Alimentacion de tanque auxiliar (10)
     swlleno = true ;  
    }
}

void abreVal1() 
{ 
  EstValUno = EEPROM.read(9) ; // Valvula uno
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  EstValUno = 0 ;
  if (EstValUno == 0)
    {
     digitalWrite(EncienPaso, HIGH) ;  // Prende Switch encendido Driver 4988 - eliminacion de reciduos
     swEnciPaso = true ;
     
     // Esta cerrada la valvula uno 
     // LOW - horario / Abre la valvula uno
     digitalWrite(dirPinV1, LOW);
     for (int i = 0; i < PasoXRevV1; i++)
        {
         // These four lines result in 1 step:
         digitalWrite(stepPinV1, HIGH);
         delayMicroseconds(veloPaso);     // velocidad menor numero + velocidad
         digitalWrite(stepPinV1, LOW);
         delayMicroseconds(veloPaso);
        }
     EstValUno = 1 ;   
     EEPROM.write(9, EstValUno);    
     delay(1000);      

     EstValUno = EEPROM.read(9) ;     // Valvula uno
     digitalWrite(EncienPaso, LOW) ;  // Apaga Switch encendido Driver 4988 - eliminacion de reciduos
     swEnciPaso = false ;

     Serial.print("Fin-valvula ABRE valvula Uno .... : ");
     Serial.println(EstValUno);  
  }
}

void cierraVal1() 
{ 
  EstValUno = EEPROM.read(9) ; // Valvula uno
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  EstValUno = 1 ;
  if (EstValUno == 1)
    {
     digitalWrite(EncienPaso, HIGH) ;  // Prende Switch encendido Driver 4988 - eliminacion de reciduos
     swEnciPaso = true ;
      
     // Esta abierta la valvula uno 
     // HIGH - Horario / Cierra la valvula uno
     digitalWrite(dirPinV1, HIGH);
     for (int i = 0; i < PasoXRevV1; i++) {
       digitalWrite(stepPinV1, HIGH);
       delayMicroseconds(veloPaso);
       digitalWrite(stepPinV1, LOW);
       delayMicroseconds(veloPaso);
       }
     EstValUno = 0 ;   
     EEPROM.write(9, EstValUno);    
     delay(1000);  

     digitalWrite(EncienPaso, LOW) ;  // Apaga Switch encendido Driver 4988 - eliminacion de reciduos
     swEnciPaso = false ;
    }
}


void totalMinutos () {
if(swTotalMin == false)
  { swTotalMin= true;
    //Serial.print("  No ejecuta el calculo de minutos "); 
    //delay(5000);
    }
else
  {
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
  
    // Serial.print("  Total Minutos "); 
    // Serial.println(tgMinutos); 
  }
}


