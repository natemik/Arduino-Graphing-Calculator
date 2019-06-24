#include "serialport.h"
#include "shunting.h"
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <utility>
#include <stack>
#include <math.h>

using namespace std;

int main() {
  // initializing all variables
  SerialPort port;
  string readLine;
  string expression;
  double x = 0;
  int sendCounter = 0;
  int BOUND =  0;
  vector<pair<double, double> > points;


  while(true) {

    readLine = port.readline(0);
    int length = readLine.length();
    expression = readLine.substr(0, length-2);

    // if the first character in an expression is an F, then treat the expression as a function
    if(readLine.substr(0, 1) == "F") {
        // compute y values for each x value from -BOUND to BOUND and keep track of 
        // how mnay points are computed.
        for(double i = (-1)*BOUND; i < BOUND + 1;) {
            double y = shunting(expression, i);
            points.push_back(make_pair(i, y));
            sendCounter++;
            i += 1;
        }
        // send the number of points and the bound to the client for graphing
        port.writeline(to_string(sendCounter));
        port.writeline("\n");
        port.readline();
        port.writeline(to_string(BOUND));
        port.writeline("\n");
        // send the xy pairs one by one to the server, waiting for an acknowledgement between each
        for(int j = 0; j < sendCounter; j++) {
            string pair = to_string(points[j].first) + " " + to_string(points[j].second) + "\n";
            if(port.readline() == "A\n") {
                port.writeline(pair);
            }
        }
        // read the last acknowledgement and clear the points
        port.readline();
        points.clear();
        sendCounter = 0;
    }
    // if the first character is a B, the line is only meant to be a single number, used as a bound
    // for a function
    else if(readLine.substr(0,1) == "B"){
        string bound = readLine.substr(4, length - 6);
        BOUND = stoi(bound);
    }
    // if there's no B or F, the expression if just a normal math expression, evaluated by shunting one time
    else {
        double RESULT = shunting(expression, x);
        // if the first character is a T, save the result for future use
        if(readLine[1] == 'T') {
          x = RESULT;
        }
        // send the result to the client to be shown.
        string ToSendFull = to_string(RESULT);
        int decInd = ToSendFull.find('.');
        string ToSend = ToSendFull.substr(0,decInd+3);
        port.writeline(ToSend);
        port.writeline("\n");
    }
  }
  return 0;
}
