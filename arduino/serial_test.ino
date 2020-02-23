
#define DATA_PIN 7   // Placeholder
#define CLOCK_PIN 5 // Placeholder
#define LATCH_PIN 6 // Placeholder

#define CUBE_SIZE 8

int led = LED_BUILTIN;
String request = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete






void errblinking() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
}
void blinking() {
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
}

layoutToLayer(String layout[]) {
  int i = 0;
  do {
    int power = 0;
    for (int j = CUBE_SIZE-1; j >= 0; j--) {

    }
  } while (++i < CUBE_SIZE);
}


/* Toggle a single layer.
 * Previously on layer will be turned off.
 */
int toggleLayer(int level) {
  if (level > 7) {
    return -1;
  }
  shift(1<<level); //PLEASE PLEASE PLEASE REMEMBER IT'S BIG ENDIAN WHEN WIRING!!!!
  return 0;
}


/* Display a single horizontal layer on the cube
 *
 * Example:
 * [255,129,129,129,129,129,129,255]
 *  to display an outline of a layer
 */
int displayLayer (unsigned char layer[], int level) {
  digitalWrite(LATCH_PIN, LOW); //hold low while transmitting bits
  toggleLayer(level);
  for (int i = 0; i < CUBE_SIZE; i++){
    shift(*(layer+i));

  }
  digitalWrite(LATCH_PIN, HIGH);
  return 0;
}


/* Shift 8 bytes at a time.
 * (unless you sport an exotic 14-bit shift register, which is rarer than unicorns)
 */
int shift (unsigned char line) {
  int nextBit;
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);

  //Big-endian style so that it comes out right side up
  for (int i = 7; i >= 0; i--) {
    digitalWrite(CLOCK_PIN, LOW); //release to get ready for next bit
    nextBit = (line & (1 << i)) && 1; //mask and check
    digitalWrite(DATA_PIN, nextBit);
    digitalWrite(CLOCK_PIN, HIGH); //register next bit
    digitalWrite(DATA_PIN, LOW); //release to prevent bleedthrough
  }
  digitalWrite(CLOCK_PIN, LOW); //stop reading
  return 0;
}


/* Display a single frame on cube
 *
 * Example:
 * [[255,129,129,129,129,129,129,255],
 *  [0,126,66,66,66,66,126,0],
 *  [0,0,60,36,36,60,0,0],
 *  [0,0,0,24,24,0,0,0],
 *  [0,0,0,0,0,0,0,0],
 *  [0,0,0,0,0,0,0,0],
 *  [0,0,0,0,0,0,0,0],
 *  [0,0,0,0,0,0,0,0]]
 *
 *  to display a hollow 4-layer piramid
 */

int display3D (unsigned char frame[8][8]) {
  for (int i = 0; i < CUBE_SIZE; i++) {
    displayLayer(frame[i], i);
  }
}
unsigned char line = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // reserve 200 bytes for the inputString:
  request.reserve(200);
}

void loop() {
  unsigned char input[] = {255,129,129,129,129,129,129,255};
  unsigned char input2[8][8] = {{255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255},
                              {255,255,255,255,255,255,255,255}};
  // displayLayer(input,2);
  display3D(input2);
  // digitalWrite(LATCH_PIN, LOW); //hold low while transmitting bits
  // toggleLayer(2);
  // shift(255);
  // shift(129);
  // shift(129);
  // shift(129);
  // shift(129);
  // shift(129);
  // shift(129);
  // shift(255);
  //
  //
  // digitalWrite(LATCH_PIN, HIGH);

  // print the string when a newline arrives:
  // readSerial();
  // if (stringComplete) {
  //   int req_len = request.length()+1;
  //   char query[req_len];
  //   request.toCharArray(query, req_len);
  //   if (!strcmp(query, "blink")) {
  //     blinking();
  //   }
  //   else if (!strcmp(query, "on")) {
  //     digitalWrite(led, HIGH);
  //   }
  //   else if (!strcmp(query, "off")) {
  //     digitalWrite(led, LOW);
  //   } else {
  //     errblinking();
  //   }
  //   request = "";
  //   stringComplete = 0;
  // }


  // digitalWrite(LATCH_PIN, LOW); //hold low while transmitting bits
  // shift(line);
  //
  //
  // delay(1000);
  // if (++line > 255) {
  //   line = 0;
  // }
  // digitalWrite(LATCH_PIN, HIGH);





}

void readSerial() {
  while (Serial.available()) {
    char nextChar = Serial.read();
    if (nextChar == '\n' || nextChar == '\0' || nextChar == '\000') {
      stringComplete = 1;
    } else {
      request += nextChar;
    }
  }
}
