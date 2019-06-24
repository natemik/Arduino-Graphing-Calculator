#include <string>
#include <string.h>
#include <vector>
#include <bits/stdc++.h>
#include <utility>
#include <stack>
#include <math.h>

// Pseudocode was used for all three functions from:
//  -https://www.geeksforgeeks.org/expression-evaluation/

using namespace std;

// global variable savinf the answer to the previously run expression
double answer = 0;


int precedence(char op) {
    /*
        Function purpose: the find the precedence of a given operation
        by PEDMAS, without exponentiation. The higher the precedence number
        the the sooner the operation is used in evaluation of the expression.

        Parameters:
        op: the character representing the operation the precedence is needed for

        Return: a number from 0 - 3 representing the precedence of op 
    */
    // by PEDMAS addition and subtraction have the lowest priority
    if(op == '+' || op == '-') {
        return 1;
    }
    // by Pedmas multiplication and division have the next highest priority
    else if(op == '*' || op == '/'  || op == (char)240) {
        return 2;
    }
    // while not a part of PEDMAS, sin and cosine need to have a higher priority
    // since for example if someone wants to get tan(45) you can do sin(45)/cos(45),
    // but you need to evalutate the trigonometric functions first
    else if(op == 'S' || op == 'C') {
      return 3;
    }
    return 0;
}


double applyOperation(double num1, double num2, char op) {
    /*
        Function Purpose: Applying mathematical operations to a pair of numbers or 
        a single number

        Parameters:
        num1: the first number the operation is applied to
        num2: the second number the operation is applied to
        op: the character representing the operation being applied

        Returns:
        the result of the operation.

        Note: any unary expressions(i.e. -, sin, cos) use num1 and discard the value of num2
        when evaluating.
    */
    switch(op) {
        case '+': return(num1 + num2);
        case '-': return(num1 - num2);
        case '*': return(num1 * num2);
        case '/': return(num1 / num2);
        case (char)240: return(num1*(-1));
        // sin and cos are meant to take in numbers in degrees but built-in sin and cos
        // use radians, hence the conversion made below.
        case 'S': return(sin(num1*3.141592/180));
        case 'C': return(cos(num1*3.141592/180));
        default: return 0;
    }
}


double shunting(string expression, double variable) {
    /*
        Function Purpose: The function takes a space seperated string to evaluate it and 
        uses a pair of stacks for conversion to reverse polish notation. the function utilizes
        the precedence and applyOperation functions described above to aid in this. The function
        supports the use of a variable for function computation and for using a saved number

        Parameters:
        expression: the space seperated string representing the maathematical expression that
        we want to evaluate
        variable: the number to be used in place of any 'x' found in the expression

        Returns: the result of the expression.
    */
    // the stacks used for conversion to reverse polish notation.
    stack <double> numbers;
    stack <char> operators;

    uint i;

    for(i = 0; i < expression.length(); i++) {
        // ' ', 'T', and 'F' are used by the server to tell what kind of expression is being evaluated,
        // but do not affect the result of the shunting function, so they are ignored
        if(expression[i] == ' ' || expression[i] == 'T' || expression[i] == 'F') {
            continue;
        }
        // if a left parenthesis if found place it on the operator stack
        else if(expression[i] == '(') {
            operators.push(expression[i]);
        }
        // this conditional is used for any character in the string that is used as a number, not 
        // an operation.
        else if(isdigit(expression[i]) || expression[i] == 'x' || expression[i] == '.' || expression[i] == 'A') {
            double newVal = 0;
            double newDecVal = 0;
            bool decFlag = false;
            int numDec = 0;

            while(i < expression.length() && (isdigit(expression[i]) || expression[i] == 'x' || expression[i] == '.' || expression[i] == 'A')) {
                // if there's an x then used the variable parameter in it's place
                if(expression[i] == 'x') {
                    newVal = variable;
                }
                // if there's an A, use the previous answer in it's place
                else if(expression[i] == 'A') {
                    newVal = answer;
                }
                else if(isdigit(expression[i])) {
                    // if a number is found save it in the newval variable unless the decFlag is true  
                    if(!decFlag) {
                        newVal = (newVal*10) + (expression[i] - '0');
                    }
                    // if decFlag is true, start saving the numbers into the newDecVal variable
                    else {
                        newDecVal = (newDecVal*10) + (expression[i] - '0');
                        numDec++;
                    }

                }
                else {
                    decFlag = true;
                }
                i++;
            }
            // convert the newDecVal into a number less than 1 by dividing by 10, since it was meant 
            // to be a decimal part of a number
            for(int j = 0; j < numDec; j++) {
                newDecVal = newDecVal/10;
            }
            newVal += newDecVal;
            numbers.push(newVal);
        }
        // if a right parenthesis is found, then do all operations in the operations stack until either
        // the stack is empty or a left parenthesis is found
        else if(expression[i] == ')') {
            while(!operators.empty() && operators.top() != '(') {
                char currentOper = operators.top();
                operators.pop();
                double num1, num2;
                // unary operators only use one number off the numbers stack
                if(currentOper == (char)240  || currentOper == 'S' || currentOper == 'C') {
                    num1 = numbers.top();
                    numbers.pop();
                }
                else {

                    num2 = numbers.top();
                    numbers.pop();

                    num1 = numbers.top();
                    numbers.pop();

                }
                numbers.push(applyOperation(num1, num2, currentOper));
            }

            operators.pop();
        }
        // if nothing above is found, run an operation off the stack.
        else {
            while(!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                char currentOper = operators.top();
                operators.pop();
                double num1, num2;

                if(currentOper == (char)240 || currentOper == 'S' || currentOper == 'C') {
                    num1 = numbers.top();
                    numbers.pop();
                }

                else {

                    num2 = numbers.top();
                    numbers.pop();

                    num1 = numbers.top();
                    numbers.pop();

                }
                numbers.push(applyOperation(num1, num2, currentOper));
            }

            operators.push(expression[i]);
        }
    }
    // when for loop is finished, evaluate operations until the operators stack is empty
    // then return and save the final answer.
    while(!operators.empty()) {
        char currentOper = operators.top();
            operators.pop();
            double num1, num2;

            if(currentOper == (char)240 || currentOper == 'S' || currentOper == 'C') {
                num1 = numbers.top();
                numbers.pop();
            }
            else {

                num2 = numbers.top();
                numbers.pop();

                num1 = numbers.top();
                numbers.pop();

            }

            numbers.push(applyOperation(num1, num2, currentOper));
    }
    answer = numbers.top();
    return(numbers.top());
}