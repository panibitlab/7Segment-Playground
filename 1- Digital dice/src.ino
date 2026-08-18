const int a = 4;
const int b = 5;
const int c = 8;
const int d = 7;
const int e = 6;
const int f = 3;
const int g = 2;

const byte leds[] = {A5, A4, A3, A2};
const int segments[7] = {a, b, c, d, e, f, g};
const bool COMMON_ANODE = true;

const int btn = 10;

const bool numbers[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

// timers
unsigned long diceTimer = 0;
unsigned long ledTimer = 0;
unsigned long celebrateTimer = 0;

// states
bool rolling = false;
bool celebrating = false;

int rollStep = 0;
int finalResult = 0;

int ledIndex = 0;

void showNumber(int number){
  for(int i = 0; i < 7; i++){
    digitalWrite(segments[i], COMMON_ANODE ? !numbers[number][i] : numbers[number][i]);
  }
}

void clearDisplay(){
  for(int i = 0; i < 7; i++){
    digitalWrite(segments[i], COMMON_ANODE ? HIGH : LOW);
  }
}

// leds
void updateLedDance(int speed){
  if (millis() - ledTimer >= speed){
    ledTimer = millis();

    for(int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
    digitalWrite(leds[ledIndex], HIGH);

    ledIndex = (ledIndex + 1) % 4;
  }
}

int celebrateStep = 0;

void updateCelebrate(){
  if (!celebrating) return;

  if (millis() - celebrateTimer >= 300){
    celebrateTimer = millis();

    bool on = (celebrateStep % 2 == 0);
    for(int i=0;i<4;i++) digitalWrite(leds[i], on ? HIGH : LOW);

    celebrateStep++;

    if (celebrateStep >= 6) {   // 3 blinks
      celebrating = false;
      celebrateStep = 0;
    }
  }
}

void updateDice() {
  if (!rolling) return;

  int speed = 50 + rollStep * 10;

  if (millis() - diceTimer >= speed) {
    diceTimer = millis();

    int randomNumber = random(1,7);
    showNumber(randomNumber);

    updateLedDance(speed);

    rollStep++;

    if (rollStep >= 20) {
      rolling = false;
      finalResult = random(1,7);
      showNumber(finalResult);
      celebrating = true;
    }
  }
}

void checkButton() {
  static bool lastState = HIGH;
  bool current = digitalRead(btn);

  if (current == LOW && lastState == HIGH) {rolling = true; rollStep = 0;}
  lastState = current;
}

void setup() {
  for(int i=0;i<7;i++){pinMode(segments[i], OUTPUT); digitalWrite(segments[i], HIGH);}
  for(int i=0;i<4;i++) {pinMode(leds[i], OUTPUT);}

  pinMode(btn, INPUT_PULLUP);
  randomSeed(analogRead(A0));
}

void loop(){
  checkButton();
  updateDice();
  updateCelebrate();
}
