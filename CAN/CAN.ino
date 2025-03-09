#include <driver/twai.h>
#include <Ticker.h>

bool stateSystem = false;

Ticker thermometerTicker;
Ticker strainGaugeTicker;
Ticker protractorTicker;
Ticker pressureSensorTicker;
Ticker humiditySensorTicker;
Ticker accelerometerTicker;

uint16_t thermometerAddres = 0x0441;
uint16_t strainGaugeAddres = 0x0442;
uint16_t protractorAddres = 0x0443;
uint16_t pressureSensorAddres = 0x0444;
uint16_t humiditySensorAddres = 0x0445;
uint16_t accelerometerAddres = 0x0508;

void SendThermometerMessage() {
  WriteData(thermometerAddres);
}

void SendStrainGaugeMessage() {
  WriteData(strainGaugeAddres);
}

void SendProtractorMessage() {
  WriteData(protractorAddres);
}

void SendPressureSensorMessage() {
  WriteData(pressureSensorAddres);
}

void SendHumiditySensorMessage() {
  WriteData(humiditySensorAddres);
}

void SendAccelerpmeterMessage() {
  WriteData(accelerometerAddres);
}

void setup() 
{
  Serial.begin(115200);

  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)22, (gpio_num_t)21, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) 
  {
    Serial.println("Failed to install TWAI driver");
    return;
  }

  if (twai_start() != ESP_OK) 
  {
    Serial.println("Failed to start TWAI driver");
    return;
  }

  Serial.println("TWAI driver started");

  randomSeed(analogRead(0));

  //thermometerTicker.attach(0.05, SendThermometerMessage);
  strainGaugeTicker.attach(0.05, SendStrainGaugeMessage);
  protractorTicker.attach(0.05, SendProtractorMessage);
  pressureSensorTicker.attach(0.05, SendPressureSensorMessage);
  humiditySensorTicker.attach(0.05, SendHumiditySensorMessage);
  accelerometerTicker.attach(0.05, SendAccelerpmeterMessage);
}

void loop() 
{
    ReadData();
}

void WriteData(uint16_t addres)
{
  if (!stateSystem)
  {
    return;
  }

  twai_message_t message;

  message.identifier = addres;

  if(addres == 0x0442 || addres == 0x0443 || addres == 0x0444 || addres == 0x0445)
  {
    message.identifier = addres;
    
    message.data_length_code = 2;
    int16_t randomValue = UpdateRandomValues(addres);
    message.data[0] = (randomValue >> 8) & 0xff;
    message.data[1] = randomValue & 0xff;
  }
  if(addres == 0x0508)
  {
    message.data_length_code = 8;
    int16_t randomValue = UpdateRandomValues(0x0446);
    message.data[0] = (randomValue >> 8) & 0xff;
    message.data[1] = randomValue & 0xff;
    message.data[2] = ((randomValue + 1) >> 8) & 0xff;
    message.data[3] = (randomValue - 1) & 0xff;
    message.data[4] = ((randomValue + 1) >> 8) & 0xff;
    message.data[5] = (randomValue - 1) & 0xff;
    randomValue = UpdateRandomValues(0x0441);
    message.data[6] = (randomValue >> 8) & 0xff;
    message.data[7] = randomValue & 0xff;
  }

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) 
  {
    Serial.println("Message sent");
  } 
  else 
  {
    Serial.println("Failed to send message");
  }
}

int16_t UpdateRandomValues(uint16_t addres) {
  int16_t randomValue = 0;

  if (addres == 0x0441) {
    randomValue = random(-40, 70);
  } else if (addres == 0x0442) {
    randomValue = random(0, 60);
  } else if (addres == 0x0443) {
    randomValue = random(-15, 15);
  } else if (addres == 0x0444) {
    randomValue = random(0, 500);
  } else if (addres == 0x0445) {
    randomValue = random(0, 100);
  } else if (addres == 0x0446) {
    randomValue = random(-10, 10);
  }

  return randomValue;
}

void ReadData()
{
    twai_message_t received_message;

    if (twai_receive(&received_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
    {
        Serial.print("Message received with ID: ");
        Serial.println(received_message.identifier, HEX);
        Serial.print("Received data: ");

        for (int i = 0; i < received_message.data_length_code; i++) 
        {
            Serial.print(received_message.data[i], HEX);
            Serial.print(" ");
        }

        Serial.println();

        if (received_message.identifier == 0x508)
        {
          if(received_message.flags & TWAI_MSG_FLAG_RTR)
          {
            stateSystem = !stateSystem;
          }
        }
    }
}