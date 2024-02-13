// Este codigo se utiliza para suministrar el agua a la lavadora, recoje toda el agua limpia, 
// desde el tanque auxiliar, independientemente del ciclo.
// Siempre llena desde el auxiliar hasta terminar el agua luego se abastece del acueducto
// en cualquier ciclo que este funciona igual
// 29/Ene/2018 - ultima actualizacion
// Originado del A1_Lavadora_tanque2_3b del 29/01/2018
// 23/Ene/2022 - Se cambio la pantalla L.C.D. por LCD-I2C.
// 09/May/2022 -
// 13/Dic/2022 - Se cambia la distacia de llenado tanque auxiliar a 29  
// 14/Dic/2022 - Calcula el tiempo para llenar el tanque de agua limpia
// 31/Ene/2023 - Se Instalo en Github
// 12/Feb/2024 - Se actualizo

// (bomba1, LOW);  // Apaga  Bomba1 LLenado desde el tanque auxiliar(8)
// (bomba1, HIGH); // Prende Bomba1 LLenado desde el tanque auxiliar(8)
// (bomba2, HIGH); // Apaga  Bomba2 Electro-Valvula Llenado desde el acueducto(9)
// (bomba2, LOW);  // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)

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

volatile float caudal_L_m ;
float volumen=0;
volatile int NumPulsos;
float factor_conversion=7.11; //para convertir de frecuencia a caudal
long dt=0;                    //variación de tiempo por cada bucle
long t0=0;
volatile int rpmcont ;
volatile int Calc;
volatile int pin_sensor = 2 ;


// Pin caudal   =  2 ;
// bomba1       =  8 ; // Bomba1 LLenado desde el tanque auxiliar(8)
// bomba2       =  9 ; // Bomba2 Electro-Valvula Llenado desde el acueducto(9)
// Echo         = A0 ;
// Trig         = A1 ;
// Pantalla LCD = A4 ; // SDA 
// Pantalla LCD = A5 ; // SCL 

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

volatile int bomba1    = 8 ; // Bomba1 LLenado desde el tanque auxiliar(8)
volatile int bomba2    = 9 ; // Bomba2 Electro-Valvula Llenado desde el acueducto(9)

void rpm ()
{
 rpmcont++;
}

void ContarPulsos ()  
{ 
 NumPulsos++;               //incrementamos la variable de pulsos
} 

void setup()
{ 
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  delay(1000); 
  Serial.begin (115200);       // inicializa el puerto seria a 115200 baudios

  pinMode(Echo, INPUT) ;
  pinMode(Trig, OUTPUT) ;
  lcd.begin(16, 2) ;
  lcd.print("Ahorrando   Agua") ;
  delay(1000)  ; 
  lcd.clear();
  
  rpmcont=0;
  attachInterrupt(digitalPinToInterrupt(2), ContarPulsos, RISING); // (2) Conecta al Pin-2  / (3) Conecta al Pin-3 (Interupcion Externa)
  interrupts();                                                    // Solo una vez - solo los pines 2 y 3 hacen interupciones externas
    
  pinMode(bomba1,OUTPUT);    // Bomba1 LLenado desde el tanque auxiliar(8)
  pinMode(bomba2,OUTPUT);    // Bomba2 Electro-Valvula Llenado desde el acueducto(9)
}


void loop()
{
  lcd.begin(16, 2) ;
  if (swinicializar==0)
    {
     AguaTanqueAux () ;
     inicializar() ;  
     swinicializar=1 ; 
    }
 else
    {
     AguaTanqueAux () ;
    }

    calculoLts() ;

    lcd.setCursor(0,1) ;
    TiempoFin = TiempoFin + 1 ;
    if (TiempoFin <= 4300)
      {   
       Serial.print(" 18/may NumPulsos >= 4 es....:  ");
       Serial.println(NumPulsos);
      
       if (NumPulsos >= 3 )
        {
          if (SwInicio1 == true)        
           {
            // Swinicio (verdadero) 
            if (SwTanqAux == true) 
              {
                // Tanque auxiliar (verdadero)
                if (swbomba1HIGH == true )
                 {
                   //   Serial.println("LLenado tanque aux-MOVI  ");
                   digitalWrite(bomba1, HIGH); // Prende Bomba1 LLenado desde el tanque auxiliar(8)
                   digitalWrite(bomba2, LOW);  // Apaga  Bomba2 Electro-Valvula Llenado desde el acueducto(9)
                   
                   swbomba1HIGH = false ;
                   textoLcd = 'Ciclo Uno lavado' ;
                 }                
               else
                 {
                  //Serial.println(" h-43 ");
                  //lcd.print(" h43") ;               
                 } 
              }
            else
              {
               // Tanque auxiliar  (false)
               contVeces++ ;
               lcd.clear() ;                
               lcd.setCursor(0,1) ;
               lcd.print("Rp:") ;
               lcd.print(NumPulsos) ; 
               lcd.print(" Vb:") ;
               lcd.print(contVeces) ; 

               lcd.setCursor(11,1) ;
               lcd.print("h-82") ;
               Serial.println(" h82 ");

               digitalWrite(bomba2, HIGH); // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
               digitalWrite(bomba1, LOW) ; // Apaga  Bomba1 LLenado desde el tanque auxiliar(8)

                if (NumPulsos <= 5 )
                 {
                  lcd.print("h-84") ;
                  lcd.print("Acu-84 Off") ;
                  digitalWrite(bomba2, LOW); // Apaga Bomba2 Electro-Valvula Llenado desde el acueducto(9)
                  Ciclo = 2 ;
                 }
              }  
         }
         else
           {
            // Swinicio (false) 
            SwInicio1 = true  ;
            lleno = 1 ;
           }  
        }  // fin rpm >= 12 (verdadero)
      else
        {
         // rpm >= 12 (false)
             if (SwTanqAux == true) 
               {
                if (distancia <= 29 )
                  {
                   digitalWrite(bomba1, LOW) ; // Apaga Bomba1 LLenado desde el tanque auxiliar(8)
                   digitalWrite(bomba2, HIGH); // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
                   swbomba1HIGH = true ;  
                 }
                else
                  {
                   digitalWrite(bomba1, LOW) ; // Apaga Bomba1 LLenado desde el tanque auxiliar(8)
                   digitalWrite(bomba2, HIGH); // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
                   SwTanqAux = false ;
                  }  
               }
             else
               {
                digitalWrite(bomba2, HIGH); // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
                digitalWrite(bomba1, LOW) ; // Apaga  Bomba1 LLenado desde el tanque auxiliar(8)
               }  
         // fin rpm >= 12 (false)
        }     
      }
    else
      {
       lcd.clear() ;
       lcd.setCursor(0,0) ;
       lcd.print("    Lavado     ") ;
       lcd.setCursor(0,1) ;
       lcd.print("<--Terminado -->") ;

       digitalWrite(bomba1, LOW); // Apaga Bomba1 LLenado desde el tanque auxiliar(8)
       digitalWrite(bomba2, LOW); // Apaga Bomba2 Electro-Valvula Llenado desde el acueducto(9)
      }
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

  lcd.setCursor(2,0) ;
  if (NumPulsos > 0)
    { 
      lcd.print("- LLenando -") ;
      lcd.setCursor(0,1) ;
      lcd.print("Vel.:") ;
      lcd.print(NumPulsos) ; 
      lcd.setCursor(9,1) ;
      lcd.print("Dist:") ;
      lcd.print(distancia) ;
    }
  else
    { 
      if (TiempoFin < 4500)
        {
         lcd.setCursor(0,0) ;
         lcd.print("Espere Por favor") ; 
        
         lcd.setCursor(9,1) ;
         lcd.print("Dist:") ;
         lcd.print(distancia) ;
        }
      else
        {
         lcd.clear() ;
         lcd.setCursor(0,0) ;
         lcd.print("    Lavado     ") ;
         lcd.setCursor(0,1) ;
         lcd.print("<--Terminado -->") ;
        }
    }  
 }

void inicializar ()
{
  digitalWrite(bomba2, LOW);  // Apaga Bomba2 Electro-Valvula Llenado desde el acueducto(9)
  lcd.setCursor(0,0) ;
  lcd.print("         ") ;

  AguaTanqueAux () ;
  if (distancia <= 29 )
   {
    if (swbomba1HIGH == true )
      { 
       //Serial.println("  LLenado del tanque auxiliar  ");
       // LLena desde tanque auxiliar
       lcd.setCursor(0,0) ;
       lcd.print("LLena de Taq_Aux.") ;
 
       digitalWrite(bomba1, HIGH); // Prende Bomba1 LLenado desde el tanque auxiliar(8)
       digitalWrite(bomba2, LOW) ; // Apaga Bomba2 Electro-Valvula Llenado desde el acueducto(9)
             
       SwTanqAux    = true ;
       swbomba1HIGH = false ;
      }
   }
  else
   {// LLena desde el acueducto
    // Ok 14Dic22 - distancia >= 33
    if (distancia >= 33 && distancia <=35 && SwAjitado == false)
      {
       digitalWrite(bomba2, HIGH);  // Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
       limpieza() ;
      }  
     
    if (swbomba2HIGH == true )
      { 
       //Serial.println("      LLenado del acueducto  ");                
       lcd.setCursor(0,0) ;
       lcd.print("LLena de Acueduc.") ;
 
       digitalWrite(bomba1, LOW) ; //  Apaga Bomba1 LLenado desde el tanque auxiliar(8)
       digitalWrite(bomba2, HIGH); //  Prende Bomba2 Electro-Valvula Llenado desde el acueducto(9)
       SwTanqAux = false ;

       swbomba1HIGH = true ;
       swbomba2HIGH = false ;
       delay(2500)  ;
      }
   } // Fin llena desde el acueducto

   Serial.println("  Hola aqui estoy   ");

   if (NumPulsos <= 10 )
     {
      Serial.println("  Pulso <= 10   ");
      lcd.print("Inicio  LLenado ") ;  
      delay(15000)  ;
     }

   swbomba1HIGH = true ;
   swbomba2HIGH = false ;
 
   lcd.clear() ;
   lcd.setCursor(3,1) ;
   lcd.print("Listo - Ok ") ;
 
   Serial.println("Termine proceso de Inicio ");
   delay(2500)  ;
}

// ********************************************
void manejadoraTimer() 
{ //Sin parametros
   actualizar=true;
   segundos++;
   if (segundos==60){
        minutos++ ;
        segundos=0;
        if (minutos==60){
            horas++;  
            minutos =0;
            segundos=0;
        }
   }
}

// ********************************************
void limpieza ()
{
}

// ********************************************
void limpieza1 ()
{
}

// ********************************************
void apagaTodos ()
{
 digitalWrite(bomba1, LOW); // Apaga Bomba1 LLenado desde el tanque auxiliar(8)
 digitalWrite(bomba2, LOW); // Apaga Bomba2 Electro-Valvula Llenado desde el acueducto(9)
}

// ******************************************** CicloDos
void CicloDos()
{
}


// ********************************************
void calculoLts() {
  Serial.print(" 18/may calculoLts  ");    
  float frecuencia = ObtenerFrecuecia();       //obtenemos la frecuencia de los pulsos en Hz
  caudal_L_m = frecuencia/factor_conversion;   //calculamos el caudal en L/m
  dt=millis()-t0;                              //calculamos la variación de tiempo
  t0=millis();

  //Serial.println(" 18/may calculoLts  ");
  //volumen=volumen+(caudal_L_m/60)*(dt/500);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/900);     // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1100);  // volumen(L)=caudal(L/s)*tiempo(s)
  //volumen=volumen+(caudal_L_m/60)*(dt/1050);  // volumen(L)=caudal(L/s)*tiempo(s)
                       // Entre mayor sea el numero disminuye el volumen
 
  //  lcd.setCursor(8,0) ;
  //  lcd.print(volumen) ;  
  //  lcd.print(" Lts") ;  

  //    lcd.setCursor(0,1) ;
  //    lcd.print("Pul3:               ") ;
  //    lcd.print(NumPulsos,DEC) ; 

  //  lcd.setCursor(0,1) ;
  //  lcd.print("rpm:") ;
  //  lcd.print(volumen,DEC) ; 
}

int ObtenerFrecuecia() 
{
  int frecuencia;
  NumPulsos = 0;        // Ponemos a 0 el número de pulsos
  // interrupts();      // Bloquea la LCD (no colocar)- Habilitamos las interrupciones
  delay(1000);          // muestra de 1 segundo
  //noInterrupts();   // Bloquea la LCD (no colocar)- Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; // Hz(pulsos por segundo)
  return frecuencia;
}
