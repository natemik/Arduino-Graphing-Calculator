#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include <stdlib.h>
#include <SPI.h>
#include <string.h>


// Screen constants and global variables
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

#define BUTTON 2

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#define YP A2
#define XM A3
#define YM 5
#define XP 4
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define XOFFSET 160
#define YOFFSET 120

int MODE = 0;

bool touchFlag;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() {
    /*
        Function purpuse: required setup function for ardiuno code

        Parameters: None
        Return: None
    */
    init();
    pinMode(BUTTON , INPUT);
    digitalWrite(BUTTON , HIGH);
    Serial.begin(9600);
    Serial.flush();
    tft.begin();
    tft.setRotation(3);
    touchFlag = true;
}

// variable to help with printing input/output to the screen
int screenIndex = 0;


void drawCalculator(int CalcBGColor) {
    /*
        Function Purpose: Draw the calculator in modes 0 and 1

        Parameters: 
        CalcBGColor: The background color of the calculator

        Return: NONE
    */

    int CalcLineColor = ILI9341_WHITE;

    tft.fillScreen(ILI9341_BLACK);
    tft.fillRect(0,60,320,240,CalcBGColor);

    // Draws all vertical lines
    tft.drawFastVLine(53, 60, 180, CalcLineColor);
    tft.drawFastVLine(107, 60, 180, CalcLineColor);
    tft.drawFastVLine(161, 60, 180, CalcLineColor);
    tft.drawFastVLine(215, 60, 180, CalcLineColor);
    tft.drawFastVLine(269, 60, 180, CalcLineColor);

    // Draws all horizontal lines
    tft.drawFastHLine(0, 60, 360, CalcLineColor);
    tft.drawFastHLine(0, 195, 360, CalcLineColor);
    tft.drawFastHLine(0, 105, 360, CalcLineColor);
    tft.drawFastHLine(0, 151, 360, CalcLineColor);

    // Draws the first collum of numbers
    tft.drawChar(20, 70, '7', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(20, 120, '4', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(20, 165, '1', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(20, 210, '0', ILI9341_BLACK, CalcBGColor, 3);

    // Draws the second collum of numbers
    tft.drawChar(75, 70, '8', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(75, 120, '5', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(75, 165, '2', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(75, 210, '.', ILI9341_BLACK, CalcBGColor, 3);

    // Draws the third collum of numbers
    tft.drawChar(130, 70, '9', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(130, 120, '6', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(130, 165, '3', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(115, 210, 's', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(130, 210, 'i', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(145, 210, 'n', ILI9341_BLACK, CalcBGColor, 2);

    // Draws the fourth collum
    tft.drawChar(180, 70, '*', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(180, 120, '+', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(180, 165, '(', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(170, 210, 'c', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(185, 210, 'o', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(200, 210, 's', ILI9341_BLACK, CalcBGColor, 2);

    // Draws the fifth collum
    tft.drawChar(235, 70, 245, ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(235, 120, '-', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(235, 165, ')', ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(235, 210, '=', ILI9341_BLACK, CalcBGColor, 3);

    // Draws the sixth collum
    tft.drawChar(270, 70, 's', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(280, 70, 'e', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(290, 70, 't', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(305, 70, 'x', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(290, 120, 240, ILI9341_BLACK, CalcBGColor, 3);
    tft.drawChar(275, 165, 'a', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(290, 165, 'n', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(305, 165, 's', ILI9341_BLACK, CalcBGColor, 2);
    tft.drawChar(290, 210, 'x', ILI9341_BLACK, CalcBGColor, 3);

}


String arduino_readline() {
    // initialize the char read, length of line, character array and start_time for timeouts
    char read;
    uint16_t length = 0;
    char char_arr[100] = {0};
    // while true to read until either timeout or /n or /r is found
    while(true) {
        if(Serial.available()) {
        // read in a character
        read = Serial.read();
            // if the character was a newline or carriage return
            if(read == '\n' || read == '\r') {
                // convert the char array to a string and return it
                char_arr[length] = read;
                String read_line = String(char_arr);
                return(read_line);
            }
            else {
                // add the character to the char array and increment index of char array
                char_arr[length] = read;
                length++;
            }
        }
    }
}
// boolean used in the sending of functions to the server
bool functionFlag = true;
bool boundFlag = true;


void CalculatorTouch(int mode) {
    /*
        Function Purpose: Process the touches on the screen and send the neccesary characters to the server
        for what needs to be read from the serial port

        Parameters: 
        mode: the mode the calculator is currently in, either 0 for expressions or 1 for functions 
    */
    TSPoint touch = ts.getPoint();
    // if a function is being sent, preface it with an F
    if(!boundFlag && functionFlag && mode == 1) {
        functionFlag = false;
        Serial.print("F ");

    }
    if(touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
        if(touchFlag == false) {
        touchFlag = true;
        }
        return;
    }
    // variables to help with the printing of characters to the screen
    int numberSize = 5;
    int numberOffset = 30;

    // conversion from the touch values to usable x and y values
    int16_t screenX = map(touch.y, TS_MINY, TS_MAXY, DISPLAY_WIDTH-1, 0);
    int16_t screenY = map(touch.x, TS_MINX, TS_MAXX, 0, DISPLAY_HEIGHT-1);


    if(touchFlag == true) {
        if(screenX > 0 && screenX < 52 && screenY > 60 && screenY < 104) {
            Serial.print(7);
            tft.drawChar(screenIndex, 0, '7', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 0 && screenX < 52 && screenY > 105 && screenY < 150) {
            Serial.print(4);
            tft.drawChar(screenIndex, 0, '4', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 0 && screenX < 52 && screenY > 151 && screenY < 195) {
            Serial.print(1);
            tft.drawChar(screenIndex, 0, '1', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 53 && screenX < 106 && screenY > 60 && screenY < 104) {
            Serial.print(8);
            tft.drawChar(screenIndex, 0, '8', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 53 && screenX < 106 && screenY > 105 && screenY < 150) {
            Serial.print(5);
            tft.drawChar(screenIndex, 0, '5', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 53 && screenX < 106 && screenY > 151 && screenY < 195) {
            Serial.print(2);
            tft.drawChar(screenIndex, 0, '2', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 107 && screenX < 160 && screenY > 60 && screenY < 104) {
            Serial.print(9);
            tft.drawChar(screenIndex, 0, '9', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 107 && screenX < 160 && screenY > 105 && screenY < 150) {
            Serial.print(6);
            tft.drawChar(screenIndex, 0, '6', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 107 && screenX < 160 && screenY > 151 && screenY < 195) {
            Serial.print(3);
            tft.drawChar(screenIndex, 0, '3', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 0 && screenX < 52 && screenY > 196 && screenY < 240) {
            Serial.print(0);
            tft.drawChar(screenIndex, 0, '0', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 53 && screenX < 106 && screenY > 196 && screenY < 240) {
            Serial.print('.');
            tft.drawChar(screenIndex, 0, '.', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 161 && screenX < 216 && screenY > 60 && screenY < 104) {
            Serial.print(" ");
            Serial.print('*');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, '*', ILI9341_YELLOW, ILI9341_BLACK, 3);
            screenIndex += numberOffset-10;
        }
        else if(screenX > 161 && screenX < 216 && screenY > 105 && screenY < 150) {
            Serial.print(" ");
            Serial.print('+');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, '+', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 217 && screenX < 266 && screenY > 60 && screenY < 104) {
            Serial.print(" ");
            Serial.print('/');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, (char)245, ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 217 && screenX < 266 && screenY > 105 && screenY < 150) {
            Serial.print(" ");
            Serial.print('-');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, '-', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 161 && screenX < 216 && screenY > 151 && screenY < 195) {
            Serial.print(" ");
            Serial.print('(');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, '(', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 217 && screenX < 266 && screenY > 151 && screenY < 195) {
            Serial.print(" ");
            Serial.print(')');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, ')', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 267 && screenX < 320 && screenY > 151 && screenY < 195) {
            Serial.print(" ");
            Serial.print('A');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 'A', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 107 && screenX < 160 && screenY > 196 && screenY < 240) {
            Serial.print(" ");
            Serial.print('S');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 'S', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 161 && screenX < 216 && screenY > 196 && screenY < 240) {
            Serial.print(" ");
            Serial.print('C');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 'C', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 267 && screenX < 320 && screenY > 60 && screenY < 104) {
            Serial.print(" ");
            Serial.print('T');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 'T', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 267 && screenX < 320 && screenY > 105 && screenY < 150) {
            Serial.print(" ");
            Serial.print((char)240);
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 240, ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
        else if(screenX > 217 && screenX < 266 && screenY > 196 && screenY < 240) {
            Serial.println('=');
            // if mode is 0, get the result from the server and print it to the screen for 4 seconds
            if(mode == 0) {
                tft.fillRect(0, 0, 320, 60, ILI9341_BLACK);
                screenIndex = 0;
                delay(500);
                tft.setCursor(0,0);
                tft.setTextSize(numberSize);
                tft.println(arduino_readline());
                delay(4000);
                tft.fillRect(0, 0, 320, 60, ILI9341_BLACK);
            }
            // if the bound was just sent, reset the screen and wait for function
            else if(mode == 1 && !boundFlag){
                functionFlag = true;
                tft.fillRect(0, 0, 320, 60, ILI9341_BLACK);
                screenIndex = 0;
            }
        }  
        else if(screenX > 267 && screenX < 320 && screenY > 196 && screenY < 240) {
            Serial.print(" ");
            Serial.print('x');
            Serial.print(" ");
            tft.drawChar(screenIndex, 0, 'x', ILI9341_YELLOW, ILI9341_BLACK, numberSize);
            screenIndex += numberOffset;
        }
    }
    touchFlag = false;
}

// struct of an xy point used for graphing
struct xyPoint {
    long x;
    long y;
};

int getOneNumber() {
    /*
        Function Purpose: get a single number from the Serial

        Parameters: None
        Return: The number
    */
    String numPointsString = arduino_readline();
    Serial.println("A");
    return(numPointsString.toInt());
}

void drawGraph(long yBound, int xbound){
    /*
        Function Purpose: Draw the graph screen.

        Parameters:
        yBound: the upper/lower y bound of the function
        xBound: the upper/lower x bound of the function

        Returns: None
    */
    tft.fillScreen(ILI9341_BLACK);
    tft.drawFastHLine(0, 119, 320, ILI9341_RED);
    tft.drawFastVLine(159, 0, 240, ILI9341_RED);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(135, 0);
    tft.print(yBound + 10);
    tft.setCursor(135, 230);
    tft.print((-1)*(yBound + 10));
    tft.setCursor(0, 115);
    tft.print((-1)*xbound);
    tft.setCursor(305, 115);
    tft.print(xbound);
}


int main() {
  setup();
  drawCalculator(ILI9341_BLUE);
  while(true) {
    if(digitalRead(BUTTON) == 0 && MODE == 0) {
      MODE = 1;
      drawCalculator(ILI9341_GREEN);
    }
    // when the mode is 1 and the button is pressed, the client assumes that the server is ready to send the
    // data needed for a graph and will attempt to read said data
    else if(digitalRead(BUTTON) == 0 && MODE == 1) {
      MODE = 2;
      int num1Start = 0;
      int num1End = -1;
      int num2Start = -1;
      int num2End = -1;
      int numPoints = getOneNumber();
      int xbound = getOneNumber();
      long yBound = 0;
      int index = 0;
      xyPoint points[numPoints];
      for(int i = 0; i < numPoints; i++) {
        index = 0;
        String readLine = arduino_readline();
        num2End = readLine.length()-1;
        while(readLine.substring(index, index+1) != "\n") {
            if(readLine.substring(index, index+1) == " ") {
                num1End = index;
                break;
            }
            index++;
        }
        num2Start = num1End;
        String num1String = readLine.substring(num1Start, num1End);
        String num2String = readLine.substring(num2Start, num2End);
        long number1 = num1String.toInt();
        long number2 = num2String.toInt();
        if(abs(number2) > yBound){
            yBound = number2;
        }
        xyPoint newPoint;
        newPoint.x = number1;
        newPoint.y = number2;
        points[i] = newPoint;
        Serial.println("A");
      }
        drawGraph(yBound, xbound);
        for(int i = 0; i < numPoints - 1; i++) {
             tft.drawLine(((160)*points[i].x)/xbound + XOFFSET, ((-120)*points[i].y)/(yBound+10) + YOFFSET,
             ((160)*points[i+1].x)/xbound + XOFFSET,((-120)*points[i+1].y)/(yBound+10) + YOFFSET, ILI9341_GREEN);
        }
        functionFlag = true;
        boundFlag = true;
    }
    // if a graph is showing and the button is pressed, return to the default calculator
    else if(digitalRead(BUTTON) == 0 && MODE == 2) {
        MODE = 0;
        drawCalculator(ILI9341_BLUE);
    }
    if(MODE == 0) {
      CalculatorTouch(0);
      delay(300);
    }
    else if(MODE == 1) {
        // when on the function screen, first ask the user for the x bounds of the function
        if(boundFlag){
            tft.setCursor(0,0);
            tft.setTextSize(5);
            tft.setTextColor(ILI9341_WHITE);
            tft.print("BOUND?:");
            screenIndex = 200;
            Serial.print("B ");
            boundFlag = false;
        }
        CalculatorTouch(1);
        delay(300);
    }

  }

  return 0;
}
