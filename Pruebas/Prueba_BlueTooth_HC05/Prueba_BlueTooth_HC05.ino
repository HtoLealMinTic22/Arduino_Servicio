// HC 05 Maestro (6) Pines/ HC 06 Esclavo (4) Pines

#include <SoftwareSerial.h>
SoftwareSerial PuertoUno(10,11);    // Tx-10 / Rx-11 

/*Instruciones 
 1. Desconecte el VCC del modulo (Antes debe cargar este codigo a la tarjeta) 
 2. Mantenga presionado el pulsador del modulo y concete el VCC
 3. Cuando la luz del modulo se prenda intermitente lenta suelte el pulsador, indica que esta en modo 
    de programacion
 4. Abra en monitor serial del arduino y aparecera el mensake "<-- Listo -->"
 5. El monitor debe tener :
              AutoScroll
              Ambos NL & CR
              9600
 6.Digite los comandos "AT" correspondientes
     - AT      Para saber si esta conectado en modo de programacion

     - AT+ROLE? 0-Esclavo / 1-Maestro
     - AT+PSWD= 0000 (Asignar el Password)
     
     - AT+NAME= AT+NAME = Ducha1   (Nombre del Bluetooth)
     - AT+NAME= Ducha1
     // esta opcion asigna el nombre al Bluetooh, pero tambien hay que
     // modificarlo manualmente en el Celular o Tablet respectivamente
     
     - AT+PSWD?  (Averiguar el Password)
     - AT+PSWD= 0000 (Asignar el Password)

     - AT+UART   (Velocidad en vaudios)
     - AT+ORGL

     - AT+RESET Para quitar el modo de programacion
     
 7. Digite el comando "AT+RESET"

*/

void setup()
{
   Serial.begin(9600);
   Serial.println("<-- Listo -->") ;

   // COMUNICACION ENTRE EL Bluetooh y el Arduino
   PuertoUno.begin(38400)  ;
   
}


void loop()
{
  if (PuertoUno.available())       // Lee PuertoUno y envia a arduino
                                   // devuelve verdadero si hay datos desde el modulo HC-05
  Serial.write(PuertoUno.read()) ; // si hay alguna informacion desde el HC-05 
                                   // la mostramos

  if (Serial.available())
  PuertoUno.write(Serial.read()) ; // Lee arduino y envia a PuertoUno
}


//   PuertoUno.print("AT+NAME") ;
//   PuertoUno.print(NOMBRE)  ;
//   delay(1000) ;
   
//   PuertoUno.print(AT+PIN) ;
//   PuertoUno.print(PASS)  ;
//   delay(1000) ;
