//Adding LiquidCrystal.h library for use of 2X16 LCD
#include <LiquidCrystal.h>

//Defining LCD pins and parameters
const int rs = 12, en = 11, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Defining button pins
int buttonUp = 2;
int buttonDown = 3;
int buttonOk = 4;

//Variables to save user-entered speed and time data
int rpm = 100;
int time = 10;

//Defining flag variable for OK button and menu variable
int menu = 1;
int buttonOkFlag = 0;

void setup() {
  
//Setup Of Buttons  
  pinMode(buttonUp,INPUT);
  pinMode(buttonDown,INPUT);
  pinMode(buttonOk,INPUT);

// Setup Of 2x16 LCD
  lcd.begin(16,2);

//For start with Main Menu
  menuMain();

}

void loop() {

buttonUpFunc();
buttonDownFunc();
buttonOkFunc();
  

}

void menuMain(){

//This function is used to select submenus in the main menu according to the pressed buttons.

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

void menuExcuaction(){

//Allows access in submenus

  switch(menu){

    case 1:
    
    menuSpeedSetting();
    break;

    case 2:

    menuTimeSetting();
    break;

  }
}

void menuSpeedSetting(){

//Text and values to be displayed in the speed menu on the LCD screen

    lcd.clear();
    lcd.print("Speed :");
    lcd.setCursor(0,1);
    lcd.print(rpm);
    lcd.print(" RPM");

}

void menuTimeSetting(){

//Text and values to be displayed in the time menu on the LCD screen

  lcd.clear();
  lcd.print("Time :");
  lcd.setCursor(0,1);
  lcd.print(time);
  lcd.print(" Min.");


}

void downRpm(){

// Allows to lower the rpm

    if(rpm <= 700 && rpm >= 100){

      rpm -= 100;

    }

    else if(rpm < 0){

      rpm = 0;

    }

}

void upRpm(){

//Allows to upper the rpm

    if(rpm < 700){

      rpm += 100;

    }
}

void downTime(){

//Allows to lower the time

    if(time <=60 && time >= 10){

      time -= 10;

    }

    else if(time < 0){

      time = 0;

    }
}

void upTime(){

//Allows to upper the time

      if(time < 60){

      time += 10;

    }
}

void buttonUpFunc(){

//By changing the menu variable of the upper button, the user selects the submenus in the main menu and calls the function that increases the time or speed variables in the submenus.

  if(digitalRead(buttonUp) == HIGH){


    if(buttonOkFlag == 0){
        
        menu --;
        menuMain();
      
    }

    else if(buttonOkFlag == 1 && menu == 1){

        upRpm();
        menuExcuaction();

    }

    else if(buttonOkFlag == 1 && menu == 2){

        upTime();
        menuExcuaction();

    }

    while(digitalRead(buttonUp) == HIGH);

  }
}

void buttonDownFunc(){

//By changing the menu variable of the sub-button, it calls the function that increases the time or speed variables in the submenus, allowing the user to select sub-menus in the main menu.

  if(digitalRead(buttonDown) == HIGH){

    if(buttonOkFlag == 0){

      menu ++;
      menuMain();
      
    }
    
    else if(buttonOkFlag == 1 && menu  == 1){

      downRpm();
      menuExcuaction();

    }

    else if(buttonOkFlag == 1 && menu == 2){

      downTime();
      menuExcuaction();

    }

    while(digitalRead(buttonDown) == HIGH);

  }

}

void buttonOkFunc(){

//Changing the ok button's flag allows to enter or exit submenus

  if(digitalRead(buttonOk) == HIGH){


    if(buttonOkFlag == 0){

    buttonOkFlag = 1;
    menuExcuaction();

    }
    else{

      buttonOkFlag = 0;
      menuMain();

    }
    while(digitalRead(buttonOk) == HIGH);

  }
}