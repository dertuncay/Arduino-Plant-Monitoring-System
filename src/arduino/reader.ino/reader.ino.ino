#include <dht.h>

dht DHT;

#define DHT11_PIN 7
#define CDS_PIN A0
#define READ_DELAY 100

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int incomingByte = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

int temperature()
{
  int temp;
  int chk = DHTLIB_ERROR_TIMEOUT;

  while (chk == DHTLIB_ERROR_TIMEOUT)
  {
    delay(READ_DELAY);
    chk = DHT.read11(DHT11_PIN);
  }
  return DHT.temperature;
}

int humidity()
{
  int humidity;
  int chk = DHTLIB_ERROR_TIMEOUT;

  while (chk == DHTLIB_ERROR_TIMEOUT)
  {
    delay(READ_DELAY);
    chk = DHT.read11(DHT11_PIN);
  }
  return DHT.humidity;
}

int light()
{
  int n_reads = 10;
  int brightness = 0;

  for (int i = 0; i < n_reads; i++)
  {
    brightness = brightness + analogRead(CDS_PIN);
    delay(50);
  }

  return brightness / n_reads;
}

int moisture_sensor(int i)
{
  int power[5];
  power[0] = 0x01;
  power[1] = 0x02;
  power[2] = 0x04;
  power[3] = 0x08;
  power[4] = 0x10;

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, power[i]);
  digitalWrite(latchPin, HIGH);

  delay(200);
  int total = 0;
  int n_reads = 10;
  for (int i = 0; i < n_reads; i++)
  {
    total = total + analogRead(A1);
    delay(50);
  }
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
  return total / n_reads;
}

void loop()
{
  /*Serial.println(temperature());
  Serial.println(humidity());
  Serial.println(light());*/
  
  //Serial.println(moisture_sensor(1));
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    switch(incomingByte - 48)
    {
      case 0:
        Serial.println(temperature());
        break;
      case 1:
        Serial.println(humidity());
        break;
      case 2:
        Serial.println(light());
        break;
      case 3:
        Serial.println(moisture_sensor(0));
        break;
      case 4:
        Serial.println(moisture_sensor(1));
        break;
      case 5:
        Serial.println(moisture_sensor(2));
        break;
      case 6:
        Serial.println(moisture_sensor(3));
        break;
    }
  }
  delay(100);
}
