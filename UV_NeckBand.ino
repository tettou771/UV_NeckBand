const float TAU = 6.28;
const int vol1pin = A0;
const int vol2pin = A1;
const int vol2swPin = 2;
const int ledPin = 7;

// for debug
bool debug = true;
uint32_t pastPrint = 0;

float v1, v2;
uint32_t past;
float brightness;

void setup() {
  pinMode(INPUT, vol1pin);
  pinMode(INPUT, vol2pin);
  pinMode(INPUT_PULLUP, vol2swPin);
  pinMode(OUTPUT, ledPin);
  v1 = v2 = 0;
  past = millis();
  brightness = 0;
}

void loop() {
  v1 += (analogRead(vol1pin) - v1) * 0.1;
  v2 += (analogRead(vol2pin) - v2) * 0.1;
  bool v2sw = digitalRead(vol2swPin);
  uint32_t t = millis();
  
  float brightnessMax = v1 / 4;
  bool blinkEnabled = (v2sw == LOW);

  if (blinkEnabled) {
    int32_t blinkFreq = 2. + v2 / 100.; // Hz
    int32_t deltaTime = t - past;
    int32_t diff = 1. * brightnessMax * deltaTime * blinkFreq / 1000.;
    brightness -= diff;
    if (brightness < 0) brightness += brightnessMax;
  }
  // not blink
  else {
    brightness = brightnessMax;
  }

  analogWrite(ledPin, brightness);

  if (debug) {
    uint32_t now = millis();
    if (now - pastPrint >= 100) {
      pastPrint = now;
      Serial.print(" b ");
      Serial.print(brightness);
      Serial.print(" v1 ");
      Serial.print(v1);
      Serial.print(" v2 ");
      Serial.print(v2);
      Serial.print(" v2sw ");
      Serial.print(v2sw);
      Serial.println();
    }
  }

  past = t;
  delay(10);
}
