//Adding Counter Header to set time
#include "Countimer.h"

//Adding LiquidCrystal.h library for use 2X16 LCD
#include <LiquidCrystal_I2C.h>

Countimer timer;

//Defining button and buzzer pins
#define buttonUp 2
#define buttonOk 3
#define buttonDown 4
#define buzzer 9

//Defining motor control pin

#define motorPin 5

//Defining LCD parameter and adress

LiquidCrystal_I2C lcd(0x27,16,2);

//Some parameters for debouncing of butons
byte lastUpButtonState = HIGH;
byte lastDownButtonState = HIGH;
byte lastOkButtonState = HIGH;
unsigned long debounceDuration = 50; // millis
unsigned long lastTimeUpButtonStateChanged = 0;
unsigned long lastTimeDownButtonStateChanged = 0;
unsigned long lastTimeOkButtonStateChanged = 0;

//Define time parameters
int timeMins = 1;
int timeHours = 0;
int time = 10;

//Defining menu, button flag and rpm
int menu = 1;
int rpm = 300;
int buttonOkFlag = 0;


void setup() {
  
  //Begin serial monitor communication for some tests
  Serial.begin(9600);

  //Setup Of Buttons
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonOk,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);

  //Setup Buzzer Pin
  pinMode(buzzer,OUTPUT);

  //2x16 Lcd start
  lcd.begin();

  //For start with main menu
  menuMain();

  //triggers the timer to count seconds
  timer.setInterval(refreshClock,1000);


}

void loop() {

buttonUpFunc();
buttonDownFunc();
buttonOkFunc();
timer.run();
  
}

//It determines the main menus and refreshes the screen.
void menuMain(){

  switch(menu){

  case 0:

  menu = 3;
  lcd.clear();
  menuMain();
  break;
  
  case 1:

  lcd.clear();
  lcd.print("> Speed Setting");
  lcd.setCursor(0,1);
  lcd.print("  Time  Setting");
  break;

  case 2:

  lcd.clear();
  lcd.print("  Speed Setting");
  lcd.setCursor(0,1);
  lcd.print("> Time  Setting");
  break;

  case 3:

  lcd.clear();
  lcd.print("  Time Setting");
  lcd.setCursor(0,1);
  lcd.print("> Start");
  break;

  case 4:

  menu = 1;
  lcd.clear();
  menuMain();
  break;

}

}

//Provides access to submenus.
void menuExcuaction(){

  switch(menu){
    case 1:
    
    menuSpeedSetting();
    break;
    
    case 2:
    menuTimeSetting();
    break;

    case 3:

    menuStart();
    break;

  }
}

//Submenu for speed settings
void menuSpeedSetting(){

    lcd.clear();
    lcd.print("Speed :");
    lcd.setCursor(0,1);
    lcd.print(rpm);
    lcd.print(" RPM");

}

//Submenu for time settings
void menuTimeSetting(){

  lcd.clear();
  lcd.print("Time :");
  lcd.setCursor(0,1);
  lcd.print(timeHours);
  lcd.print(":");
  lcd.print(timeMins);

}

/*Desired time starts counting. It sends information every second and triggers a screen refresh. 
It determines the voltage value suitable for the desired speed and triggers the motor pin.*/

void menuStart(){

  lcd.clear();
  timer.setCounter(timeHours,timeMins,0, timer.COUNT_DOWN, onComplete);
  timer.setInterval(refreshClock,1000);
  if(!timer.isCounterCompleted()){
    
    int motorVoltage = map(rpm,100,700,65,230);
    analogWrite(motorPin,motorVoltage);
    timer.start();

  }

}

//It causes the display to show the set time decreasing every second.
void refreshClock(){
  
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Time:");
  lcd.setCursor(4,1);
  lcd.print(timer.getCurrentTime());

}

//Resets the arduino
void(* resetFunc) (void) = 0;

//Function that determines what will happen when the set time expires
void onComplete(){

  analogWrite(motorPin,0);

  if(buttonOkFlag == 1){
      for(int i = 0; i<3;i++){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("COMPLETED");
        digitalWrite(buzzer,HIGH);
        delay(1000);
        lcd.clear();
        digitalWrite(buzzer,LOW);
        delay(1000);
    }
  } 
  else if(buttonOkFlag == 0){
      for(int i = 0; i<3;i++){
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("EMERGENCY");
        digitalWrite(buzzer,HIGH);
        delay(1000);
        lcd.clear();
        digitalWrite(buzzer,LOW);
        delay(1000);
    }
  }

  lcd.clear();
  resetFunc();

}

//Down the rpm
void downRpm(){

    if(rpm <= 700 && rpm >= 110){
      rpm -= 10;
    }
    else if(rpm < 100){
      rpm = 100;
    }

}

//Up the rpm
void upRpm(){

    if(rpm < 700){
      rpm += 10;
    }
}

//Down the time
void downTime(){

  if(buttonOkFlag == 1){
    if(timeMins <= 60 && timeMins >= 2){
      timeMins--;
    }
    else if(timeMins < 0){
      timeMins = 0;
    }
  }
  else if(buttonOkFlag == 2){
    if(timeHours <=60 && timeHours >=1){
      timeHours --;
    }
    else if(timeHours <0){
      timeHours = 0;
    }
  }
}

//Up the time
void upTime(){
  
  if(buttonOkFlag == 1){
    if(timeMins <59){
      timeMins++;
    }
  }
  else if(buttonOkFlag == 2){
    if(timeHours < 59){
      timeHours++;;
    }
  }
}

/*By changing the menu variable of the upper button, the user selects the submenus 
in the main menu and calls the function that increases the some variables.Also,
this function prevents bouncing of keys.*/
void buttonUpFunc(){
  if (millis() - lastTimeUpButtonStateChanged > debounceDuration) {
    byte buttonUpState = digitalRead(buttonUp);
      if (buttonUpState != lastUpButtonState) {
        lastTimeUpButtonStateChanged = millis();
        lastUpButtonState = buttonUpState;
          if (buttonUpState == LOW) {
            digitalWrite(buzzer,HIGH);
            if(menu == 3 && buttonOkFlag == 1){

            }
            else if(buttonOkFlag == 0){
              menu --;
              menuMain();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag == 1 && menu == 1){
              upRpm();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag == 1 && menu ==2){
              upTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag == 2 && menu == 2){
              upTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag = 3 && menu == 2){
              upTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            digitalWrite(buzzer,LOW);
      }
    }
  }
}

/*By changing the menu variable of the sub-button, it calls he function that increas
that some variables in the submenus, allowing the user to select sub-menus in the
main menu.Also, this function prevents bouncing of keys.*/
void buttonDownFunc(){  
  if (millis() - lastTimeDownButtonStateChanged > debounceDuration) {
    byte buttonDownState = digitalRead(buttonDown);
      if (buttonDownState != lastDownButtonState) {
        lastTimeDownButtonStateChanged = millis();
        lastDownButtonState = buttonDownState;
          if (buttonDownState == LOW) {
            digitalWrite(buzzer,HIGH);
            if(buttonOkFlag == 0 ){
              menu ++;
              menuMain();
              digitalWrite(buzzer,LOW);
            }  
            else if(buttonOkFlag == 1 && menu == 1){
              downRpm();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }   
            else if(buttonOkFlag == 1 && menu == 2){
              downTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag == 2 && menu == 2){
              downTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            else if(buttonOkFlag == 3 && menu == 2){
              downTime();
              menuExcuaction();
              digitalWrite(buzzer,LOW);
            }
            digitalWrite(buzzer,LOW);   
        }
      }
    }
  }

//Changing the ok button's flag allows to enter or exit submenus.
void buttonOkFunc(){
  if (millis() - lastTimeOkButtonStateChanged > debounceDuration) {
    byte buttonOkState = digitalRead(buttonOk);
      if (buttonOkState != lastOkButtonState) {
        lastTimeOkButtonStateChanged = millis();
        lastOkButtonState = buttonOkState;
          if (buttonOkState == LOW) {
            digitalWrite(buzzer,HIGH);
            if(buttonOkFlag == 0){
              buttonOkFlag = 1;
              menuExcuaction();
              digitalWrite(buzzer,LOW);
              Serial.println(buttonOkFlag);
            }
            else if(buttonOkFlag == 1 && menu == 2){
              buttonOkFlag ++;
              menuExcuaction();
              digitalWrite(buzzer,LOW);
              Serial.println(buttonOkFlag);
            }
            else if(buttonOkFlag == 2 && menu == 2){
              buttonOkFlag = 0;
              menuMain();
              digitalWrite(buzzer,LOW);
              Serial.println(buttonOkFlag);
            }
            else if(buttonOkFlag == 1 && menu == 3){
                buttonOkFlag = 0;
                timer.setCounter(0,0,0, timer.COUNT_DOWN, onComplete);
                timer.setInterval(refreshClock,1000);
                Serial.println(buttonOkFlag);
            }
            else{
              buttonOkFlag = 0;
              menuMain();
              digitalWrite(buzzer,LOW);
              Serial.println(buttonOkFlag);
        }
      }
    }
  }
}