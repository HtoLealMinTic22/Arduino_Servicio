// Activa la fecha de inicio de proceso solo cuando la cantidad de agua que entra al 
// recipiente inicial y el tiempo es diferente de (0), eliminando asi el FeeBack de agua por 
// reciduos pequeños de agua despues de terminar el baño.

// Se conecta a otro Arduino para manupular las valvulas 
// Arduinos Feb/10/20, Feb/26/20
// Emisor   (Viene de A0 entra a A1) (Invertido)
// Receptor (Viene de A1 entra a A2) (Invertido)

// El positivo que alimenFsin aguata los motores de paso se energizan por el Pin-13
// Se suspende la bomba auxiliar para ingresar agua de otro tanque
// 14/Ene/2021 - Cambia la velocidad cuando se trata de residuos iniciales 
// May/25/2021 - Se cambio nuevamente la velocidad de apertura y cierre Valvula tres - Tq-Gral. 
// Jun/21/2021 - Se cambio el giro del motor Paso a paso, de la opcion reciclar o no. 

// Sep/15/2021 - Se cambio la PCB, incluyendo un Driver adicional para el llenado de la pileta
// Oct/06/2021 - Se continua la instalacion de la nueva PCB
// Dic/13/2021 - Se cambio la velocidad de apertura y cierre Valvula tres - Tq-Gral.
// Ago/17/2022 - Se suspendio el proceso de residuos iniciales (Resini)
// Ene/05/2023 - Se cambio la Bomba2 al al IN3 del rele, se daño el IN2
// Ene/16/2023 - Se definio el EncienPaso en el Pin13 
// Ene/20/2023 - Se modifican los valores que se envian a la tarjeta 
//              - To-(Tanque General Open) 
//              - Tc-(Tanque General Cierra) 
//              - Si-(Si recicla) 
//              - Nr-(No recicla) 
//              - Re-(Abre Residuos ) 
//              - Ri-(Residuos Iniciales) 
// Ene/31/2023  - Se instala en Github


// se    bloquea - veloPaso =  500 ;
// se    bloquea - veloPaso =  650 ;
// no se bloquea - veloPaso =  800 ;
//                 veloPaso = 2000 ;

//pinLibre  =  2  // Libre
//pulsador1 =  3  // Agua en Pileta
//pulsador2 =  4  // Agua en la Sisterna
//stepPinV3 =  5  // Valvula tres - Tq-Gral.
//dirPinV3  =  6  // Valvula tres - Tq-Gral.
//Libre        7  // 
//bomba1    =  8  // Bomba1 - Pileta
//bomba2    = 10  // Bomba2 - acueducto
//bomba3    =  9  // Libre
//bomba4    = 11  // MotoReductor - ajita el agua de la pileta para la limpieza
//pulsador4 = 12  // Boton para limpieza de la pileta
//EncienPaso= 13  // Encendido Driver 4988 (motor Paso a Paso)
//
//SoftwareSerial PuertoUno A0
//SoftwareSerial PuertoUno A1 
//dirPinV1   A2 // Valvula uno - Reciclar
//stepPinV1  A3 // Valvula uno - Reciclar
//dirPinV2   A4 // Valvula dos - Residuos
//stepPinV2  A5 // Valvula dos - Residuos
//stepPinV3   5 // Valvula tres - Tq-Gral.
//dirPinV3    6 // Valvula tres - Tq-Gral.


#include <Servo.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define dirPinV1        A2 // Valvula uno - Reciclar
#define stepPinV1       A3 // Valvula uno - Reciclar
#define PasoXRevV1     650 // Numero de vueltas  

#define dirPinV2        A4 // Valvula dos - Residuos
#define stepPinV2       A5 // Valvula dos - Residuos
#define PasoXRevV2     300 // Numero de vueltas valvula dos 

#define dirPinV3         6 // Valvula tres - Tq-Gral.
#define stepPinV3        5 // Valvula tres - Tq-Gral.
#define PasoXRevV3     250 // Numero de vueltas valvula tres

volatile int         veloPaso = 2000  ; // Velocidad motor paso - mayor numero menos velocidad
//volatile int       veloPaso = 1500  ; // Velocidad motor paso - mayor numero menos velocidad
volatile boolean swEnciPaso = false ;

SoftwareSerial PuertoUno(A0,A1);  // recibe datos de la tarjeta-Ducha

String Mensaje ;
String MensajeEnt ;

int valorPulso1 ;
int valorPulso2 ;
int valorPulso3 ;
int valorPulso4 ;    // Bomba ajita el agua de la pileta para la limpieza

int pulsador1 = 3 ;  // Agua en Pileta
int pulsador2 = 4 ;  // Agua en la Sisterna

int bomba1    =  8 ; // Bomba1 - Pileta
int bomba2    = 10 ; // Bomba2 - acueducto
//int bomba3  = 10 ; // Bomba3 - tanque externo - Libre
int bomba4    = 11 ; // MotoReductor - ajita el agua de la pileta para la limpieza
int pulsador4 = 12 ; // Boton para limpieza de la pileta
int EncienPaso= 13 ; // Switch de endendico de los motores de Paso 

int EstValUno, EstValDos, EstValTres ;

volatile boolean swApagaB3  = true ;
volatile boolean swLimpieza = false ;
volatile long duracion1, distancia1;

void setup()
{
 
  Serial.begin(115200);
  PuertoUno.begin(9600); // recibe datos de la tarjeta-Ducha
 
  pinMode(stepPinV1,OUTPUT);
  pinMode(dirPinV1, OUTPUT);

  pinMode(stepPinV2,OUTPUT);
  pinMode(dirPinV2, OUTPUT);
  
  pinMode(stepPinV3,OUTPUT);
  pinMode(dirPinV3, OUTPUT);

  pinMode(pulsador1, INPUT); // Agua en Pileta
  pinMode(pulsador2, INPUT); // Agua en la Sisterna
//pinMode(pulsador3, INPUT); // Limite Maximo Pileta
  pinMode(pulsador4, INPUT); // Boton para limpieza de la pileta

  pinMode(bomba1, OUTPUT);  // Bomba1 - Pileta
  pinMode(bomba2, OUTPUT);  // Bomba2 - acueducto
//pinMode(bomba3, OUTPUT);  // Bomba3 - tanque externo
  pinMode(bomba4, OUTPUT);  // MotoReductor - ajita el agua de la pileta para la limpieza
  
  pinMode(EncienPaso, OUTPUT);
  digitalWrite(EncienPaso, LOW)  ; //Apagado
}



void loop()
{
  serialEvent1();
  
  valorPulso1 = digitalRead(pulsador1) ;  // Agua en Pileta
  valorPulso2 = digitalRead(pulsador2) ;  // Agua en la Sisterna
  valorPulso4 = digitalRead(pulsador4) ;  // Bomba ajita el agua de la pileta para la limpieza
  delay(400)  ;

  // lleno : HIGH(1) / vacio : LOW(0) // Agua en Pileta
  // valorPulso1 = 0 ;
  if (valorPulso1 == LOW )
  {
    AguaTanqueLim() ;
    //Serial.print("  Pileta Sin agua : LOW ");
  }
  else
  {
    //Serial.print("  Pileta Con agua : HIGH ");
  }
  //Serial.print(valorPulso1);

  // -----------------------------    Limpieza de agua ----------------------------
  valorPulso4 = digitalRead(pulsador4) ;  // Bomba ajita el agua de la pileta para la limpieza
  //--------------------------------  valorPulso4 = 1 ;  //  PROVISIONAL  
  if (valorPulso4 == HIGH )
    {
     //Serial.println(" Apagada la limpieza") ;
     if (valorPulso2 == HIGH ) {
       // Sisterna con agua HIGH(1)
       digitalWrite(bomba4, HIGH) ; // Apaga MotoReductor - ajita el agua de la pileta para la limpieza  
    }
    swLimpieza = false ;
  }
  else
  {
    swLimpieza = true ;
    //Serial.println(" Prendida la limpieza") ;
    digitalWrite(bomba4, LOW) ; // Prende MotoReductor - ajita el agua de la pileta para la limpieza 
    digitalWrite(bomba1, LOW) ; // Prende Bomba1 - Pileta
    
    if (valorPulso1 == LOW ) {
      // Sin agua en la pileta
      digitalWrite(bomba1, HIGH) ; // Apaga Bomba1 - Pileta      
      digitalWrite(bomba4, HIGH) ; // Apaga MotoReductor - ajita el agua de la pileta para la limpieza  
    }
  }

  // Agua en la sisterna
  // Sisterna sin agua LOW(0) / Sisterna con agua HIGH(1)
  // valorPulso2 = 1 ;
  if (valorPulso2 == LOW )
  {
    //Serial.print("  Sisterna sin agua LOW - prende motor ");
    //Serial.println(valorPulso2);
    //Agua en Auxiliar
    if (valorPulso1 == LOW )
    {
     //Serial.print(" LLenando desde el acueducto ");
     digitalWrite(bomba1, HIGH);  // Apaga  Bomba1 - Pileta
     digitalWrite(bomba4, HIGH);  // Apaga  MotoReductor - ajita el agua de la pileta para la limpieza  
     digitalWrite(bomba2, LOW) ;  // Prende Bomba2 - acueducto
    }
    else
    {
     //Serial.print(" LLenando y ajitando ");
     digitalWrite(bomba1, LOW)  ; // Prende Bomba1 - Pileta
     digitalWrite(bomba4, LOW)  ; // Prende MotoReductor - ajita el agua de la pileta para la limpieza  
     digitalWrite(bomba2, HIGH) ; // Apaga  Bomba2 - acueducto
    }
  }
  else
  {
    if (swLimpieza == false)
      {
       //Serial.print("     Sisterna con agua HIGH - motores Apagados ");
       //Serial.println(valorPulso2);
       digitalWrite(bomba1, HIGH);  // Apaga  Bomba1 - Pileta 
       digitalWrite(bomba2, HIGH);  // Apaga  Bomba2 - acueducto
      }
    else
      { }
  }
}

void AguaTanqueLim() {
}


void serialEvent1()
{  // Ok    Serial.println("Mensaje .. : ");
   PuertoUno.listen();
   while (PuertoUno.available()) 
     {
      //Serial.println("Mensaje .. : ");
      delay(5) ;
      char c = PuertoUno.read();
      Mensaje += c ;
   }  

   Mensaje=Mensaje.substring(0,2) ;
   Serial.print("Mensaje1 .. : ");
   Serial.print(Mensaje);
   Serial.print("MensajeEnt .. : ");
   Serial.println(MensajeEnt);

   if (Mensaje.length()>0)
     {
      MensajeEnt = Mensaje.substring(0,2) ;
      
      // Suministra la corriente para los motores Paso a Paso 
      if (swEnciPaso == false)
        {
         digitalWrite(EncienPaso, HIGH) ;  // Prendido
         swEnciPaso = true ;
        }

        //Serial.print("Mensaje .. : ");
        //Serial.println(Mensaje);
        
        //Serial.print("Mensaje substring(3) ... : ");
        //Serial.println(MensajeEnt);
        
        if (Mensaje == "Si" ) 
          {
           abreVal1()   ;  // Abre valvula uno Reciclar
           Mensaje = "" ;   
          }
          
        if (Mensaje == "Nr" ) 
          {
           cierraVal1() ;  // Cierra valvula uno Reciclar       
           Mensaje = "" ;   
          }  

        //Abre en tanque el General (TgOpen)
        if ( Mensaje == "To" || MensajeEnt== "To"  ) 
          {
           //Serial.print("------ Abriendo valvula Tres 06Sep------ ");
           delay(500)   ;  // 1/2 Segundo
           abreVal3()   ;  // Abre valvula tres - Tanque General      
           delay(500)   ;  // 1/2 Segundo
           Mensaje = "" ;   
          }

        //Cierra el tanque el General (TgCier)
        if ( Mensaje == "Tc" || MensajeEnt== "Tc"  ) 
          {
           delay(500)   ;  // 1/2 Segundo           
           cierraVal3() ;  // Cierra valvula tres      
           delay(500)   ;  // 1/2 Segundo
           Mensaje = "" ;   
          }
        
        if ( Mensaje == "Re" || MensajeEnt== "Re"  ) 
          {
           //Serial.print("------ Abriendo valvula Dos ------ ");
           cierraVal2() ;  // Abre valvula dos      
           delay(38000) ;  // 35 Segundos // espera para cerrar residuos
           abreVal2()   ;  // Cierra valvula dos residuos      
           Mensaje = "" ;   
          } 
            
        if ( Mensaje == "Ri"  ) 
          {
           //Serial.print("-- Abriendo valvula Dos Residuo Inicial -- ");
           //cierraVal2();      // Cierra valvula - dos      
           // delay(500)  ;      // 5 Segundos
           // abreVal2()  ;      // Abre valvula dos - residuos      
           Mensaje = ""     ;   
          }   
     }
     Mensaje = "" ;   
     digitalWrite(EncienPaso, LOW) ; //Apagado
     swEnciPaso = false ;
}



// (0) - Cerrada (1) - Abierta
// Derecha - Abre   // Izquierda cierra 
// Reciclar
void abreVal1() 
{ 
  EstValUno = EEPROM.read(1) ; // Valvula uno
  veloPaso = 2000  ;           
  
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValUno = 0 ;
  if (EstValUno == 0)
    {
     //Esta cerrada la valvula uno 
     //LOW - horario / Abre la valvula uno
     //LOW,HIGH,LOW
     digitalWrite(dirPinV1, LOW);
     for (int i = 0; i < PasoXRevV1; i++) {
         digitalWrite(stepPinV1, HIGH);
         delayMicroseconds(veloPaso);     // velocidad menor numero + velocidad
         digitalWrite(stepPinV1, LOW);
         delayMicroseconds(veloPaso);
        }
     EstValUno = 1 ;   
     EEPROM.write(1, EstValUno);    
     delay(1000);      
  }
}

// Derecha - Abre   // Izquierda cierra 
// Reciduos
void abreVal2() 
{ 
  EstValDos = EEPROM.read(2) ; // Valvula dos
  veloPaso = 2000  ;  
  
  //Serial.print("Abre valvula Dos ");
  //Serial.println(EstValDos);
  // (0) - Cerrada (1) - Abierta  
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValDos = 0 ;
  if (EstValDos == 0)
    {
     // Esta cerrada la valvula dos 
     //  - Antihorario / Abre la valvula dos
     digitalWrite(dirPinV2, LOW);
     for (int i = 0; i < PasoXRevV2; i++) {
        digitalWrite(stepPinV2, HIGH);
        delayMicroseconds(veloPaso);     // velocidad menor numero + velocidad
        digitalWrite(stepPinV2, LOW);
        delayMicroseconds(veloPaso);
     }
     EstValDos = 1 ;   
     EEPROM.write(2, EstValDos);    
     delay(1000);     
    }
}

// Derecha - Abre   // Izquierda cierra 
// Tanque General
void abreVal3() 
{ 
  EstValTres = EEPROM.read(3) ; // Valvula tres
  veloPaso = 3000  ;           
  //veloPaso = 2000  ;           
  //veloPaso = 1500  ;
  //Serial.print("Estado de valvula - abreVal3 0-Cerr/1-Abierta ");
  //Serial.println(EstValTres);
  // (0) - Cerrada (1) - Abierta  

  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValTres = 0 ;
  if (EstValTres == 0)
    {
     // Esta cerrada la valvula dos 
     //  - Antihorario / Abre la valvula tres
     digitalWrite(dirPinV3, HIGH);  // Direccion a la derecha
     for (int i = 0; i < PasoXRevV3; i++) {
        digitalWrite(stepPinV3, HIGH);
        delayMicroseconds(veloPaso); // velocidad / mayor numero menos(-) velocidad
        digitalWrite(stepPinV3, LOW);
        delayMicroseconds(veloPaso);
     }
     EstValTres = 1 ;   
     EEPROM.write(3, EstValTres);    
     delay(1000);     
    }
}

// Reciclar
void cierraVal1() 
{ 
  EstValUno = EEPROM.read(1) ; // Valvula uno
  veloPaso = 2000  ;  
  
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValUno = 1 ;
  if (EstValUno == 1)
    {
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
     EEPROM.write(1, EstValUno);    
     delay(1000);  
    }
}

// Reciduos
void cierraVal2() 
{ 
  EstValDos = EEPROM.read(2) ; // Valvula dos
  veloPaso = 2000  ; 
    
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValDos = 1 ;
  if (EstValDos == 1)
    {
     // Serial.print("------ Giro Izquierda ------ ");
     // Esta abierta la valvula dos
     // - Horario / Cierra la valvula dos
     digitalWrite(dirPinV2, HIGH);
     for (int i = 0; i < PasoXRevV2; i++) {
       digitalWrite(stepPinV2, HIGH);
       delayMicroseconds(veloPaso);
       digitalWrite(stepPinV2, LOW);
       delayMicroseconds(veloPaso);
     }
      EstValDos = 0 ;   
      EEPROM.write(2, EstValDos);    
      delay(1000);  
    }
}

// Tanque General
void cierraVal3() 
{ 
  EstValTres = EEPROM.read(3) ; // Valvula tres
  veloPaso = 3000  ;             
  //veloPaso = 2000  ;           
  //veloPaso = 1500  ;  
  
  // (0) - Cerrada (1) - Abierta  
  // Esta linea se debe colocar cuando la tarjeta es nueva 
  // EstValTres = 1 ;
  if (EstValTres == 1)
    {
     // Serial.print("------ Giro Izquierda ------ ");
     // Esta abierta la valvula tres
     // - Horario / Cierra la valvula tres
     digitalWrite(dirPinV3, LOW);
     for (int i = 0; i < PasoXRevV3; i++) {
       digitalWrite(stepPinV3, HIGH);
       delayMicroseconds(veloPaso);
       digitalWrite(stepPinV3, LOW);
       delayMicroseconds(veloPaso);
     }
      EstValTres = 0 ;   
      EEPROM.write(3, EstValTres);    
      delay(1000);  
      //Serial.println("Se cerro la valvula TRES - cierraVal3 - ENTRO 07OCt ");
    }
}
