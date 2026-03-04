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
