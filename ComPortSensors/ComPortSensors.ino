#include <Arduino.h>
#include <Ticker.h>

Ticker thermometerTicker;
Ticker strainGaugeTicker;
Ticker protractorTicker;
Ticker pressureSensorTicker;
Ticker humiditySensorTicker;
Ticker accelerometerTickerX;
Ticker accelerometerTickerY;
Ticker accelerometerTickerZ;


uint8_t thermometerMessage[] = { 0xc0, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00 };
uint8_t strainGaugeMessage[] = { 0xc0, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00 };
uint8_t protractorMessage[] = { 0xc0, 0x02, 0x03, 0x02, 0x00, 0x00, 0x00 };
uint8_t pressureSensorMessage[] = { 0xc0, 0x02, 0x04, 0x02, 0x00, 0x00, 0x00 };
uint8_t humiditySensorMessage[] = { 0xc0, 0x02, 0x05, 0x02, 0x00, 0x00, 0x00 };
uint8_t accelerometerMessageX[] = { 0xc0, 0x02, 0x06, 0x02, 0x00, 0x00, 0x00 };
uint8_t accelerometerMessageY[] = { 0xc0, 0x02, 0x07, 0x02, 0x00, 0x00, 0x00 };
uint8_t accelerometerMessageZ[] = { 0xc0, 0x02, 0x08, 0x02, 0x00, 0x00, 0x00 };

void SendThermometerMessage() {
  UpdateRandomValues(thermometerMessage);
  UpdateCRC(thermometerMessage);
  Serial.write(thermometerMessage, sizeof(thermometerMessage));
}

void SendStrainGaugeMessage() {
  UpdateRandomValues(strainGaugeMessage);
  UpdateCRC(strainGaugeMessage);
  Serial.write(strainGaugeMessage, sizeof(strainGaugeMessage));
}

void SendProtractorMessage() {
  UpdateRandomValues(protractorMessage);
  UpdateCRC(protractorMessage);
  Serial.write(protractorMessage, sizeof(protractorMessage));
}

void SendPressureSensorMessage() {
  UpdateRandomValues(pressureSensorMessage);
  UpdateCRC(pressureSensorMessage);
  Serial.write(pressureSensorMessage, sizeof(pressureSensorMessage));
}

void SendHumiditySensorMessage() {
  UpdateRandomValues(humiditySensorMessage);
  UpdateCRC(humiditySensorMessage);
  Serial.write(humiditySensorMessage, sizeof(humiditySensorMessage));
}

void SendAccelerpmeterXMessage() {
  UpdateRandomValues(accelerometerMessageX);
  UpdateCRC(accelerometerMessageX);
  Serial.write(accelerometerMessageX, sizeof(accelerometerMessageX));
}


void SendAccelerpmeterYMessage() {
  UpdateRandomValues(accelerometerMessageY);
  UpdateCRC(accelerometerMessageY);
  Serial.write(accelerometerMessageY, sizeof(accelerometerMessageY));
}


void SendAccelerpmeterZMessage() {
  UpdateRandomValues(accelerometerMessageZ);
  UpdateCRC(accelerometerMessageZ);
  Serial.write(accelerometerMessageZ, sizeof(accelerometerMessageZ));
}

void UpdateRandomValues(uint8_t* message) {
  int16_t randomValue = 0;

  if (message[2] == 0x01) {
    randomValue = random(-40, 70);
  } else if (message[2] == 0x02) {
    randomValue = random(0, 60);
  } else if (message[2] == 0x03) {
    randomValue = random(-15, 15);
  } else if (message[2] == 0x04) {
    randomValue = random(0, 500);
  } else if (message[2] == 0x05) {
    randomValue = random(0, 100);
  } else if (message[2] == 0x06) {
    randomValue = random(-10, 10);
  } else if (message[2] == 0x07) {
    randomValue = random(-10, 10);
  } else if (message[2] == 0x08) {
    randomValue = random(-10, 10);
  }

  message[4] = (randomValue >> 8) & 0xff;
  message[5] = randomValue & 0xff;
}

void UpdateCRC(uint8_t* message) {
  message[6] = (message[0] + message[1] + message[2] + message[3] + message[4] + message[5]) % 255;
}

void setup() {
  Serial.begin(921600);
  randomSeed(analogRead(0));

  thermometerTicker.attach(0.1, SendThermometerMessage);
  strainGaugeTicker.attach(0.1, SendStrainGaugeMessage);
  protractorTicker.attach(0.1, SendProtractorMessage);
  pressureSensorTicker.attach(0.1, SendPressureSensorMessage);
  humiditySensorTicker.attach(0.1, SendHumiditySensorMessage);
  accelerometerTickerX.attach(0.1, SendAccelerpmeterXMessage);
  accelerometerTickerY.attach(0.1, SendAccelerpmeterYMessage);
  accelerometerTickerZ.attach(0.1, SendAccelerpmeterZMessage);
}

void loop() {

}