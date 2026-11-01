/*
This code allows you to draw on the LED matrix of the Arduino UNO R4 WiFi.

You will need at least 4 buttons. You can leave out either the up or down button, as well as either the left or right button. 
The program will still work without those two buttons.

In the Button Mapping section, you can change the pins used for the buttons.
In the Settings section, you can enable or disable the internal pull-up resistors and the start screen.

Please keep in mind that I'm not a professional programmer, so this code may not be the most efficient or elegant solution.

Feel free to modify this code and share your version on GitHub, as long as you give me credit.
*/

// ========== Libraries ==========
#include "Arduino_LED_Matrix.h"

// ========== Settings ==========
bool pullup = true; // By making this false you deactivate the intern pull-up resistors, and you will have to use external resistors.
bool showStartScreen = true; // By making this false you disable the start screen, and you won't see the drawing for 2,5 seconds.

// ========== Button Mapping ==========
byte upButton = 7;
byte downButton = 6;
byte leftButton = 5;
byte rightButton = 4;
byte hideShowButton = 3;
byte selectButton = 2;

// ========== LED Matrix ==========
ArduinoLEDMatrix matrix;
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte drawing[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte startScreen[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// ========== GVL ==========
int cursorPositionX = 5;
int cursorPositionY = 3;
bool cursorVisible = false;
bool showCursor = true;

bool upButtonChanged = false;
bool downButtonChanged = false;
bool leftButtonChanged = false;
bool rightButtonChanged = false;
bool hideShowButtonChanged = false;
bool selectButtonChanged = false;

bool lastUpButtonState = true;
bool lastDownButtonState = true;
bool lastLeftButtonState = true;
bool lastRightButtonState = true;
bool lastHideShowButtonState = true;
bool lastSelectButtonState = true;

unsigned long timing = 0;

// ========== Setup ==========
void setup(){
  if(pullup == false){
    pinMode(upButton, INPUT);
    pinMode(downButton, INPUT);
    pinMode(leftButton, INPUT);
    pinMode(rightButton, INPUT);
    pinMode(hideShowButton, INPUT);
    pinMode(selectButton, INPUT);
  }else{
    pinMode(upButton, INPUT_PULLUP);
    pinMode(downButton, INPUT_PULLUP);
    pinMode(leftButton, INPUT_PULLUP);
    pinMode(rightButton, INPUT_PULLUP);
    pinMode(hideShowButton, INPUT_PULLUP);
    pinMode(selectButton, INPUT_PULLUP);
  }

  matrix.begin();
  start();
}

// ========== Loop ==========
void loop(){
  cursorBlink();
  checkButtonUpdates();
  cursorMovement();
  drawPixel();
  showOrHideCursor();
  showDrawing();
  drawCursor();

  matrix.renderBitmap(frame, 8, 12);
}

// ========== Functions ==========
void cursorBlink(){
  if(drawing[cursorPositionY][cursorPositionX] == 1){
    if(millis() - timing >= 500){
      cursorVisible = false;
    }
    if(millis() - timing >= 800){
      cursorVisible = true;
      timing = millis();
    }
  }

  if(drawing[cursorPositionY][cursorPositionX] == 0){
    if(millis() - timing >= 500){
      cursorVisible = true;
    }
    if(millis() - timing >= 800){
      cursorVisible = false;
      timing = millis();
    }
  }
}

void checkButtonUpdates(){
  if(digitalRead(upButton) == LOW && lastUpButtonState == true){
    lastUpButtonState = false;
    upButtonChanged = true;
    delay(50);
  }else if(digitalRead(upButton) == HIGH && lastUpButtonState == false){
    lastUpButtonState = true;
    delay(50);
  }

  if(digitalRead(downButton) == LOW && lastDownButtonState == true){
    lastDownButtonState = false;
    downButtonChanged = true;
    delay(50);
  }else if(digitalRead(downButton) == HIGH && lastDownButtonState == false){
    lastDownButtonState = true;
    delay(50);
  }

  if(digitalRead(leftButton) == LOW && lastLeftButtonState == true){
    lastLeftButtonState = false;
    leftButtonChanged = true;
    delay(50);
  }else if(digitalRead(leftButton) == HIGH && lastLeftButtonState == false){
    lastLeftButtonState = true;
    delay(50);
  }

  if(digitalRead(rightButton) == LOW && lastRightButtonState == true){
    lastRightButtonState = false;
    rightButtonChanged = true;
    delay(50);
  }else if(digitalRead(rightButton) == HIGH && lastRightButtonState == false){
    lastRightButtonState = true;
    delay(50);
  }

  if(digitalRead(hideShowButton) == LOW && lastHideShowButtonState == true){
    lastHideShowButtonState = false;
    hideShowButtonChanged = true;
    delay(50);
  }else if(digitalRead(hideShowButton) == HIGH && lastHideShowButtonState == false){
    lastHideShowButtonState = true;
    delay(50);
  }

  if(digitalRead(selectButton) == LOW && lastSelectButtonState == true){
    lastSelectButtonState = false;
    selectButtonChanged = true;
    delay(50);
  }else if(digitalRead(selectButton) == HIGH && lastSelectButtonState == false){
    lastSelectButtonState = true;
    delay(50);
  }
}

void cursorMovement(){
  if(upButtonChanged == true){
    upButtonChanged = false;
    cursorPositionY = cursorPositionY - 1;
    if(cursorPositionY == -1){
      cursorPositionY = 7;
    }
    timing = millis() - 500;
  }

  if(downButtonChanged == true){
    downButtonChanged = false;
    cursorPositionY = cursorPositionY + 1;
    if(cursorPositionY == 8){
      cursorPositionY = 0;
    }
    timing = millis() - 500;
  }

  if(leftButtonChanged == true){
    leftButtonChanged = false;
    cursorPositionX = cursorPositionX - 1;
    if(cursorPositionX == -1){
      cursorPositionX = 11;
    }
    timing = millis() - 500;
  }

  if(rightButtonChanged == true){
    rightButtonChanged = false;
    cursorPositionX = cursorPositionX + 1;
    if(cursorPositionX == 12){
      cursorPositionX = 0;
    }
    timing = millis() - 500;
  }
}

void drawPixel(){
  if(selectButtonChanged == true){
    selectButtonChanged = false;
    drawing[cursorPositionY][cursorPositionX] = !drawing[cursorPositionY][cursorPositionX];
    cursorVisible = drawing[cursorPositionY][cursorPositionX];
    timing = millis();
  }
}

void showOrHideCursor(){
  if(hideShowButtonChanged == true){
    hideShowButtonChanged = false;
    showCursor = !showCursor;
  }
}

void showDrawing(){
  for(int y = 0; y < 8; y++){
    for(int x = 0; x < 12; x++){
      frame[y][x] = drawing[y][x];
    }
  }
}

void drawCursor(){
  if(showCursor == true){
    frame[cursorPositionY][cursorPositionX] = cursorVisible;
  }
}

void start(){
  if(showStartScreen == true){
    matrix.renderBitmap(startScreen, 8, 12);
    delay(2500);
  }
}