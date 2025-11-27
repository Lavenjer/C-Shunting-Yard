#include <iostream>
#include <cmath>
#include <regex>
#include <map>
#include <stack>
#include <queue>
#include <list>
using namespace std;
regex operatorr("([+-x/])");
regex RPNnumber("^[0-9]+(\\.[0-9]+)?$");

list<string> ConvertToRPN(list<string> expression) 
///This function takes in the expression. The expression is a list of string tokens. The tokens either contain a number, decimal number, or an operator.
///It is like an element in an expression. This function uses a stack which stores the operators as it checks each token in the expression list.
///This function uses a stack, map (dictionary), lists, and regular expressions.
{
    map<string, int> precedence = { {"+",2}, {"-",2}, {"x",4}, {"/",4} };
    stack<string> operator_stack = {};
    list<string> RPNexpression = {};
    for (string token : expression) 
    {
        if (regex_match(token, RPNnumber)) //if token is a number
        {
            RPNexpression.push_back(token); //put number directly into RPN expression
        }
        else if (regex_match(token, operatorr)) //if token is an operator
        {
            while (!operator_stack.empty() && precedence[operator_stack.top()] >= precedence[token]) // run until operator stack is empty and precedence of the top of operator stack is greater than token's precedence
            {
                RPNexpression.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token); // if operator stack empty or precedence is lower, add token to operator stack
        }
    }
    while (!operator_stack.empty()) // clean out operator stack and pop all elements
    {
        string temp = operator_stack.top();
        operator_stack.pop();
        RPNexpression.push_back(temp);
    }
    return RPNexpression;
}

double EvaluateRPN(list<string> RPNexpression)
///This function evaluates RPN expressions. The RPNexpression is a list of elements but re-arranged into Reverse Polish Notation.
///It loops through the tokens in the expression and checks if they are a number or an operator. If the token is a number, push to number_stack
///If the token is an operator, pop two operands and perform a calculation and push the result back into the number_stack to be used again.
///This function uses stacks, lists, and regular expressions.
{
    stack<double> number_stack = {};
    for (string token : RPNexpression)
    {
        if (regex_match(token, RPNnumber)) // if token is a number
        {
            double token_float = std::stof(token); // convert the token from a string to a float and push to number_stack. Put in two lines instead of one for readability.
            number_stack.push(token_float);
        }
        else if (regex_match(token, operatorr)) // if token is an operator
        {
            double operand2 = number_stack.top();
            number_stack.pop();
            double operand1 = number_stack.top();
            number_stack.pop();
            if (token == "+")
            {
                number_stack.push(operand1 + operand2);
            }
            if (token == "-")
            {
                number_stack.push(operand1 - operand2);
            }
            if (token == "x")
            {
                number_stack.push(operand1 * operand2);
            }
            if (token == "/")
            {
                number_stack.push(operand1 / operand2);
            }
        }
    }
    double result = number_stack.top(); //After the loop, the only element in the number_stack should be the result only. Therefore it is checked and popped.
    number_stack.pop();
    return result;
}

void ConfirmExpression(string expression)
///This function checks if the input is a valid expression. If it isnt, program is halted with an error message.
///If not, approval message is sent and program continues
{
    regex pattern("^([0-9]+(\\.[0-9]+)?)([+\\-x/]([0-9]+(\\.[0-9]+)?))*$"); //Regular expression which checks if the einput is a valid expression
    if (regex_match(expression, pattern))
    {
        cout << "Valid Input\n"; //true
    }
    else 
    {
        cout << "Invalid Input\n";
        exit(1); //false
    }
}

list<string> ConvertToList(string input)
///This function converts the input into a list of tokens. The tokens will be each element of an expressions. It will split the expression
///based on operand and operator, not by character. Therefore decimal numbers and numbers take 1 token each. This will allow me to evaluate this
///using shunting yard.
///This function uses regular expressions and lists.
{
    list<string> expression = {};
    string num = "";
    regex number("^[0-9\\.]$");
    for (int index = 0; index < input.length(); index += 1) // run as many times as the length of the input
    {
        string input_character_string(1, input[index]);
        if (regex_match(input_character_string, number)) // if character is a number
        {
            num += input_character_string;
        }
        else if (input_character_string == ".") // if character is a "."
        {
            num += input_character_string;
        }
        else if (regex_match(input_character_string, operatorr)) //if character is an operator
        {
            expression.push_back(num);
            num = "";
            expression.push_back(input_character_string);
        }
    }
    if (!num.empty())
    {
        expression.push_back(num);
        num = "";
    }
    return expression;
}

int main () 
///This function is the main loop of the program. It will take in all the inputs and use the functions above to abstract the process.
///This function is mostly input and output statements since all the required values are obtained from the functions.
///This function uses lists.
{
    string input;
    cout << "Please input the infix expression:\n";
    cin >> input;
    ConfirmExpression(input);
    list<string> expression = ConvertToList(input);
    cout << "The converted expression to a list is: \n";
    for (string convert_token : expression)
    {
        cout << convert_token << " ";
    }
    cout << "\n";
    list<string> RPNexpression = ConvertToRPN(expression);
    cout << "the RPN expression is:\n";
    for (string token : RPNexpression)
    {
        cout << token << " ";
    }
    double result = EvaluateRPN(RPNexpression);
    cout << "\n\nThe result is: " << result;
    return 0;
}