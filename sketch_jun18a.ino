#include <Servo.h>

Servo servo;

int Dat[8], pPWD[8] = {0,0,0,0,0,0,0,0};
int PWD[4][8], cPWD[4][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
const int SW1 = 2;
const int SW2 = 3;
const int SW3 = 4;
const int Servo_Pin = 5;
const int DSW1 = 6;
const int DSW2 = 7;
const int DSW3 = 8;
const int DSW4 = 9;
const int DSW5 = 10;
const int DSW6 = 11;
const int DSW7 = 12;
const int DSW8 = 13;
const int PR = A0;
const int LED = A1;

int lastButtonState1,lastButtonState2,lastButtonState3,Button1,Button2,Button3 = LOW;
int buttonState1,buttonState2,buttonState3,deci_val;
int pre_count, count, SW_Mode, Enter_Count, Whole_Mode, real_count = 0;

void setup()
{
  servo.attach(Servo_Pin);
  for(int i = 0; i<3; i++){
    pinMode(2+i,INPUT_PULLUP);
  }
  for(int j=0;j<8;j++){
    pinMode(6+j,INPUT);
  }
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void check_button_val(){
  buttonState1 = digitalRead(SW1); 
  buttonState2 = digitalRead(SW2); 
  buttonState3 = digitalRead(SW3); 
  if((buttonState1 != lastButtonState1)&&(buttonState1 == HIGH)){
    Button1 = HIGH;
    SW_Mode = 1;
  }
  else{
    Button1 = LOW;
  }
  
  if((buttonState2 != lastButtonState2)&&(buttonState2 == HIGH)){
    Button2 = HIGH;
    SW_Mode = 2;
  }
  else{
    Button2 = LOW;
  }
  
  if((buttonState3 != lastButtonState3)&&(buttonState3 == HIGH)){
    Button3 = HIGH;
    SW_Mode = 3;
  }
  else{
    Button3 = LOW;
  }
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
}

void loop()
{
  check_button_val();
  if (Whole_Mode==0) {
    Serial.println("Set Your Password.");
    servo.write(0);
    while (real_count < 4) {
      check_button_val();
      // Removed the while(!Button1) loop to automatically start password setting
      pre_count = 0;
      for (int i = 0; i < 8; i++) {
        pPWD[i] = digitalRead(6 + i);
        Serial.print(pPWD[i]);
        pre_count += pPWD[i];
      }
      Serial.println(" ");
      count = pre_count;
      delay(500);
      for(int i=0; i<8; i++){
        PWD[real_count][i] = pPWD[i];
      }
      Serial.println("Set DIP to ZERO");
      while (count != 0) {
        pre_count = 0;
        for (int i = 0; i < 8; i++) {
          pPWD[i] = digitalRead(6 + i);
          pre_count += pPWD[i];
        }
        count = pre_count;
      }
      real_count++;
    }
    Serial.println("Password Setting Complete!");
    Serial.print("PASSWORD");
    Serial.println(" ");
    for(int i=0;i<4;i++){
      for(int j = 0; j<8; j++){
        Serial.print(PWD[i][j]);
      }
      Serial.println(" ");
    }
    Serial.println(" ");
    Whole_Mode = 1;
  }
  else {
    for (int i = 0; i < 8; i++) {
      Dat[i] = digitalRead(6 + i);
    }

    if (Button1) {
      Serial.print("Input Mode");
      Serial.println(" ");
      servo.write(0);
      int SW1_count = 0;
      while(SW1_count<4){
        check_button_val();
        for (int i = 0; i < 8; i++) {
          Dat[i] = digitalRead(6 + i);
        }
        if(Button1){
          for(int i = 0; i<8; i++){
            cPWD[SW1_count][i] = Dat[i];
          }
          SW1_count++;
          Serial.println("NEXT PASSWORD");
          delay(500);
        }
      }
      Enter_Count = 0;
      for(int i = 0; i<4; i++){
        for(int j = 0; j<8; j++){
          if(PWD[i][j] == cPWD[i][j]){
            Enter_Count++;
          }
        }
      }
      if(Enter_Count == 32){
        Serial.println("Correct!");
        analogWrite(LED, 0);
        servo.write(180);
      }
      else{
        Serial.println("Wrong Password!");
        analogWrite(LED, 200);
      }
    }
    else if (Button2) {
      analogWrite(LED, 0);
      Serial.print("Setting Mode / Password");
      Serial.println(" ");
      servo.write(0);
      real_count = 0;
      for(int i = 0; i<4;i++){
        for(int j = 0; j<8; j++){
          PWD[i][j] = 0;
        }
      }
      while (real_count < 4) {
        check_button_val();
        while(!Button2) {
          check_button_val();
          pre_count = 0;
          for (int i = 0; i < 8; i++) {
            pPWD[i] = digitalRead(6 + i);
            Serial.print(pPWD[i]);
            pre_count += pPWD[i];
          }
          Serial.println(" ");
          count = pre_count;
          delay(500);
        }
        for(int i=0; i<8; i++){
          PWD[real_count][i] = pPWD[i];
        }
        Serial.println("Set DIP to ZERO");
        while (count != 0) {
          pre_count = 0;
          for (int i = 0; i < 8; i++) {
            pPWD[i] = digitalRead(6 + i);
            pre_count += pPWD[i];
          }
          count = pre_count;
        }
        real_count++;
      }
      Serial.println("Password Setting Complete!");
      Serial.print("PASSWORD");
      Serial.println(" ");
      for(int i=0;i<4;i++){
        for(int j = 0; j<8; j++){
          Serial.print(PWD[i][j]);
        }
        Serial.println(" ");
      }
      Serial.println(" ");
    }
    else if (Button3) {
      analogWrite(LED, 0);
      Serial.print("Check Mode / Password ");
      Serial.println(" ");
      servo.write(0);
      for (int i = 0; i < 4; i++) {
        for(int j = 0; j<8; j++){
          Serial.print(PWD[i][j]);
        }
        Serial.println(" ");
      }
      Serial.println(" ");
    }
  }
}
