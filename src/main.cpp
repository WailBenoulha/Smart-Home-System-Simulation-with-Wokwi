#include <Arduino.h>
#include <DHT.h>

// Pin definitions
#define PIR_PIN 27
#define DHTPIN 15
#define DHTTYPE DHT22
#define LDR_PIN 34
#define LED_PIN 25
#define BUZZER_PIN 26

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.println("System Initialized.");
}

void triggerAlarm() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500); // Alarm sound duration
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Read motion from PIR sensor
  int motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    Serial.println("Motion detected!");
    triggerAlarm();
  }

  // Read light level from LDR
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light level: ");
  Serial.println(lightLevel);

  // Check light level and motion to light the LED
  if (lightLevel < 500 && motionDetected) { // Adjust the threshold (500) as per your LDR calibration
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON: Dark and motion detected.");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(2000); // Loop delay for readability
}
