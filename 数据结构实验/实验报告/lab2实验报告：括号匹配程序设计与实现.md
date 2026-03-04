# 实验报告：括号匹配程序设计与实现

## 1. 问题描述

### 1.1 设计目标

本实验的目标是设计并实现一个括号匹配的程序，检查给定的表达式中的括号是否正确匹配。要求处理不同类型的括号（圆括号 `()`、方括号 `[]`、大括号 `{}`），并判断其是否按正确的顺序和配对方式出现。

### 1.2 输入数据

输入数据由一个整数 `N` 和随后的 `N` 行字符串组成，每行字符串表示一个数学或编程表达式，可能包含多种括号，字符串以`@`结束。

例如：

```
3
2*[(x+y)/(1-x)]@
(25+x)*(a*(a+b+b)}@
{1+2[c-d(7/9)}@
```

### 1.3 输出结果

对于每个表达式，输出其括号是否匹配的结果：

- 如果括号正确匹配，输出 "YES"。
- 如果括号不匹配，输出 "NO"。

例如：

```
YES
NO
NO
```

## 2. 算法描述

### 2.1 数据结构的描述

在该程序中，我们使用了栈（`Stack`）来处理括号的匹配。栈能够有效地支持括号的后进先出（LIFO）特性，从而判断括号是否匹配。栈的结构包含以下元素：

- **data**: 存储栈元素的数组。
- **top**: 栈顶指针，指向栈顶的元素。
- **buttom**: 栈底指针，初始化为0。

栈的操作有以下几种：

- **push**: 将括号入栈。
- **pop**: 将栈顶元素出栈。
- **isEmpty**: 判断栈是否为空。

```cpp
struct Stack {
    char data[MAX_SIZE];
    int top;
    int buttom;

    Stack() : top(-1), buttom(0) {}

    void push(char c) {
        if (top < MAX_SIZE - 1) {
            data[++top] = c;
        } else {
            std::cerr << "栈已满" << std::endl;
        }
    }

    char pop() {
        if (top >= buttom) {
            return data[top--];
        } else {
            return '\0';
        }
    }

    bool isEmpty() {
        return top < buttom;
    }
};
```

### 2.2 算法流程

1. **读取输入**：首先读取整数 `N` 表示有 `N` 行表达式，然后逐行读取每个表达式。

2. 括号匹配

   ：对每个表达式进行处理：

   - 遍历每个字符，如果是左括号（`(`, `[`, `{`），则将其入栈。
   - 如果是右括号（`)`, `]`, `}`），则判断栈是否为空，以及栈顶元素是否匹配。

3. **输出结果**：根据括号是否匹配，输出 `YES` 或 `NO`。

### 2.3 优先级判断

为了支持不同类型括号的优先级比较，我们定义了一个简单的优先级函数：

```cpp
int Priority(char c) {
    if (c == '{') return 3;
    if (c == '[') return 2;
    if (c == '(') return 1;
}
```

在遍历括号时，若发现当前左括号的优先级大于栈顶括号的优先级，则返回 `"NO"`。

### 2.4 主要函数

`Bracket_pairing`函数用于判断给定的表达式中的括号是否正确匹配。函数通过栈来处理括号配对，逐个字符进行检查。

```cpp
const char* Bracket_pairing(const char* expression) {
    Stack stack;
    for (int i = 0; expression[i] != '@'; i++) {
        char c = expression[i];

        if (c == '(' || c == '[' || c == '{') {
            stack.push(c);
            if (stack.top > 0) {
                char rear = stack.data[stack.top - 1];
                if (Priority(c) > Priority(rear)) {
                    return "NO";
                }
            }
        } else if (c == ')' || c == ']' || c == '}') {
            if (stack.isEmpty()) {
                return "NO";
            }
            char top = stack.pop();

            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return "NO";
            }
        }
    }
    return stack.isEmpty() ? "YES" : "NO";
}
```

### 2.5 主函数

主函数负责处理文件的输入输出，并调用`Bracket_pairing`函数进行括号匹配判断。

```cpp
int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    if (!inputFile.is_open()) {
        std::cerr << "无法打开文件!" << std::endl;
    }

    int N;
    inputFile >> N;
    inputFile.ignore();

    for (int i = 0; i < N; i++) {
        char expression[MAX_SIZE];
        inputFile.getline(expression, MAX_SIZE);
        outputFile << Bracket_pairing(expression) << std::endl;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
```

------

### 2.6 全部代码

```cpp
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


```

------



## 3. 调试分析

### 3.1 测试数据选择

为了验证程序的正确性，选择了多种不同类型的表达式进行测试，确保程序能够正确判断各种括号配对的情况。

#### 测试数据：

```
3
2*[(x+y)/(1-x)]@
(25+x)*(a*(a+b+b)}@
{1+2[c-d(7/9)}@
```

#### 预期输出：

```
YES
NO
NO
```

### 3.2 程序调试中遇到的问题

1. **优先级问题**：在处理优先级时，误判断了不同类型括号的顺序，通过调试逐步修正了优先级比较的逻辑。

2. **判断逻辑问题**：在处理此处的括号配对判断时，

   ```cpp
           else if (c == ')' || c == ']' || c == '}') {
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
   ```

   我一开始并未考虑到碰到有括号是否要判断是否为空栈，导致怎么也运行不对，后续检查时才发现。



## 4. 算法的时空分析

### 4.1 时间复杂度

1. **遍历表达式**：对于每个表达式，程序遍历每个字符一次，时间复杂度为 O(m)，其中 m 是表达式的长度。
2. **栈操作**：栈的操作（`push` 和 `pop`）的时间复杂度为 O(1)。

因此，总时间复杂度为 O(m)，其中 m 是每个表达式的长度。

### 4.2 空间复杂度

栈的大小最大为表达式中的括号数量，空间复杂度为 O(m)，其中 m 是表达式的长度。



## 5. 测试结果及分析

### 5.1 测试数据

**输入数据**：

```
3
2*[(x+y)/(1-x)]@
(25+x)*(a*(a+b+b)}@
{1+2[c-d(7/9)}@
```

**输出数据**：

```
YES
NO
NO
```

### 5.2 测试结果分析

程序正确识别了括号配对的情况，对于合法的括号对输出 `YES`，对于不匹配的括号对输出 `NO`。结果符合预期。



## 6. 实验体会和收获

通过本次实验，我深入理解了栈数据结构的使用，特别是在括号匹配问题中的应用。栈通过先进后出的特性，能够高效地处理括号配对问题。在调试过程中，我意识到边界条件的重要性，确保栈的操作不发生越界，并合理处理不同类型括号的优先级。通过这次实验，我对栈的实现和应用有了更深入的理解，同时提高了自己的调试能力和程序设计能力。