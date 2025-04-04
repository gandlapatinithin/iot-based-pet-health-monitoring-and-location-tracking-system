#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
#define ONE_WIRE_BUS 2         // DS18B20 on digital pin 2
#define HEARTBEAT_PIN A0       // Analog pin A0 for Heartbeat sensor
#define GPS_RX 4               // GPS Module RX
#define GPS_TX 3               // GPS Module TX
#define GSM_RX 7               // GSM RX
#define GSM_TX 8               // GSM TX

// Initialize LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temperature = 0.0;
int heartbeat = 0;
String gpsData = "";

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsmSerial.begin(9600);
  sensors.begin();
  
  lcd.print("Pet Tracker");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // Read heartbeat (basic analog reading)
  heartbeat = analogRead(HEARTBEAT_PIN);

  // Read GPS Data (mocked for now)
  gpsData = readGPS();

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(heartbeat / 10); // scaled for display

  lcd.setCursor(0, 1);
  lcd.print("Sending SMS...");
  
  sendSMS(gpsData, temperature, heartbeat / 10);

  delay(10000); // delay 10 seconds
}

String readGPS() {
  String gpsBuffer = "";
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gpsBuffer += c;
    if (c == '\n') break;
  }

  // You can parse GPS sentences here if needed
  // For now, return mock location
  return "Lat:17.3850,Long:78.4867";
}

void sendSMS(String location, float temp, int bpm) {
  gsmSerial.println("AT+CMGF=1"); // SMS mode
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+91xxxxxxxxxx\""); // replace with your number
  delay(1000);
  gsmSerial.print("Pet Location: ");
  gsmSerial.println(location);
  gsmSerial.print("Temp: ");
  gsmSerial.print(temp);
  gsmSerial.println(" C");
  gsmSerial.print("Heartbeat: ");
  gsmSerial.print(bpm);
  gsmSerial.println(" BPM");
  gsmSerial.write(26); // Ctrl+Z to send
  delay(3000);
}
