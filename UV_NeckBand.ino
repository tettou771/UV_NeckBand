const float TAU = 6.28;
const int vol1pin = A0;
const int vol2pin = A1;
const int vol2swPin = 2;
const int ledPin = 3;

// for debug
bool debug = true;
uint32_t pastPrint = 0;

void setup() {
  pinMode(INPUT, vol1pin);
  pinMode(INPUT, vol2pin);
  pinMode(INPUT_PULLUP, vol2swPin);
  pinMode(OUTPUT, ledPin);
}

void loop() {
  int v1 = analogRead(vol1pin);
  int v2 = analogRead(vol2pin);
  bool v2sw = digitalRead(vol2swPin);
  uint32_t t = millis();
  
  int brightnessMax = v1 / 4;
  int brightness = 0;
  bool blinkEnabled = v2sw == LOW;

  if (blinkEnabled) {
      float blinkFreq = v2 / 100. + 1.;
      int brightness = brightnessMax * abs(sin(blinkFreq * TAU * t / 2000.));
  }
  // not blink
  else {
    brightness = brightnessMax;
  }

  // manual PWM
  float phase = 0;
  float threshold = 0;
  {
    long freq = 1; // Hz
    long term = 1000000 / freq; // usec
    long current = micros() % term;
    threshold = 1. * brightness / 255.;
    phase = 1. * current / term;
    if (phase < threshold) {
      digitalWrite(ledPin, HIGH);
    }else {
      digitalWrite(ledPin, LOW);
    }
  }

  if (debug) {
    uint32_t now = millis();
    if (now - pastPrint >= 100) {
      pastPrint = now;
      Serial.print(" phase ");
      Serial.print(phase);
      Serial.print(" th ");
      Serial.print(threshold);
      Serial.print(" v1 ");
      Serial.print(v1);
      Serial.print(" v2 ");
      Serial.print(v2);
      Serial.print(" v2sw ");
      Serial.print(v2sw);
      Serial.println();
    }
  }
}
