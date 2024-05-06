#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

int applyOperation(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

int evaluateExpression(const string& expression) {
    stack<int> values;
    stack<char> operators;

    stringstream ss(expression);
    char token;
    while (ss >> token) {
        if (isdigit(token)) {
            int num = token - '0';
            values.push(num);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();
                values.push(applyOperation(a, b, op));
            }
            if (!operators.empty())
                operators.pop(); // Discard '('
        } else if (token == '+' || token == '-' || token == '*' || token == '/') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                char op = operators.top();
                operators.pop();
                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        int b = values.top();
        values.pop();
        int a = values.top();
        values.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

int main() {
    string expression;
    cout << "Enter a mathematical expression: ";
    getline(cin, expression);

    int result = evaluateExpression(expression);
    cout << "Result: " << result << endl;

    return 0;
}
