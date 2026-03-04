#include <cstdint>
#include <iostream>
#include <fstream>
#include <bitset>

#define LENGTH 1
#define MAXLEN 100
#define STUDENT_ID_SECRET 0x7c

int16_t lab1(int16_t n) {
       // initialize
    int16_t R0 = 0, R2 = 0, R1 = 0, R3 = 0;

    // R0 = A， R2 = B
    // 初始化R0
    R0 &= 0x0;  // R0 = 0
    R0 = R0 + n;
    // 初始化R2
    R2 &= 0x0;  // R2 = 0
    // ADD R2, R2, xf; 学号23151782 -> 0111 1100 -> x7c
    R2 = STUDENT_ID_SECRET; // R2 = 0x7C

    // A XOR B = (A AND NOT B) OR (NOT A AND B)
    // R0 = A, R2 = B


    // NOT R1, R2; R1 <- NOT R2
    R1 = ~R2;
    // AND R1, R2, R0; R1 = R0 AND NOT R2
    R1 = R1 & R0;
    // NOT R0, R0; R0 <- NOT R0
    R0 = ~R0;
    // AND R2, R0, R2; R2 <- NOT R0 AND R2
    R2 = R0 & R2;
    // ADD R3, R1, R2; R3 <- (R0 AND NOT R2) + (NOT R0 AND R2)
    R3 = R1 + R2;

    // return value
    return R3;
}

int16_t lab2(int16_t n) {
    int16_t R0 = n;  // 初始值 a0
    int16_t R2 = 0;  // 步数计数器 k
    int16_t R3 = 0;  // 临时寄存器用于检查奇偶性
    int16_t R4 = 0;  // 临时寄存器，用于计算 3 * a0 + 1 或 a0 / 2
    int16_t R5 = 0;  // 临时寄存器，用于一些计算
    int16_t result = 0;  // 最终的步数结果

    while (R0 != 1) {  // 循环直到 a0 为 1
        R2++;  // 步数计数器 k 自增

        // 判断是奇数还是偶数
        R3 = R0 & 1;  // 如果 R0 是奇数，R3 为 1，否则为 0
        if (R3 == 0) {  // 偶数
            // 使用加法替代除法 R0 = R0 / 2
            R4 = R0;
            R5 = 0;
            while (R4 >= 2) {
                R4 = R4 - 2;
                R5++;
            }
            R0 = R5;
        } else {  // 奇数
            // 使用加法替代乘法 R0 = 3 * R0 + 1
            R4 = R0;
            R4 = R4 + R4 + R4;  // 3 * R0
            R0 = R4 + 1;  // 3 * R0 + 1
        }
    }

    result = R2;  // 步数计数器即为最终的步数
    return result;  // 返回步数
}


int16_t lab3(char s1[], int n) {
    // 判断字符串是否为回文
    int start = 0;
    int end = n - 1;  // 假设 n 是字符串的长度

    while (start < end) {
        if (s1[start] != s1[end]) {
            return 0;  // 如果字符不匹配，返回 0，表示不是回文
        }
        start++;
        end--;
    }

    return 1;  // 如果所有字符都匹配，返回 1，表示是回文
}


// 计算挣钱
int16_t earn(int16_t n) {
    if (n == 0) {
        return 6;
    } else {
        return earn(n - 1) * 2;
    }
}

// 计算花钱
int16_t spend(int16_t n) {
    if (n == 0) {
        return 2;
    } else if (spend(n - 1) >= earn(n - 1)) {
        return 2;
    } else {
        return spend(n - 1) * 4;
    }
}

// 计算存款
int16_t savings(int16_t n) {
    if (n == 0) {
        return 10;
    } else {
        return savings(n - 1) + earn(n - 1) - spend(n - 1);
    }
}

int16_t lab4(int16_t n) {
    // 直接调用 savings 函数来计算存款
    return savings(n);  // 返回存款数额
}

int main() {
    std::fstream file;
    file.open("test.txt", std::ios::in);

    // lab1
    int16_t n = 0;
    std::cout << "===== lab1 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab1(n) << std::endl;
    }

    // lab2
    std::cout << "===== lab2 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab2(n) << std::endl;
    }

    // lab3
    std::cout << "===== lab3 =====" << std::endl;
    char s1[MAXLEN];
    for (int i = 0; i < LENGTH; ++i) {
        file >> s1 >> n;
        std::cout << lab3(s1, n) << std::endl;
    }
    
    // lab4
    std::cout << "===== lab4 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n ;
        std::cout << lab4(n) << std::endl;
    }
    
    file.close();
    return 0;
}