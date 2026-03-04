# 实验报告：行编辑器程序设计与实现

## 1. 问题描述

### 1.1 设计目标

本实验的目标是设计并实现一个简单的文本编辑器，支持以下基本操作：

- **Move**: 将光标移动到文本中的指定位置。
- **Insert**: 在光标位置插入指定长度的字符串。
- **Delete**: 删除光标后指定长度的字符。
- **Rotate**: 反转光标后指定长度的字符。
- **Prev**: 光标前移一个字符。
- **Next**: 光标后移一个字符。
- **Get**: 输出当前文本。

### 1.2 输入数据

输入数据包含多个操作命令。每条命令由命令类型和必要的参数组成，格式如下：

- **Move k**：将光标移动到第k个字符之后。
- **Insert n S**：在光标位置插入一个长度为n的字符串S。
- **Delete n**：删除光标后n个字符。
- **Rotate n**：反转光标后n个字符。
- **Prev**：将光标前移一个字符。
- **Next**：将光标后移一个字符。
- **Get**：输出当前文本。

### 1.3 输出结果

输出文件记录了每次执行`Get`命令时的文本内容。每次调用`Get`时，文本中的内容将被写入文件中，且每次文本后都换行。

## 2. 算法描述

### 2.1 数据结构的描述

在该程序中，我们使用了一个**数组**来存储文本内容。光标的位置和文本的当前长度是程序的关键变量。具体数据结构如下：

- **text**: 一个字符数组，用于存储当前文本内容。最大长度为`MAX_TEXT_SIZE`（1000）。
- **cursor**: 一个整数，表示光标的位置。
- **length**: 一个整数，表示当前文本的长度。

```cpp
const int MAX_TEXT_SIZE = 1000; // 最大文本长度
const int MAX_COMMAND_SIZE = 50; // 最大命令长度

class TextEditor {
private: 
    char text[MAX_TEXT_SIZE];  // 文本内容
    int cursor;  // 光标位置
    int length;  // 文本当前长度
public:
    // 构造函数
    TextEditor();
    void move(int k);
    void insert(int n, const char* s);
    void deleteChars(int n);
    void rotate(int n);
    void get(std::ofstream& outputFile);
    void prev();
    void next();
};
```

### 2.2 程序结构的描述

程序的主要功能通过`TextEditor`类实现，类中包含多个成员函数来处理不同的文本编辑命令。

1. **move(int k)**: 将光标移动到第k个字符之后，如果k为0，则将光标移到文本的开头。
2. **insert(int n, const char* s)**: 在光标位置插入一个长度为n的字符串。
3. **deleteChars(int n)**: 删除光标后n个字符。
4. **rotate(int n)**: 反转光标后n个字符。
5. **get(std::ofstream& outputFile)**: 输出当前文本到文件。
6. **prev()**: 光标前移一个字符。
7. **next()**: 光标后移一个字符。

```cpp
bool compareCommand(const char* inputCommand, const char* expectedCommand);
int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");
    
    // 读取命令并执行相应操作
}
```

### 2.3 命令比较

为了判断命令是否匹配，我们实现了一个辅助函数`compareCommand`，通过逐字符比较输入命令和预期命令是否一致。

```cpp
bool compareCommand(const char* inputCommand, const char* expectedCommand);
```

### 2.4 主函数流程

主函数负责读取输入命令，创建`TextEditor`对象并根据不同的命令调用对应的成员函数。命令格式如下：

1. 读取命令类型。
2. 根据命令类型读取相应参数并调用编辑器方法。
3. 执行`Get`命令时，输出当前文本到文件。

```cpp
int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    int numCommands;
    inputFile >> numCommands;
    inputFile.ignore(); // 忽略换行符

    TextEditor editor;
    for (int i = 0; i < numCommands; ++i) {
        char command[MAX_COMMAND_SIZE];
        inputFile >> command;

        if (compareCommand(command, "Move")) {
            int k;
            inputFile >> k;
            editor.move(k);
        } else if (compareCommand(command, "Insert")) {
            int n;
            char s[MAX_COMMAND_SIZE];
            inputFile >> n;
            inputFile.ignore(); // 忽略空格
            for (int j = 0; j < n; ++j) {
                s[j] = inputFile.get(); // 逐个读取字符
            }
            s[n] = '\0'; // 添加字符串结束符
            editor.insert(n, s);
        } else if (compareCommand(command, "Delete")) {
            int n;
            inputFile >> n;
            editor.deleteChars(n);
        } else if (compareCommand(command, "Rotate")) {
            int n;
            inputFile >> n;
            editor.rotate(n);
        } else if (compareCommand(command, "Get")) {
            editor.get(outputFile);
        } else if (compareCommand(command, "Prev")) {
            editor.prev();
        } else if (compareCommand(command, "Next")) {
            editor.next();
        }
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
```

------

### 2.5 全部代码

```cpp
#include <iostream>
#include <fstream>

const int MAX_TEXT_SIZE = 1000;  // 假设最大文本长度为1000
const int MAX_COMMAND_SIZE = 50; // 假设命令字符串的最大长度

class TextEditor {
private: 
    char text[MAX_TEXT_SIZE];
    int cursor;
    int length; // 当前文本长度

public:
    TextEditor(): cursor(0), length(0) {
        text[0] = '\0';
    }

    // 将光标移动到第 k 个字符之后，如果 k=0 则将光标移到文本第一个字符之前
    void move(int k) {
        if (k >= 0 && k <= length) {
            cursor = k;
        } else if (k > length) {
            cursor = length;
        } else {
            cursor = 0;
        }
    }

    // 在光标后插入长度为 n 的字符串 S，光标位置不变，n≥1
    void insert(int n, const char* s) {
        if (length + n > MAX_TEXT_SIZE) {
            return;
        }
        // 光标后内容向后移动n个单位
        for (int i = length - 1; i >= cursor; --i) {
            text[i + n] = text[i];
        }
        // 向光标后插入S
        for (int i = 0; i < n; ++i) {
            text[cursor + i] = s[i];
        }
        length += n;
        text[length] = '\0';
    }

    // 删除光标后的 n 个字符，光标位置不变，n≥1
    void deleteChars(int n) {
        if (n <= 0 || cursor >= length) return;
        int deleteCount = (cursor + n > length) ? length - cursor : n;
        for (int i = cursor; i + deleteCount < length; ++i) {
            text[i] = text[i + deleteCount];
        }
        length -= deleteCount;
        text[length] = '\0';
    }

    // 反转光标后的 n 个字符，光标位置不变，n≥1
    void rotate(int n) {
        if (n <= 0 || cursor + n > length) return;
        for (int i = 0; i < n / 2; ++i) {
            char temp = text[cursor + i];
            text[cursor + i] = text[cursor + n - i - 1];
            text[cursor + n - i - 1] = temp;
        }
    }

    // 输出此时程序内保存的文本
    void get(std::ofstream& outputFile) {
        for (int i = 0; i < length; ++i) {
            outputFile.put(text[i]);
        }
        outputFile.put('\n');
    }

    // 光标前移一个字符
    void prev() {
        if (cursor > 0) {
            cursor--;
        }
    }

    // 光标后移一个字符
    void next() {
        if (cursor < length) {
            cursor++;
        }
    }
};

bool compareCommand(const char* inputCommand, const char* expectedCommand) {
    int i = 0;
    while (inputCommand[i] != '\0' && expectedCommand[i] != '\0') {
        if (inputCommand[i] != expectedCommand[i]) {
            return false;
        }
        ++i;
    }
    return inputCommand[i] == '\0' && expectedCommand[i] == '\0';
}

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    int numCommands;
    inputFile >> numCommands;
    inputFile.ignore(); // 忽略换行符

    TextEditor editor;
    for (int i = 0; i < numCommands; ++i) {
        char command[MAX_COMMAND_SIZE];
        inputFile >> command;

        if (compareCommand(command, "Move")) {
            int k;
            inputFile >> k;
            editor.move(k);
        } else if (compareCommand(command, "Insert")) {
            int n;
            char s[MAX_COMMAND_SIZE];
            inputFile >> n;
            inputFile.ignore(); // 忽略空格
            for (int j = 0; j < n; ++j) {
                s[j] = inputFile.get(); // 逐个读取字符
            }
            s[n] = '\0'; // 添加字符串结束符
            editor.insert(n, s);
        } else if (compareCommand(command, "Delete")) {
            int n;
            inputFile >> n;
            editor.deleteChars(n);
        } else if (compareCommand(command, "Rotate")) {
            int n;
            inputFile >> n;
            editor.rotate(n);
        } else if (compareCommand(command, "Get")) {
            editor.get(outputFile);
        } else if (compareCommand(command, "Prev")) {
            editor.prev();
        } else if (compareCommand(command, "Next")) {
            editor.next();
        }
    }

    inputFile.close();
    outputFile.close();
    return 0;
}

```

------



## 3. 调试分析

### 3.1 测试数据选择

测试数据的选择应该涵盖所有可能的操作，确保每种功能都能正常工作。测试数据包括：

1. 光标移动：测试`Move`命令，确保光标可以正确移动到指定位置。
2. 字符插入：测试`Insert`命令，确保字符串能够正确插入到光标位置。
3. 字符删除：测试`Delete`命令，确保删除操作能按预期工作。
4. 字符旋转：测试`Rotate`命令，确保文本内容能被正确反转。
5. 输出：通过`Get`命令，验证输出的文本是否符合预期。

### 3.2 程序调试中遇到的问题

在调试过程中，最常见的问题包括：

- **光标超出范围**：例如在执行`move`或`delete`命令时，光标可能会越界。通过增加检查条件来解决这个问题。
- **字符插入超出文本范围**：插入的字符串长度可能超出了预设的最大文本长度。解决方法是加入对最大长度的检查。



## 4. 算法的时空分析

### 4.1 时间复杂度

1. **move(k)**: 时间复杂度为O(1)，因为移动光标只是修改一个整数值。
2. **insert(n, s)**: 时间复杂度为O(n)，需要移动数组中的元素来腾出空间并插入新字符。
3. **deleteChars(n)**: 时间复杂度为O(n)，需要移动数组中的元素来删除字符。
4. **rotate(n)**: 时间复杂度为O(n)，需要遍历指定的字符段并交换它们的位置。
5. **get(outputFile)**: 时间复杂度为O(m)，其中m为文本的当前长度。

### 4.2 空间复杂度

程序的空间复杂度主要由`text`数组决定，空间复杂度为O(MAX_TEXT_SIZE)，即O(1000)。



## 5. 测试结果及分析

### 5.1 测试数据

测试数据包括一系列`Move`、`Insert`、`Delete`、`Rotate`等命令。每次执行`Get`命令时，程序应输出当前的文本内容。

**输入数据：**

```
12
Insert 13
Balanced eert
Get
Move 2
Delete 5
Get
Next
Insert 6
editor
Move 0
Get
Move 10
Rotate 4
Get
```

**输出数据：**

```
Balanced eert
Bad eert
Badeditor eert
Badeditor tree
```

### 5.2 测试结果分析

测试结果表明，程序能够正确处理所有操作，包括光标移动、字符串插入、字符删除和旋转操作。`Get`命令输出了正确的文本内容。



## 6. 实验体会和收获

通过本次实验，我更深入理解了文本编辑器的基本操作和数据结构的设计。在实际编程中，细节问题（如边界条件的处理）对程序的正确性至关重要。通过调试和多次测试，我能够更好地把握程序的执行流程，提高了自己的编程能力和调试技巧。