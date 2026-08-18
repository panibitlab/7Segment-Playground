const int a = 1;
const int b = 2;
const int c = 3;
const int d = 4;
const int e = 5;
const int f = 6;
const int g = 7;

const int btn = 8;

const int segments[7] = {a, b, c, d, e, f, g};

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


void showNumber(int number) {

  for (int i = 0; i < 7; i++) {digitalWrite(segments[i], numbers[number][i]);}
}


void clearDisplay() {

  for (int i = 0; i < 7; i++) {digitalWrite(segments[i], LOW);}
}


void showResult(int result) {

  for (int i = 0; i < 3; i++) {

    showNumber(result);
    delay(200);

    clearDisplay();
    delay(200);
  }
  showNumber(result);
}


void rollDice() {

  for (int i = 0; i < 20; i++) {
  
    int randomNumber = random(1, 7);
    showNumber(randomNumber);

    int speed = 50 + i * 10;
    delay(speed);
  }
  int result = random(1, 7);
  showResult(result);
}


void checkButton() {

  if (digitalRead(btn) == LOW) {

    rollDice();
    while (digitalRead(btn) == LOW) {delay(10);}
  }
}


void setup() {

  for (int i = 0; i < 7; i++) {pinMode(segments[i], OUTPUT);}
  pinMode(btn, INPUT_PULLUP);

  randomSeed(analogRead(A0));
}


void loop() {checkButton();}
