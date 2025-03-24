void setup() {
  Serial.begin(115200); // Инициализация UART с Baud Rate 115200
}

void loop() {
  Serial.println("1"); // Отправка числа 1
  delay(1000); // Задержка 1 секунда
}