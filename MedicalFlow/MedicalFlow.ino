#define BLYNK_TEMPLATE_ID "TMPL6IsMPCiZY"
#define BLYNK_TEMPLATE_NAME "MedicalFlow System"
#define BLYNK_AUTH_TOKEN "oqCHWthEyFX6T-aNPZQenxw_yjdsWIWg"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include "pitches.h"


#define LED_PIN 16
#define LED_COUNT 12
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#define LED_ONEPIN 13
#define LIGHT_SENSOR 34
#define TONE_OUTPUT_PIN 27
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define POWER_PIN  17 // ESP32 pin GPIO17 connected to sensor's VCC pin
#define SIGNAL_PIN 32


int lightValue = 0;
int pinValue = 0;
int liquidValue= 0;
const int TONE_PWM_CHANNEL = 0; 

char ssid[] = "RUNI-Wireless";
char pass[] = "";


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetLED led1(V3);

void sendTemperature()
{
  float t = dht.readTemperature(); 
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, t);
}

void sendLight()
{
  lightValue = analogRead(LIGHT_SENSOR);
  Blynk.virtualWrite(V2, lightValue);
}

void sendLiquid()
{
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  liquidValue = analogRead(SIGNAL_PIN);
  Blynk.virtualWrite(V5, liquidValue);
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF
}

void sendLed()
{
  lightValue = analogRead(LIGHT_SENSOR);
  if(lightValue > 3000) {
    led1.on();
    digitalWrite(LED_ONEPIN, HIGH); 
  }
  else{
    led1.off();
    digitalWrite(LED_ONEPIN, LOW); 
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      for (int c = b; c < strip.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5
};

int noteDurations[] = {
  8, 8, 8,
  8, 8, 8,
  4, 4, 4, 4, 4
};


void setup() {
  Serial.begin(115200);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "sgp1.blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  dht.begin();
  timer.setInterval(1000L, sendTemperature);
  timer.setInterval(1000L, sendLight);
  timer.setInterval(1000L, sendLed);
  timer.setInterval(1000L, sendLiquid);
  pinMode(13, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
}


void loop() {
  Blynk.run();
  timer.run();
}



BLYNK_WRITE(V0)
{
  pinValue = param.asInt(); // assigning incoming value from pin V0 to a variable
  if (pinValue <= 6) {
    // If pinValue is smaller than 7, turn lights green
    for (int i = 0; i < pinValue; i++) {
      strip.setPixelColor(i, 0<<16 | 255<<8 | 0); // Set to green
    }
  } else {
    // If pinValue is 7 or more, turn lights red
    for (int i = 0; i < pinValue && i < 12; i++) { // Ensure it does not exceed the strip length
      strip.setPixelColor(i, 255<<16 | 0<<8 | 0); // Set to red
    }
  }
  // Turn off the remaining LEDs.
  for (int i = pinValue; i < 12; i++) { 
    strip.setPixelColor(i, 0); // Turn off
  }
  strip.show(); // Update the strip with new settings
}

BLYNK_WRITE(V4)
{
  int switchValue = param.asInt(); // Assuming this comes from some input
  // Static variable to keep track if the melody has been played
  static int played = 0; 
  
  if (switchValue == 2) {
    if (played == 0) { // Check if the melody hasn't been played yet
      // Play the rainbow-enhanced theaterChase variant
      theaterChaseRainbow(50); 
      played = 1; // Mark as played to prevent replaying
      for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(int); thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        ledcWriteTone(TONE_PWM_CHANNEL, melody[thisNote]); // Play the note
        delay(noteDuration); // Duration of the note
        ledcWrite(TONE_PWM_CHANNEL, 0); // Stop the tone for a pause between notes
        int pauseBetweenNotes = noteDuration * 0.3; // Short pause between notes
        delay(pauseBetweenNotes);
      }
    }
  } else {
    played = 0; // Reset played flag if switchValue is not 2
  }
  
  if(switchValue == 1) {
    if (pinValue <= 6) {
      // If pinValue is smaller than 7, turn lights green
      for (int i = 0; i < pinValue; i++) {
        strip.setPixelColor(i, 0<<16 | 255<<8 | 0); // Set to green
    }
  } else {
    // If pinValue is 7 or more, turn lights red
    for (int i = 0; i < pinValue && i < 12; i++) { // Ensure it does not exceed the strip length
      strip.setPixelColor(i, 255<<16 | 0<<8 | 0); // Set to red
    }
  }

  // Turn off the remaining LEDs.
  for (int i = pinValue; i < 12; i++) { 
    strip.setPixelColor(i, 0); // Turn off
  }
  strip.show(); // Update the strip with new settings
  }
}
