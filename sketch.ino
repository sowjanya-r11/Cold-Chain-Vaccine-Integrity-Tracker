#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

#define DHTPIN 15
#define DHTTYPE DHT22

#define LDR 34
#define BUZZER 13

#define RED 25
#define GREEN 26
#define BLUE 27

#define ONE_WIRE_BUS 4

DHT dht(DHTPIN, DHTTYPE);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

Adafruit_MPU6050 mpu;

void setup() {

  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  dht.begin();
  ds18b20.begin();

  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }

  Serial.println("System Ready");
}

void loop() {

  // DHT22 humidity only
  float humidity = dht.readHumidity();

  // DS18B20 temperature
  ds18b20.requestTemperatures();
  float dsTemp = ds18b20.getTempCByIndex(0);

  // LDR reading
  int light = analogRead(LDR);

  // MPU6050 reading
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.println("------ SENSOR DATA ------");

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("DS18B20 Temp: ");
  Serial.println(dsTemp);

  Serial.print("Light Level: ");
  Serial.println(light);

  Serial.print("Acceleration X: ");
  Serial.println(a.acceleration.x);

  // ALERT CONDITIONS
  if (dsTemp > 40 || light < 200 || fabs(a.acceleration.x) > 5) {

    Serial.println("ALERT DETECTED!");

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    tone(BUZZER, 1000);
    delay(300);
    noTone(BUZZER);
    delay(300);
  }

  else {

    Serial.println("System Safe");

    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);

    noTone(BUZZER);
  }

  delay(1500);
}
