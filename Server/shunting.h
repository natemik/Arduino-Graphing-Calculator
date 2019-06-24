#ifndef _SHUNTING_H_
#define _SHUNTING_H_

using namespace std;

int precedence(char op);

double applyOperation(double num1, double num2, char op);

double shunting(string expression, double variable);

#endif