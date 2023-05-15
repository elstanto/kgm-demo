#include <Adafruit_AW9523.h>
#include <ESP8266WiFi.h>

#define IMPULSE D5 
#define NORMAL D6

#define DWELL_LIMIT 1000 // 10s

Adafruit_AW9523 aw;

int count;
int dwell;
bool last_impulse = true;

void setup() {
  WiFi.forceSleepBegin();
  Serial.begin(115200);
  if (! aw.begin()) {
    while (1) delay(10);  // halt forever
  }
  for (int led = 0; led < 10; led++) {
    aw.pinMode(led, OUTPUT);
  }
  pinMode(IMPULSE, INPUT_PULLUP);
  pinMode(NORMAL, INPUT_PULLUP);
}

void clear_leds() {
  for (int led = 0; led < 10; led++) {
    aw.digitalWrite(led, LOW);
  }
}

void update_leds() {
  clear_leds();
  aw.digitalWrite(count - 1, HIGH);
}

void loop() {
  if (digitalRead(NORMAL)) {
    if (dwell > DWELL_LIMIT) {
      dwell = 0;
      clear_leds();
      ESP.deepSleep(2e5);
    } else if (dwell > 0) {
      dwell++;
      last_impulse = false;
      count = 0;
    } else {
      ESP.deepSleep(2e5);
    }
  } else {
    dwell = 1;
    if (digitalRead(IMPULSE) == true & last_impulse == false) {
      last_impulse = true;
      count++;
      if (count > 10) {
        count = 10;
      }
      update_leds();
    } else {
      if (digitalRead(IMPULSE) == false) {
        last_impulse = false;
      }
    }
  }
  delay(10);
  Serial.println("tick");
}
