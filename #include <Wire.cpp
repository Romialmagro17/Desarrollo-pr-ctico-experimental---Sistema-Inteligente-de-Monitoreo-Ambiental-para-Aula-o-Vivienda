#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// 1. Configuración de la Pantalla LCD I2C (Dirección 0x27 o 0x3F, 16 columnas y 2 filas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 2. Configuración del Sensor DHT11
#define DHTPIN 2       // Pin digital donde está conectado el pin de datos del DHT11
#define DHTTYPE DHT11  // Definimos que es el modelo DHT11
DHT dht(DHTPIN, DHTTYPE);

// 3. Definición de pines para LEDs y Zumbador (¡Modifícalos aquí si tus cables están en otros pines!)
const int pinLedVerde = 2;   // Pin del cable morado (LED verde)
const int pinLedRojo = 3;    // Pin del cable gris (LED rojo)
const int pinBuzzer = 7;     // Pin donde está conectado el zumbador

// 4. Límite de temperatura para la alarma (puedes ajustarlo)
const float temperaturaLimite = 25.0; 

void setup() {
  // Inicializar comunicación serie y componentes
  Serial.begin(9600);
  dht.begin();
  
  lcd.init();                      
  lcd.backlight(); // Encender luz de fondo de la pantalla

  // Configurar los pines de los LEDs y el zumbador como SALIDAS
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  // Mensaje de bienvenida inicial
  lcd.setCursor(0, 0);
  lcd.print("Iniciando Sistema");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leer la humedad y la temperatura
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Temperatura en Celsius

  // Verificar si hubo un error en la lectura del sensor
  if (isnan(h) || isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Error sensor DHT");
    return;
  }

  // Mostrar los datos en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C  ");

  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(h);
  lcd.print(" %  ");

  // Lógica de control para LEDs y Zumbador según la temperatura
  if (t > temperaturaLimite) {
    // Si la temperatura SUPERA el límite: ALERTA
    digitalWrite(pinLedRojo, LOW);   // Enciende LED rojo
    digitalWrite(pinLedVerde, HIGH);   // Apaga LED verde
    tone(pinBuzzer, 1000);            // Hace sonar el zumbador (frecuencia de 1000 Hz)
  } else {
    // Si la temperatura está normal o fresca: TODO BIEN
    digitalWrite(pinLedRojo, LOW);    // Apaga LED rojo
    digitalWrite(pinLedVerde, HIGH);  // Enciende LED verde
    noTone(pinBuzzer);                // Apaga el zumbador por completo
  }

  // Esperar 2 segundos antes de la siguiente lectura
  delay(2000);
}