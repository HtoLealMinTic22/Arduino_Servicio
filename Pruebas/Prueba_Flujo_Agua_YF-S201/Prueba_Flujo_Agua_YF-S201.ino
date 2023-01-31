// Esta opcion recoje toda el agua desde el tanque auxiliar, independientemente del ciclo
// siempre llena desde el auxiliar hasta terminar el agua luego se abastece del acueducto
// en cualquier ciclo que este funciona igual

// (bomba1, LOW);  // Apaga  Bomba1 LLenado desde el tanque auxiliar(10)
// (bomba1, HIGH); // Prende Bomba1 LLenado desde el tanque auxiliar(10)
// (bomba2, HIGH); // Apaga  Bomba2 Electro-Valvula Llenado desde el acueducto(11)
// (bomba2, LOW);  // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(11)

// Serial.print, no funciona

#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Wire.h>

volatile int Echo = A0 ;
volatile int Trig = A1 ;
volatile int distancia ;
volatile int duracion ;
volatile int espera ;

//volatile long duracion ;

LiquidCrystal lcd(9, 8, 7, 6, 5, 4) ;
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
  Serial.begin(115200); 
   
  rpmcont=0;
  attachInterrupt(0, rpm, RISING);
}


void loop()
{
  rpmcont=0;
  sei();
  delay(1000);
  cli();
  contVeces++;
  Calc = (rpmcont * 60 / 350) ;

  Serial.print("L/min : ");
  Serial.print(Calc, DEC);
  Serial.print(" Rpm : ");
  Serial.println(rpmcont);

  Serial.print("L/min : ");
  Serial.print(Calc, DEC);
  Serial.print(" Rpm : ");
  Serial.println(rpmcont);
}
