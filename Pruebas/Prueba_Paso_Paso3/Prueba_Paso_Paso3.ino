// Prende con el Pin que se le indique, para dar suministro de corriente de 5v.  (EncienPaso)


#define EncienPaso  13 // Si se enciende por el Pin No. 13


//#define steps   A1
//#define direccion A0

//#define steps   A2
//#define direccion A3


// PROBADOR UNO
//#define steps   A3
//#define direccion A2

#define steps   A5
#define direccion A4

//#define steps   5
//#define direccion 6

// a mayor numero mas lento
//volatile int     veloPaso   = 500  ; // Ok - mas rapido / Velocidad motor paso 
//volatile int     veloPaso   = 1500  ; // Velocidad motor paso 
volatile int     veloPaso   = 2000  ; // Velocidad motor paso 
volatile boolean swEnciPaso = false ;

void setup ()
{
  Serial.begin(9600);
  pinMode(direccion, OUTPUT);
  pinMode(steps, OUTPUT);

  pinMode(EncienPaso, OUTPUT);
  digitalWrite(EncienPaso, LOW)  ; //Apagado
}

void loop()
{
  if (swEnciPaso == false)
    {
     swEnciPaso = true ;
     digitalWrite(EncienPaso, HIGH) ; //Prende el Pin de corriente para el Driver
    } 

  Serial.println("  ");  
  Serial.println("  Abre valvula - Reloj ");  
  
  delay(2000);
  digitalWrite(direccion, LOW);  // Direccion Horario
  for (int i=0; i<480; i++){   // 480
     digitalWrite(steps,HIGH);   // siempre debe estar los step en "HIGH""

     delayMicroseconds(veloPaso);  // velocidad / mayor numero menos(-) velocidad
     digitalWrite(steps,LOW);    // siempre debe estar los step en ""LOW"
     
     delayMicroseconds(veloPaso);       

  }
  delay(1000);
  
  Serial.println("  ");  
  Serial.println("  Cierra valvula - AntiReloj");  
  digitalWrite(direccion, HIGH);  // Direccion AntiHorario
  for (int i=0; i<480; i++){    // 480
     digitalWrite(steps,HIGH);
  
     delayMicroseconds(veloPaso);  // velocidad / mayor numero menos(-) velocidad
     digitalWrite(steps,LOW);
 
     delayMicroseconds(veloPaso);       
  }
  delay(1000); 

  if (swEnciPaso == true)
    {
     swEnciPaso = false ;
     digitalWrite(EncienPaso, LOW) ; //Prende el Pin de corriente para el Driver
    } 
}
