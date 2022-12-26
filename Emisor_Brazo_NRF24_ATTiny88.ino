
/* Comunicación con NRF24L01 ATTiny88 
- Programa para el Transmisor
Libreria: 
TMRh20/RF24 https://github.com/tmrh20/RF24/ */
 
/* Debemos incluir la libreria SPI basica y la 
libreria RF24  que ya hemos instalado */
#include <SPI.h>
#include <RF24.h>
 
/* Crear un objeto RF24 al que llamaremos “radio”. 
Los dos argumentos que se utilizan en la funcion 
son los numeros de pin que vamos a usar para CS y CE. */
const int pinCE = 15;
const int pinCSN = 14;
RF24 radio(pinCE, pinCSN); // CE, CSN
/*
El resto de pines del NRF a conectar al ATTiny son los del SPI:
NRF-MOSI > 11
NRF-SCK  > 13
NRF-MISO > 12
*/


 
/* Creamos una matriz de bytes que represente la 
direccion con la que se reconoceran entre si los 
dos modulos. Podemos cambiar el valor de esta 
direccion a cualquier conjunto de 5 letras y/o 
numeros. Esta direccion permite elegir con que 
receptor vamos a comunicarnos. En nuestro caso, 
usaremos la misma direccion tanto en el receptor 
como en el transmisor. */

//const byte identificacion[6] = "PP101";
const uint64_t pipe = 0xE8E8F4F0E1LL;

int data[1];//Array de data a ENVIAR por el NRF24


//Estableciendo Pines para JOYSTICK

int X;				// variable para almacenar valor leido del eje X
int Y;				// variable para almacenar valor leido del eje y

int LED_ARRIBA = 7;		// LED de arriba a pin digital 5
int LED_ABAJO = 8;		// LED de abajo a pin digital 6
int LED_DERECHO = 9;		// LED derecho a pin PWM 10
int LED_IZQUIERDO = 10;		// LED izquierdo a pin PWM 9

int LED_SW = 6;		// LED de pulsador a pin digital 25
int PULSADOR = 16;		// pulsador incorporado pin digital 16
int SW;				// variable para almacenar valor leido del pulsador

int X_CODE;   // variable para almacenar "Acción del eje X" [0=Nada|1=DERECHA|2=IZQUIERDA]
int Y_CODE;   // variable para almacenar "Acción del eje Y" [0=Nada|1=ARRIBA|2=ABAJO]
int PULSADOR_MODE;  // Variable para saber si se ha pulsado el pulsador del joystick

void setup(){
/* En la seccion de configuracion debemos inicializar 
el objeto “radio” */
  radio.begin();

/* Mediante la funcion radio.openWritingPipe(), 
establecer la direccion del receptor al que 
enviaremos los datos. En este ejemplo, la cadena 
de 5 caracteres que hemos configurado. */
  radio.openWritingPipe(pipe);     // Abrir para escribir

/* Configuramos el nivel del amplificador de potencia,
usando la funcion radio.setPALevel(). Mientras 
estemos a este nivel de prueba, es decir prototipos 
sobre la mesa de trabajo, lo establecemos al minimo, 
ya que los modulos estaran cerca el uno del otro. 
Si se utiliza un nivel mas alto, se recomienda 
utilizar capacitores de filtro entre las entradas 
GND y 3,3 V de los mdulos para que su voltaje se 
mantenga más estable durante el funcionamiento. */
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);

/* La función radio.stopListening() cumple la de establecer al módulo como transmisor. */
  radio.stopListening();

/* La funcion radio.startListening() establece al modulo como receptor. */
//radio.startListening();


/* Inicializamos los Pines de LEDS y PULSADORES */
  pinMode(LED_IZQUIERDO, OUTPUT);	// LED como salida
  pinMode(LED_DERECHO, OUTPUT);		// LED como salida
  pinMode(LED_ABAJO, OUTPUT);		// LED como salida
  pinMode(LED_ARRIBA, OUTPUT);		// LED como salida
  pinMode(LED_SW, OUTPUT);		// LED como salida
  pinMode(PULSADOR, INPUT);		// pulsador como entrada
  // entradas analogicas no requieren inicializacion
}

void loop(){

  data[0] = 0; //Data a enviar por el NRF34
  X_CODE = 0;  // Acción del eje 'X' [Subir-Bajar]
  Y_CODE = 0;  // Acción del eje 'Y' [Derecha-Izquierda]
  PULSADOR_MODE = 0;

  X = analogRead(A7);			// lectura de valor de eje x
  Y = analogRead(A6);			// lectura de valor de eje y
  SW = digitalRead(PULSADOR);		// lectura de valor de pulsador


  //EJE 'X' DERECHA-IZQUIERDA
  if (X > 541 && X <= 1023){			// si X esta en la zona derecha
    analogWrite(LED_DERECHO, map(X, 541, 1023, 0, 255));	// brillo LED proporcional
    X_CODE = 1;
  } else {
    analogWrite(LED_DERECHO, 0);	// X en zona de reposo, apaga LED
  }
  if (X >= 0 && X < 481){					// si X esta en la zona izquierda
    analogWrite(LED_IZQUIERDO, map(X, 0, 481, 255, 0));		// brillo LED proporcional
    X_CODE = 2;
  } else {
    analogWrite(LED_IZQUIERDO, 0);				// X en zona de reposo, apaga LED
  }


  //EJE 'Y' ARRIBA-ABAJO
  if (Y >= 0 && Y < 481){					// si Y esta en la zona de abajo
    analogWrite(LED_ARRIBA, map(Y, 0, 481, 255, 0));		// brillo LED proporcional
    Y_CODE = 1;
  } else {
    analogWrite(LED_ARRIBA, 0);					// Y en zona de reposo, apaga LED
  }
  if (Y > 541 && Y <= 1023){					// si Y esta en la zona de arriba
    analogWrite(LED_ABAJO, map(Y, 541, 1023, 0, 255));		// brillo LED proporcional
    Y_CODE = 2;
  } else {
    analogWrite(LED_ABAJO, 0);   				// Y en zona de reposo, apaga LED     
    //Enviando 0 al Receptor
  }
      
  if (digitalRead(PULSADOR)==LOW){					// si Y esta en la zona de abajo
    digitalWrite(LED_SW, 1);		// brillo LED 
    PULSADOR_MODE = 1;
  } else {
    digitalWrite(LED_SW, 0);	//  apaga LED
  }


 //Según que boton/es este pulsado se enviará un mensaje u otro
    if (PULSADOR_MODE==1){
        data[0] = 2;
        //send("2")-modalidad;

    } else if ((Y_CODE==1) && (X_CODE==0)){
        data[0] = 5;
        //send("5")-alante;

    }else if ((Y_CODE==2) && (X_CODE==0)){
        data[0] = 6;
        //send("6")-atras;

    }else if ((Y_CODE==0) && (X_CODE==2)){
        data[0] = 7;
        //send("7")-izquierda;

    }else if ((Y_CODE==0) && (X_CODE==1)){
        data[0] = 8;
        //send("8")-derecha;

    }else if ((Y_CODE==1) && (X_CODE==2)){
        data[0] = 9;
        //send("9")-alanteizquierda;

    }else if ((Y_CODE==1) && (X_CODE==1)){
        data[0] = 10;
        //send("10")-alantederecha;

    }else if ((Y_CODE==2) && (X_CODE==2)){
        data[0] = 11;
        //send("11")-atrasizquierda;

    }else if ((Y_CODE==2) && (X_CODE==1)){
        data[0] = 12;
        //send("12")-atrasderecha;

    }else{
        data[0] = 0;

    }//Fin de ELSE-IF


  radio.write(data, sizeof(data));
  
//  delay(200);
}