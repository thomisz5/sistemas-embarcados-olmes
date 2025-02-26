#include <Wire.h>

#define BUTTON_PIN 10
#define SENSOR_PIN 13
#define BUZZER_PIN 8
#define LED_PIN 3
#define LED_RED 5
#define LED_GREEN 4

int valorLido = 0;
int valorSensor;
bool ultimoEstado = -1;
bool ledState = false;        // Estado atual para alternar os LEDs
bool lastButtonState = LOW;   // Último estado do botão
bool currentButtonState = HIGH;      // Estado atual do botão

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(SENSOR_PIN, INPUT_PULLUP );
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH); // LED Verde ligado inicialmente

  Wire.begin();  // Inicia comunicação I2C
  Serial.begin(9600);
}

void loop() {
  
   currentButtonState = digitalRead(BUTTON_PIN); // Lê o estado atual do botão

  // Detecta se o botão foi pressionado (mudança de HIGH para LOW)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    ledState = !ledState;  // Alterna o estado dos LEDs
    
    // Liga um LED e desliga o outro dependendo do estado
    if (!ledState) {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      Wire.beginTransmission(8); // Endereço do escravo (Arduino 1)
      Wire.write('P'); // Enviar comando para avisar que está permitido
      Wire.endTransmission();
      delay(200);
    } else {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      Wire.beginTransmission(8); // Endereço do escravo (Arduino 1)
      Wire.write('B'); // Enviar comando para bloquear
      Wire.endTransmission();
      delay(200);
    }

    delay(50); // Debounce simples para evitar múltiplas leituras rápidas
  }

  lastButtonState = currentButtonState; // Atualiza o estado do botão
  
  valorSensor = digitalRead(SENSOR_PIN);
  Serial.println(valorSensor);

  if (valorSensor == LOW && !ledState) {
    Serial.print('entrou no incrementar');
    // Envia sinal de detecção ao escravo quando autorizado e sensor ativado
    Wire.beginTransmission(8);  // Endereço do escravo
    Wire.write('I');              // Sinal de unidade detectada
    Wire.endTransmission();
  } else if (valorSensor == LOW && ledState) {
    // Ativa o buzzer e o LED vermelho
    Serial.print('entrou no bloquear');
    tone(BUZZER_PIN, 220, 100);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    delay(1000);
  }

  delay(100); // Anti-debounce para o sensor e reduz a carga no processador
}
