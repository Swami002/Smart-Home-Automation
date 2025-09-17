#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <DHT.h>


// LCD setup (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(13, 3, 5, 6, 7, 8);


// DHT sensor configuration
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// Define pins for sensors
#define RAIN_SENSOR_ANALOG_PIN A0
#define RAIN_SENSOR_DIGITAL_PIN 12
#define FLAME_SENSOR_PIN 3


// Bluetooth module (HC-05) connected to pins 10 (RX), 11 (TX)
SoftwareSerial BTSerial(10, 11);


void setup() {
    Serial.begin(9600);   
    BTSerial.begin(9600); 
    dht.begin(); 


    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();


    pinMode(RAIN_SENSOR_ANALOG_PIN, INPUT);
    pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT);
    pinMode(FLAME_SENSOR_PIN, INPUT);


    Serial.println("Bluetooth Ready!");
    BTSerial.println("Bluetooth Ready!");
}


void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error reading DHT sensor!");
        BTSerial.println("Error reading DHT sensor!");
        return;
    }


    int rainAnalogValue = analogRead(RAIN_SENSOR_ANALOG_PIN);
    int rainDigitalValue = digitalRead(RAIN_SENSOR_DIGITAL_PIN);
    int flameSensorValue = digitalRead(FLAME_SENSOR_PIN);


    String sensorData = "Temp: " + String(temperature) + "C\n";
    sensorData += "Hum: " + String(humidity) + "%\n";
    sensorData += "Rain: " + String(rainAnalogValue > 400 ? "No" : "Yes") + "\n";
    sensorData += "Flame: " + String(flameSensorValue == HIGH ? "No" : "Yes") + "\n";
    sensorData += "Rain Analog Value: " + String(rainAnalogValue) + "\n";
    Serial.println(sensorData);
    BTSerial.println(sensorData);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print("C H:");
    lcd.print(humidity);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Rain:");
    lcd.print(rainAnalogValue > 400 ? "No" : "Yes");
    lcd.print(" F:");
    lcd.print(flameSensorValue == HIGH ? "No" : "Yes");


    if (BTSerial.available()) {
        String receivedData = BTSerial.readStringUntil('\n');
        receivedData.trim();
        Serial.print("BT Received: ");
        Serial.println(receivedData);
    }


    if (Serial.available()) {
        String sendData = Serial.readStringUntil('\n');
        Serial.print("Sending to BT: ");
        Serial.println(sendData);
        BTSerial.println(sendData);
    }


    delay(2000);
}
