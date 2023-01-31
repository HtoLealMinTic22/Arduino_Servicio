import serial
import time 

arduino = serial.Serial('COM4',9600)
time.sleep(2)



while True:
    val = arduino.readline()
    cad = val.decode('ascii')
    print(cad)
    arduino.write(b'1')    
    arduino.write(b'A')    




