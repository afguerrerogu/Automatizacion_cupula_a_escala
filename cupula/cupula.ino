#define STEP 4      // pin STEP de A4988 (Controlador del motor paso a paso nema) a pin 4 del arduino
#define DIR 5      // pin DIR de A4988 (Controlador del motor paso a paso nema) a pin 5 del arduino
#define RESET 9    // Pin reset de A4988 (Controlador del motor paso a paso nema) a pin 5 del arduino
#define speedPin 11// Pin 1 del integrado L293D a Pin 11 de arduino, Control de velocidad
#define dirPin1 12 // Pin 2 del integrado L293D a Pin 12 de arduino, control de una direccion del motoreductor
#define dirPin2 13 // Pin 7 del integrado L293D a Pin 13 de arduino, Control de la segunda direccion del motoreductor
#define xpin A0    // Recibe el valor de x del joystick y lo envia al pin A0 de arduino
#define ypin A1    // Recibe el valor de y del joystick y lo envia al pin A1 de arduino


//Variables necesarias para medir la velocidad del viento

int Vueltas;        // almacena valor leido por el sensor de seguimiento
int ANTERIOR = 1;   // almacena ultimo valor leido del sensor de segimiento
float tt;           // almacena el tiempo desde la ejecucion del programa
bool b;             // Variable auxiliar
int vueltas;        // Almacena el numero de vueltas las aspas del anemometro
float t1 = 0;       // Variable auxiliar para guardar el timpo recorrido
float t2;           // Varaible que mide el delta timepo en una vuelta del anemometro
float  pi =  3.1415926535897932384626433832795; // pi
float w;            // Velocidad angular
float v;            // velocidad radial del
float r = 4E-5;     // radio del anemimetro
float mlah = 2.777E7; // conversion de milisegundos a horas
float ts;              //TIEMPO EN segundos

// Variables necesarias para el uso de la cupula manualmente con el joystick
  
int speedMotor = 500;  // Velocidad del motor que abre y cierra la cupula
int xVal;               // Almacena el valor leido del joystick en el eje x
int yVal;               // Almacena el valor leido del joystick en el eje y

// Variables requeridas para el uso de la cupula remota

float paso;             // Numero de pasos requeridos para mover la cupula un angulo especifico
int posicion = 0;       // Orientacion de la cupula
int azimut;             // angulo al que se desea mover la cupula
int dato;               // entrada dada por el usuario
int dif;                // diferencia entre la posicion actual y el angulo requerido
int mov;                // angulo que se va a mover
int aux;                // variable auxilar


// Funciones para el funcionamiento del proyecto

void abrir();           // Abre la cupula
void cerrar();          // Cierra la cupula
void moviz(int pasos);  // mueve la cupula a la izquierda un numero de pasos especifico
void movder(int pasos); // mueve la cupula a la derecha un numero de pasos especificos


void setup() 
{
  
  Serial.begin(9600);       // inicializamos el puerto serial
  
  pinMode(STEP, OUTPUT);    // pin 4 como salida
  pinMode(DIR, OUTPUT);     // pin 5 como salida
  pinMode(RESET, OUTPUT);   // pin 9 como salida 
  pinMode(speedPin,OUTPUT); // pin 11 como salida
  pinMode(dirPin1,OUTPUT);  // pin 12 como salida
  pinMode(dirPin2,OUTPUT);  // pin 13 como salida
  
  pinMode(xpin, INPUT);     // pin A0 como entrada
  pinMode(ypin, INPUT);     // pin A1 como entrada



  Serial.println("Inicianlizando Cupula"); // Cabecera de la salida de datos
  delay(2000);
  Serial.println("...");
}

void loop() 
{

    Serial.println("Cupula lista");
    Serial.println("Abrir compuerta : digite 500");
    Serial.println("Cerrar compuerta : digite 600");
    Serial.println("Uso manual de la cupule: digite 1000");
    Serial.println("Ingrese cordenada azimutal de la estrella");
       
  while(Serial.available() == 0){

  
  }

// -------------------------------------------------------------------------------------------
// Movimiento remoto de la cupula 

  dato = Serial.parseInt(); // leemos un dato entero del usuario 
   
// Abrir remotamente la cupula, se ha elegido 500 como el codigo para hacerlo     
    if(dato == 500){

      digitalWrite(RESET, HIGH);
      digitalWrite(DIR, LOW);

       Serial.println("abriendo ...");
       abrir();
       Serial.println("Cupula abierta");  
       
       delay(20);
       digitalWrite(RESET, LOW);
    }

// Cerrar remotamente la cupula, se ha elegido 600 como el codigo para hacerlo  
    if(dato == 600){
      digitalWrite(RESET, HIGH);
      digitalWrite(DIR, LOW);
      
      Serial.println("cerrando...");
      cerrar();
      Serial.println("Cupula Cerrada");
      
       digitalWrite(RESET, HIGH);
       delay(20);
       digitalWrite(RESET, LOW);
    }

// Movimiento de la cupula, La entada del usuario debe ser un numero entre 1 y 360

    if (dato > 0 and dato <= 360 ){
      
      dif = posicion - dato;
      Serial.print("El angulo a mover es: ");
      Serial.println(dif);    
      
      if(dif < 0 and dif >= -180){

          mov = abs(dif);
          paso = 0.5555 * mov ;
          paso = int(paso);
          moviz(paso);
          posicion = dato;
      }
      if (dif > 0 and dif <= 180){
          mov = abs(dif) ;         
          paso = 0.5555 * mov ;
          paso = int(paso);
          movder(paso);
          posicion = dato;
      }
     if (dif > 180 and dif < 360){
          mov = 360 - abs(dif);
          paso = 0.5555 * mov ;
          paso = int(paso);
          moviz(paso);
          posicion = dato;
        
     }
     if (dif < -180 and dif > -360){
          mov = 360 - abs(dif) ;         
          paso = 0.5555 * mov ;
          paso = int(paso);
          movder(paso);
          posicion = dato;
     }
     
    }


    
// ---------------------------------------------------------------------------------------   
    // Movimiento de la cupula en modo manual
    while (dato == 1000){   // El comando para activar el modo manual se define como 1000

      
// almacenamos las salidas analogicas del joystick en las variables correspondientes

    xVal = analogRead(xpin);
    yVal = analogRead(ypin);

    
// Al mover el joystick hacia arriba la cupula se mueve en sentido contrario a las agujas del reloj
    if(xVal > 800){
        digitalWrite(RESET, HIGH);
        delay(1);
        digitalWrite(DIR, HIGH);    // giro en un sentido
          digitalWrite(STEP, HIGH);       // nivel alto
          delay(15);          // por 10 mseg
          digitalWrite(STEP, LOW);        // nivel bajo
          delay(15);
      
    }

// Al mover el joystick hacia abajo la cupula se mueve en sentido de las agujas del reloj
    if(xVal < 200){
        digitalWrite(RESET, HIGH);
        delay(1);
        digitalWrite(DIR, LOW);     // giro en sentido opuesto
        digitalWrite(STEP, HIGH);
        delay(15);          
        digitalWrite(STEP, LOW);        // nivel bajo
        delay(15);
        
      }

// Si el joystick se encuentra en reposo se apaga el controlador del motor para que no sufra
      if(xVal <=800 and xVal >=200){
        digitalWrite(RESET, LOW);
        //digitalWrite(STEP, LOW);
      }

// al mover el joystick a la izquierda la cupula se abre
      if(yVal > 800){
        digitalWrite(dirPin1, 1);
        digitalWrite(dirPin2,0);
        analogWrite(speedPin, speedMotor);    
      }

// al mover el joystick a la derecha la cupula se cierra
    if(yVal < 200){
        digitalWrite(dirPin1, 0);
        digitalWrite(dirPin2,1);
        analogWrite(speedPin, speedMotor);    
      }

// si el joystick esta en reposo se apaga el motor 
     if(yVal <= 800 and yVal >= 200){
        digitalWrite(dirPin1, 0);
        digitalWrite(dirPin2,0);
        analogWrite(speedPin, speedMotor);    
      }

  
  }


// ------------------------------------------------------------------------------------------------   

}


void abrir(){
  /*----------------------------------------
   * Funcion abrir
   * 
   * Esta funcion no recibe ningun parametro
   * de entrada y no devuelve salida. 
   * 
   * La funcion activa el motor que abre la 
   * puerta de la cupula por 9.5 segundos.
   -----------------------------------------*/
   digitalWrite(dirPin1, 1);
   digitalWrite(dirPin2,0);
   analogWrite(speedPin, speedMotor);
   delay(7500);
   digitalWrite(dirPin1, 0);
   digitalWrite(dirPin2,0);
   analogWrite(speedPin, speedMotor);       
  }



void cerrar(){
    /*----------------------------------------
   * Funcion cerrar
   * 
   * Esta funcion no recibe ningun parametro
   * de entrada y no devuelve salida. 
   * 
   * La funcion activa el motor que cierra la 
   * puerta de la cupula por 9.5 segundos.
   -----------------------------------------*/
   digitalWrite(dirPin1, 0);
   digitalWrite(dirPin2,1);
   analogWrite(speedPin, speedMotor);
   delay(7500);
   digitalWrite(dirPin1, 0);
   digitalWrite(dirPin2,0);
   analogWrite(speedPin, speedMotor);       
  }


void moviz(int pasos){
    /*----------------------------------------
   * Funcion moviz (mover a la izquierda)
   * 
   * Esta funcion recibe como parametro el numero 
   * de pasos que se desea mover la cupula teniendo
   * en cuenta que 200 pasos equivalen a 360 grados
   * y no devuelve salida. 
   * 
   * La funcion activa el motor que gira la cupula 
   * hasta llegar al angulo pedido.
   -----------------------------------------*/

  digitalWrite(RESET, HIGH);
  digitalWrite(DIR, HIGH);    
  for(int i = 0; i < pasos; i++){     
    digitalWrite(STEP, HIGH);       
    delay(10);          
    digitalWrite(STEP, LOW);        
    delay(10);          
  }
  digitalWrite(RESET, LOW); 
  
  
  }

  
void movder(int pasos){
      /*----------------------------------------
   * Funcion moviz (mover a la derecha)
   * 
   * Esta funcion recibe como parametro el numero 
   * de pasos que se desea mover la cupula teniendo
   * en cuenta que 200 pasos equivalen a 360 grados
   * y no devuelve salida. 
   * 
   * La funcion activa el motor que gira la cupula 
   * hasta llegar al angulo pedido.
   -----------------------------------------*/

  digitalWrite(RESET, HIGH);
  digitalWrite(DIR, LOW);     
  for(int i = 0; i < pasos; i++){
    digitalWrite(STEP, HIGH); 
    delay(10);
    digitalWrite(STEP, LOW);
    delay(10);
  }
  digitalWrite(RESET,LOW);

}


  
