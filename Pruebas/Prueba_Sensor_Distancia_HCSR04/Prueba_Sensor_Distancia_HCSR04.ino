// Esta opcion recoje toda el agua desde el tanque auxiliar de agua limpiada, independientemente del ciclo
// siempre llena desde el auxiliar hasta terminar el agua luego se abastece del acueducto
// en cualquier ciclo que este funciona igual
// 29/Ene/2018 - ultima actualizacion
// Originado del A1_Lavadora_tanque2_3b del 29/01/2018
// 23/Ene/2022 - Se cambio la pantalla L.C.D. por LCD-I2C.


// (bomba1, LOW);  // Apaga  Bomba1 LLenado desde el tanque auxiliar(10)
// (bomba1, HIGH); // Prende Bomba1 LLenado desde el tanque auxiliar(10)
// (bomba2, HIGH); // Apaga  Bomba2 Electro-Valvula Llenado desde el acueducto(11)
// (bomba2, LOW);  // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(11)

// Serial.print, no funciona

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <RTClib.h>


volatile int Echo = A0 ;
volatile int Trig = A1 ;

volatile int distancia ;
volatile int duracion ;
volatile int espera ;

LiquidCrystal_I2C lcd(0x27,16,2);
//LiquidCrystal lcd(9, 8, 7, 6, 5, 4) ;
//LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

volatile int rpmcont ;
volatile int Calc;
volatile int pin_sensor = 2 ;

volatile double  TiempoFin = 0 ;
volatile int     Ciclo = 1 ;
volatile boolean SwInicio1 = false ;

volatile boolean swbomba1HIGH = true ;
volatile boolean swbomba2HIGH = true ;

volatile boolean SwAjitado = false ;
volatile boolean SwTanqAux = true  ;
volatile boolean swLimpia  = false ;

volatile int lleno   = 0 ; 
volatile boolean actualizar = true ;
volatile char textoLcd;
volatile int swinicializar= 0 ;

volatile int horas  = 0 ; // variable dentro y fuera de una interupcion
volatile int minutos= 0 ;
volatile int segundos=0 ; 

volatile int contVeces=0 ; 
volatile int espera2=0 ; 

volatile boolean InicioRele = false ;
volatile boolean CicloLavado1 = true ;
volatile boolean CicloLavado2 = false ;

volatile int vel      = 1000; 
volatile int tiempo   = 80  ;
volatile int i        = 0   ;

int valorPulso3    ;
int pulsador3 = 3  ; // Tanque auxiiar (3) 

volatile int bomba1    = 10 ; // Bomba1 LLenado desde el tanque auxiliar(10)
volatile int bomba2    = 11 ; // Bomba2 Electro-Valvula Llenado desde el acueducto(11)

void rpm ()
{
 rpmcont++;
}


void setup()
{ 
  Serial.begin (9600); 
  pinMode(Echo, INPUT) ;
  pinMode(Trig, OUTPUT) ;   
  
//  lcd.init();                      // initialize the lcd 
//  lcd.backlight();
//  delay(1000); 
//
//  lcd.begin(16, 2) ;
//  lcd.print("Ahorrando   Agua") ;
//  delay(1000)  ; 
//  lcd.clear();
//  rpmcont=0;
//  attachInterrupt(0, rpm, RISING);
//   
//  pinMode(bomba1,OUTPUT);    // Bomba1 LLenado desde el tanque auxiliar(10)
//  pinMode(bomba2,OUTPUT);    // Bomba2 Electro-Valvula Llenado desde el acueducto(11)
}


void loop()
{
  Serial.print("  Distancia ..:  ") ;
  Serial.println(distancia) ;
  AguaTanqueAux() ;
    
//  lcd.begin(16, 2) ;
//  lcd.clear();
//  lcd.setCursor(0,0) ;
//  lcd.print("Distancia..:") ;
//  lcd.print(distancia) ;
}

void AguaTanqueAux ()
{
  // Hace un disparo de sonido limpio
  digitalWrite(Trig, LOW)  ;
  delayMicroseconds(4) ;
  digitalWrite(Trig, HIGH)  ;
  delayMicroseconds(10) ;
  digitalWrite(Trig, HIGH)  ;
  duracion  = pulseIn(Echo, HIGH) ;
  duracion  = duracion/2 ;
  distancia = duracion/29 ;
  delay(50);

  //  lcd.setCursor(1,1) ;
  //  lcd.print("Dist:") ;
  //  lcd.print(distancia) ;
}
