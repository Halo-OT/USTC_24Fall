#include<iostream>
#include<fstream>
#include<cstdlib>

#define MAX_SIZE 1000
#define STACK_MAX_SIZE 1000

struct Stack{
    char data[MAX_SIZE];
    int top;
    int buttom;

    Stack() : top(-1), buttom(0) {}

    // 入栈
    void push(char c) {
        if (top < MAX_SIZE - 1) {
            data[++top] = c;
        } else {
            std::cerr << "栈已满" << std::endl;
        }
    }

    // 出栈
    char pop() {
        if (top >= buttom) {
            return data[top--];
        } else {
            return '\0';
        }
    } 

    // 空栈
    bool isEmpty() {
        return top < buttom;
    }
};

int Priority(char c) {
    if (c == '{') return 3;
    if (c == '[') return 2;
    if (c == '(') return 1;
}

const char* Bracket_pairing(const char* expression) {
    Stack stack;
    for (int i = 0; expression[i] != '@'; i++) {
        char c = expression[i];

        // 左括号入栈
        if (c == '(' || c == '[' || c == '{') {
            stack.push(c);
            if (stack.top > 0) {
                char rear = stack.data[stack.top - 1];
                // 判断括号优先级
                if (Priority(c) > Priority(rear)) {
                    return "NO";
                }
            }
        } else if (c == ')' || c == ']' || c == '}') {
            // 判断碰到右括号时是否为空栈
            if (stack.isEmpty()) {
                return "NO";
            }
            // 右括号出栈 比较
            char top = stack.pop();

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return "NO";
            }

        }
    }

    // 最后为空栈，说明匹配完成
    return stack.isEmpty() ? "YES" : "NO";
} 

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");
    
    // 打开失败
    if (!inputFile.is_open()) {
        std::cerr << "Can not open the file! " << std::endl;
    }

    // 读入 N 循环
    int N;
    inputFile >> N;
    inputFile.ignore();

    for (int i = 0; i < N; i++) {
        char expression[MAX_SIZE];
        // 逐行读入
        inputFile.getline(expression, MAX_SIZE);
        // 对应输出
        outputFile << Bracket_pairing(expression) << std::endl;
    }
    // 关闭文件
    inputFile.close();
    outputFile.close();

    return 0;
}

