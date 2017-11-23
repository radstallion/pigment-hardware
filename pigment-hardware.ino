#define encoder0PinA  2
#define encoder0PinB  3
#define CCW 0
#define CW 1
#define SENT 0
#define READY 1


//defines modes
#define MODE_BRUSH  1
#define MODE_ERASER  2
#define MODE_HANDTOOL  3
#define MODE_HSB  4
#define MODE_HISTORY  5

uint8_t mainBtn[] = {5, 6, 7, 8, 9};
uint8_t sideBtn[] = {10, 11, 12};
uint8_t mainStatus = 0; //0 for none 1-5 for modes
uint8_t sideStatus = 0; //0 for none 1-3 for modes
uint8_t old_mainStatus = 0; //0 for none 1-5 for modes
uint8_t old_sideStatus = 0; //0 for none 1-5 for modes

char msg = '-';

volatile int encoder0Pos = 0;
volatile uint8_t r_state = CCW;
volatile uint8_t s_state = SENT;

void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);
  for (int i = 0 ; i < 5 ; i++) {
    pinMode(mainBtn[i], INPUT);
    digitalWrite(mainBtn[i], HIGH);
  }
  for (int i = 0 ; i < 3 ; i++) {
    pinMode(sideBtn[i], INPUT);
    digitalWrite(sideBtn[i], HIGH);
  }
  Serial.begin (9600);
}

void loop() {
  //  checks mainBtn status
  for (int i = 0 ; i < 5 ; i++) {
    if (digitalRead(mainBtn[i]) == 0) {
      mainStatus = i + 1;
      break;
    }
  }
  //  checks sideBtn status
  for (int i = 0 ; i < 3 ; i++) {
    if (digitalRead(sideBtn[i]) == 0) {
      sideStatus = i + 1;
      break;
    }
  }
  //send if status  != old status
  if (mainStatus != old_mainStatus) {
    switch (mainStatus)
    {
      case 1:
        msg = 'B';
        break;
      case 2:
        msg = 'E';
        break;
      case 3:
        msg = 'H';
        break;
    }
    Serial.print(msg);
  }

  if (s_state == READY) {
    if ((mainStatus == 1) || (mainStatus == 2)) {

      if (sideStatus == 1) {
        if (r_state == CW) {
          Serial.print('[');
        } else {
          Serial.print(']');
        }
      } else if (sideStatus == 2) {
        if (r_state == CW) {
          Serial.print('{');
        } else {
          Serial.print('}');
        }
      }
    } else if (mainStatus == 4) {
      if (sideStatus == 1) {
        if (r_state == CW) {
          Serial.print('C');
        } else {
          Serial.print('c');
        }
      } else if (sideStatus == 2) {
        if (r_state == CW) {
          Serial.print('S');
        } else {
          Serial.print('s');
        }
      } else if (sideStatus == 3) {
        if (r_state == CW) {
          Serial.print('V');
        } else {
          Serial.print('v');
        }
      }
    } else if (mainStatus == 5) {
      if (r_state == CW) {
        Serial.print('Y');
      } else {
        Serial.print('y');
      }
    }
    s_state == SENT;
  }
  old_mainStatus = mainStatus;
  old_sideStatus = sideStatus;
}

void doEncoderA() {

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
      r_state = CW;
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
      r_state = CCW;
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
      r_state = CW;
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
      r_state = CCW;
    }
  }
  s_state = READY;
  //  Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderB() {

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
      r_state = CW;
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
      r_state = CCW;
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
      r_state = CW;
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
      r_state = CCW;
    }
  }
  s_state = READY;
}
