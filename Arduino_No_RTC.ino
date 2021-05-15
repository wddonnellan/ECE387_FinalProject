#include <DFRobot_RGBMatrix.h> // Hardware-specific library

#define OE    9
#define LAT   10
#define CLK   11
#define A     A0
#define B     A1
#define C     A2
#define D     A3
#define E     A4
#define WIDTH 64 // # of LEDs vertically
#define HEIGHT  64 // # of LEDs horizontally
#define b1    50 // Hour and Day Set
#define b2    51 // Minute and Month Set
#define b3    52 // AM/PM and Year Set
#define enter 53 // Button pressed once desired date and time are set

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, HEIGHT);
int day_int = 1;
String day = String(day_int);

int month_int = 0;
char *Months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
String month = Months[month_int];

int year_int = 2000;
String year = String(year_int);

String Date = month + " " + day;

int hour_int = 1;
String hour = String(hour_int);

int minute_int = 0; 
String minute = String(minute_int);

int second_int = 0;
String second = String(second_int);

String Time = hour + ":" + minute;

bool AM = true;

int white = matrix.Color333(7, 7, 7);
int black = matrix.Color333(0, 0, 0);

void setup() {
  
  matrix.begin();//This line must come before modifying the matrix screen
  matrix.setTextSize(1);
  matrix.setCursor(1,1);
  
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(enter, INPUT);
  setTime_Date(); 
}

void loop() {
  Time_Date();
}


void setTime_Date() {
  Set_Time_toMatrix();
  while (digitalRead(enter) == LOW) {
    
    if (digitalRead(b1) == HIGH) {
      hour_int++;
      if (hour_int > 12) {
        hour_int = 1;
      }
      Set_Time_toMatrix();
      delay(50);
    }
    
    if (digitalRead(b2) == HIGH) {
      minute_int++;
      if (minute_int > 59) {
        minute_int = 0;
      }
      Set_Time_toMatrix();
      delay(50);
    }
      
     if (digitalRead(b3) == HIGH) {
      AM = !AM;
      Set_Time_toMatrix();
      delay(50);
     }
  }
  
  delay(100);
  Set_Date_toMatrix();
  
  while(digitalRead(enter) == LOW) {
     if (digitalRead(b1) == HIGH) {
        month_int++;
        if (month_int > 11) {
          month_int = 0;
        }
        Set_Date_toMatrix();
        delay(50);
     }
    
     if (digitalRead(b2) == HIGH) {
        day_int++;
        if (day_int > 31) {
          day_int = 1;
        }
        Set_Date_toMatrix();
        delay(50);
     } 
     if (digitalRead(b3) == HIGH) {
        year_int++;
        Set_Date_toMatrix();
        delay(50);
     }
  }
  Time_Date_toMatrix();
}

void Time_Date() {
  second_int++;
  if (second_int >= 60) {
    second_int = second_int - 60;
    minute_int++;
  }
  
  if (minute_int >= 60) {
    minute_int = 0;
    hour_int++;
  }
  
  if (hour_int == 13){
    hour_int = 1;
    AM = !AM;
    if (AM) {
      day_int++;
      if (month_int == 1 && day_int == 30 && (year_int % 4 == 0)) {
        month_int++;
        day_int = 1;
      } else if ( month_int == 1 && day_int == 29) {
        month_int++;
        day_int = 1;
      } else if (month_int == 0 || month_int == 2 || month_int == 4 || month_int == 6 || month_int == 7 || month_int == 9 || month_int == 11 && day_int == 32) {
        month_int++;
        day_int = 1; 
      } else if (day_int = 31) {
        month_int++;
        day_int = 1;
      }
      if (month_int == 12) {
        year_int++;
        month_int=0;
      }
    }
  }
  if (second_int == 0) {
    Time_Date_toMatrix();
  }
  delay(1000);
}

void Set_Time_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  matrix.print("Set Time:");
  hour = String(hour_int);
  if(minute_int > 9) {
    minute = String(minute_int);
  } else {
    minute= "0" + String(minute_int);
  }
  second = String(second_int);

  if(AM) {
    Time = hour + ":" + minute + " AM";
  }
  else {
    Time = hour + ":" + minute + " PM";
  }
  matrix.setCursor(1,17);
  matrix.print(Time);
}

void Set_Date_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  matrix.print("Set Date:");
  month = Months[month_int];
  day = String(day_int);
  year = String(year_int);

  Date = month + " " + day;
  matrix.setCursor(1,17);
  matrix.print(Date);
  matrix.setCursor(1,32);
  matrix.print(year);
}

void Time_Date_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  hour = String(hour_int);
  if(minute_int > 9) {
    minute = String(minute_int);
  } else {
    minute= "0" + String(minute_int);
  }
  second = String(second_int);

  if(AM) {
    Time = hour + ":" + minute + " AM";
  }
  else {
    Time = hour + ":" + minute + " PM";
  }

  month = Months[month_int];
  day = String(day_int);
  year = String(year_int);

  Date = month + " " + day;
  
  matrix.print(Time);
  matrix.setCursor(1,17);
  matrix.print(Date);
  matrix.setCursor(1,32);
  matrix.print(year);
}
