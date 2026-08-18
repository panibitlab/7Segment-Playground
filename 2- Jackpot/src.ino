const int a = 13;
const int b = 11;
const int c = 8;
const int d = 9;
const int e = 10;
const int f = 12;
const int g = 7;

const int dig1 = 1;
const int dig2 = 2;
const int dig3 = 3;

const int red_led = 5;
const int green_led = 6;

const int btn = A5;

const int segments[7] = {a, b, c, d, e, f, g};
const int digits[3] = {dig1, dig2, dig3};

const bool numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};

void clearDigits() {

  for (int i = 0; i < 4; i++) {digitalWrite(digits[i], HIGH);}
}

void clearSegments() {

  for (int i = 0; i < 7; i++) {digitalWrite(segments[i], LOW);}
}

void showDigit(int digit, int number) {

  clearDigits();
  clearSegments();

  for (int i = 0; i < 7; i++) {digitalWrite(segments[i], numbers[number][i]);}

  digitalWrite(digits[digit], LOW);

  delay(2);
}

void refreshDigits(int values[], int count) {

  for (int i = 0; i < count; i++) {showDigit(i, values[i]);}
}

int randomSlotNumber() {

  int symbols[] = {
    1, 1, 1,
    2, 2, 2,
    3, 3, 3,
    4, 4, 4,
    5, 5, 5,
    6, 6,
    7, 7
  };

  int size = sizeof(symbols) / sizeof(symbols[0]);
  return symbols[random(0, size)];
}

int rollDigit(int digit, int values[], int previousCount) {

  int result;

  for (int i = 0; i < 22; i++) {
  
    result = randomSlotNumber();

    int speed = 35 + i * 10;
    unsigned long start = millis();

    while (millis() - start < speed) {refreshDigits(values, previousCount); showDigit(digit, result);}
  }
  return result;
}

void showFinalNumbers(int values[]) {

  unsigned long start = millis();

  while (millis() - start < 1200) {refreshDigits(values, 3);}
}

void showJackpot() {

  unsigned long start = millis();

  while (millis() - start < 2500) {showDigit(0, 7); showDigit(1, 7); showDigit(2, 7);}
}

void showWin() {

  unsigned long start = millis();

  while (millis() - start < 2000) {
  
    // eyes
    clearDigits();
    clearSegments();

    digitalWrite(a, HIGH);
    digitalWrite(d, HIGH);

    digitalWrite(dig1, LOW);
    delay(2);

    // smile
    clearDigits();
    clearSegments();

    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(c, HIGH);
    digitalWrite(d, HIGH);

    digitalWrite(dig2, LOW);
    delay(2);
  }
}

void showLose() {

  unsigned long start = millis();

  while (millis() - start < 2000) {
  
    // eyes
    clearDigits();
    clearSegments();

    digitalWrite(a, HIGH);
    digitalWrite(d, HIGH);

    digitalWrite(dig1, LOW);
    delay(2);


    // inverted smile
    clearDigits();
    clearSegments();

    digitalWrite(a, HIGH);
    digitalWrite(f, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(d, HIGH);

    digitalWrite(dig2, LOW);
    delay(2);
  }
}

void clearDisplay() {clearDigits(); clearSegments();}

void jackpot() {

  int values[3];
  // first num
  values[0] = rollDigit(0, values, 0);
  // second num
  values[1] = rollDigit(1, values, 1);
  //third num
  values[2] = rollDigit(2, values, 2);
  // result
  blinkLEDs(values);

  if (values[0] == 7 && values[1] == 7 && values[2] == 7) {
    //JACKPOT!!! 
    showJackpot();
  }
  else if (values[0] == values[1] && values[1] == values[2]) {showWin();}
  else {showLose();}

  clearDisplay();
}

void blinkLEDs(int values[]) {

  unsigned long start = millis();
  unsigned long previous = 0;

  bool state = false;

  while (millis() - start < 1200) {
  
    refreshDigits(values, 3);

    if (millis() - previous >= 200) {
      previous = millis();
      state = !state;

      digitalWrite(red_led, state);
      digitalWrite(green_led, !state);
    }
  }
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
}

void setup() {

  for (int i = 0; i < 7; i++) {pinMode(segments[i], OUTPUT);}
  for (int i = 0; i < 3; i++) {pinMode(digits[i], OUTPUT);}

  pinMode(btn, INPUT_PULLUP);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);

  clearDigits();
  clearSegments();

  randomSeed(analogRead(A0));
}

void loop() {

  if (digitalRead(btn) == LOW) {
    jackpot();
    //debounce
    while (digitalRead(btn) == LOW) {delay(10);}

    delay(500);
  }
}
