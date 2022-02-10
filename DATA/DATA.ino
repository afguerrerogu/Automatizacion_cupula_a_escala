/* Librerias necesrias para hacer uso del sensor de humedad y temperatur */

#include <DHT_U.h>   
#include <DHT.h>  

#define LDR A3     // pin S de modulo de fotoresistencia a entrada analogica A3 de Arduino
#define TempHum 2  // Pin de salida del sensor de Humedad y temperatura DHT11 a PIN 2 de arduino
#define Viento 7   // Pin de salida del sesor de seguimiento a pin 7 de arduino
#define led_h 8    // Alerta de humedad alta
#define led_l 6     // Alerta de buena luminosidad

// Variables necesarias para el uso del sensor fotoresistivo 

int LUMINOSIDAD;    // almacena valor leido de entrada A0
int ValorMapeado;   // Guarda el valor de Luminocidad en un dato util para astronomia
int pwm;            // alerta de brillo

// Variables necesarias para el uso del sensor de temperatura y humedad

int TEMPERATURA;    // Almacena el valor de Temperatura dado por el sensor
int HUMEDAD;        // Almacena el valor de la humedad dado por el sensor
bool aux_h;         // variable auxiliar para cerrar cupula

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

float velocidad( float Valor); // Calcula la velocidad del viento
bool aux;
int ant = 0;

// inicializa el sensor de humedad
DHT dht(TempHum, DHT11);

void setup() {

  Serial.begin(9600);
  pinMode(led_h, OUTPUT);   // PIN 8 como salida
  pinMode(led_l, OUTPUT);   // PIN 6 como salida
  
  pinMode(Viento, INPUT);   // pin 7 como entrada


  dht.begin();

//  Serial.println("#Ubicacion,timepo ,temperatura, humedad, viento, luminosidad"); // Cabecera de la salida de datos
}



void loop() {
    // Escribimos los datos dados por los sensores y los escribimos en variables para generar salidas codificables
    
  TEMPERATURA = dht.readTemperature();
  HUMEDAD = dht.readHumidity();
  tt = millis();
  ts = tt /1000;
  Vueltas = digitalRead(Viento);
  LUMINOSIDAD = analogRead(LDR); // lee valor de entrada A0
  ValorMapeado = map(LUMINOSIDAD, 0, 1023, -10, 25);
  pwm = map(LUMINOSIDAD,0, 1023, 0,255);
  analogWrite(led_l, pwm);
  
  v = velocidad(Vueltas);

  // imprimimos los datos de salida
//  if (int(ts) == ant and aux == false){
//    aux = true;
//    ant = int(ts)+1;
    Serial.print(ts);
    Serial.print(", ");
    Serial.print(HUMEDAD);
    Serial.print(", ");
    Serial.print(TEMPERATURA);
     Serial.print(", ");
     Serial.print(v);
     Serial.print(", ");
     Serial.println(ValorMapeado);
      
//  }
//  if (int(ts) != ant){
//    aux = false;
//  }


// Alerta humedad alta

  if (HUMEDAD > 80){
    digitalWrite(led_h, HIGH);
  }
  if (HUMEDAD <= 80){
    digitalWrite(led_h,LOW);
  }


}


float velocidad( float VALOR){
      /*----------------------------------------
   * Funcion Velocidad
   * 
   * Esta funcion recibe como parametro el valor 
   * que da el sensor de segimiento, tener en cuenta
   * que este valor es 1 o 0 y devuelve la velocidad
   * del viento.
   * 
   * La funcion calcula la velocidad angular de las 
   * aspas que son movidas por el viento y devuelve
   * la velocidad del viento en km/h
   -----------------------------------------*/
    if (VALOR == 0 and b == false){
    vueltas = vueltas + 1;
    b = true;

      t2 = tt - t1;
      t1 = tt;
      
      w = 2*pi / t2;
      w = w * mlah;
      v = r*w ;
  }
  if (VALOR == 1){
    b = false;
  }
  return v;
}
