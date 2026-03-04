# 实验报告：银行业务模拟程序设计与实现

## 1. 问题描述

### 1.1 设计目标

本实验旨在设计一个银行业务的模拟程序，模拟客户在银行进行交易的过程。程序模拟银行的队列系统，客户根据自己的需求排队等候。每个客户在到达银行后，会根据自己的存款需求和银行的储备金进行交易。如果银行储备金足够满足客户需求，客户立即进行交易并离开；如果储备金不足，客户将被放入二队，等待再次处理。每位客户在银行的等待时间也需要被记录，最终输出所有客户的等待时间。

### 1.2 输入数据

输入数据包括：

- `N`：客户数量
- `total`：银行初始储备金
- `close_time`：银行的关闭时间（模拟时间）
- `average_time`：每个客户交易所需的时间（每个客户处理一次的时间间隔）

每个客户的记录包括：

- `amount`：客户需求的金额
- `time`：客户到达银行的时间

示例输入数据格式：

```
4 10000 600 10
-2000 0
-11000 10
-10000 30
2000 50
```

### 1.3 输出数据

输出数据包含每个客户的等待时间，以及所有客户的平均等待时间。

例如：

```
0 
590
30
0
155
```

表示每个客户的等待时间和所有客户的平均等待时间。

## 2. 算法描述

### 2.1 数据结构的描述

在这个模拟程序中，我们设计了一个 `Customer` 结构体和一个 `Queue` 类来表示客户和队列：

1. **Customer 结构体**：
   - `amount`：客户的需求金额。
   - `time`：客户到达银行的时间。
   - `wait_time`：客户在银行的等待时间。
2. **Queue 类**：
   - 使用循环队列来存储客户，采用动态内存分配的方式实现。
   - `quene`：客户队列的指针数组，用于存储客户对象。
   - `front`：队列的队首位置。
   - `rear`：队列的队尾位置。
   - `size`：队列当前的大小。
   - `capacity`：队列的最大容量。

队列操作包括：

- `enquene()`：将客户加入队列。
- `outquene()`：从队列中取出客户。
- `peek()`：查看队首客户。

```cpp
struct Quene {
    Customer **quene; // 存储客户的指针数组
    int front;        // 队首
    int rear;         // 队尾
    int size;         // 当前的队列大小
    int capacity;     // 队列容量

    // 初始化队列
    Quene(int num) {
        capacity = num;
        quene = (Customer**)malloc(capacity * sizeof(Customer*)); // 分配存储Customer指针的数组
        front = 0;
        rear = -1;
        size = 0;
    }

    // 判断队列为空
    bool isEmpty() {
        return size == 0;
    }

    // 判断队列为满
    bool isFull() {
        return size == capacity;
    }

    // 从队尾入队
    void enquene(Customer *c) {
        if (isFull()) {
            return;
        }
        rear = (rear + 1) % capacity;
        quene[rear] = c;
        size++;
    }

    // 从队首出队
    Customer *outquene() {
        if (isEmpty()) {
            return nullptr;
        }
        Customer *c = quene[front];
        front = (front + 1) % capacity;
        size--;
        return c;
    }

    // 查看队首元素
    Customer *peek() {
        if (isEmpty()) {
            return nullptr;
        }
        return quene[front];
    }
};
```

### 2.2 算法流程

1. **初始化**：首先，读取客户数据和银行初始的储备金。
2. **入队操作**：将所有客户按顺序加入队列 `q1`。
3. **银行处理过程**：
   - 每当有客户到达时，检查当前时间与客户的到达时间，若客户已经到达，则尝试为其办理业务。
   - 如果银行储备金足够满足客户需求，客户出队并更新储备金和等待时间。
   - 如果银行储备金不足，则将客户移至队列 `q2`，等待下一轮处理。
   - 若银行关闭时队列中仍有客户，则直接计算他们的等待时间。
4. **输出结果**：程序输出所有客户的等待时间，并计算并输出平均等待时间。

```cpp
void SimulateBank(int N, int total, int close_time, int average_time, Customer customers[]) {
    Quene q1(N);  // 初始队列
    Quene q2(N);  // 第二队列
    int Current_Time = 0;  // 当前时间
    int Total_Wait_Time = 0;  // 总等待时间
    int Total_Moeny = total;  // 银行储备金

    // 将客户按顺序入队
    for (int i = 0; i < N; i++) {
        q1.enquene(&customers[i]);
        customers[i].wait_time = 0;
    }

    while (!q1.isEmpty() || !q2.isEmpty()) {
        while (!q1.isEmpty() && Current_Time < close_time) {
            Customer *current_customer1 = q1.peek();
            Current_Time = Current_Time >= current_customer1->time ? Current_Time : current_customer1->time;

            if (current_customer1->amount + Total_Moeny >= 0) {
                q1.outquene();
                Total_Moeny += current_customer1->amount;
                current_customer1->wait_time = Current_Time - current_customer1->time;
                Current_Time += average_time;
            } else {
                q1.outquene();
                q2.enquene(current_customer1);
            }
        }

        while (!q2.isEmpty()) {
            Customer *current = q2.outquene();
            current->wait_time = close_time - current->time;
        }
    }
}
```

### 2.3 主函数

主函数负责读取输入文件、调用 `SimulateBank` 函数进行模拟，并将结果写入输出文件。

```cpp
int main() {
    std::ifstream inputFile("Bank_customer_input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open the input file!" << std::endl;
        return 1;
    }

    int N, total, close_time, average_time;
    inputFile >> N >> total >> close_time >> average_time;

    Customer *customers = new Customer[N];
    for (int i = 0; i < N; i++) {
        inputFile >> customers[i].amount >> customers[i].time;
    }

    SimulateBank(N, total, close_time, average_time, customers);

    inputFile.close();
    return 0;
}
```

------

### 2.6 全部代码

```cpp
#include<iostream>
#include<fstream>
#include <cstdlib>

struct Customer {
    int amount;    // 金额
    int time;      // 到达时间
    int wait_time; // 等候时间
};

struct Quene {
    Customer **quene; // 储存客户的指针数组
    int front;        // 队首
    int rear;         // 队尾
    int size;         // 当前的队列大小
    int capacity;     // 队伍容量

    // 初始化队列
    Quene(int num) {
        capacity = num;
        quene = (Customer**)malloc(capacity * sizeof(Customer*)); // 分配存储Customer指针的数组
        front = 0;
        rear = -1;
        size = 0;
    }

    // 判断队列为空
    bool isEmpty() {
        return size == 0;
    }

    // 判断队列为满
    bool isFull() {
        return size == capacity;
    }

    // 从队尾入队
    void enquene(Customer *c) {
        if (isFull()) {
            return;
        }
        rear = (rear + 1) % capacity;
        quene[rear] = c;
        size++;
    }

    // 从队首出队
    Customer *outquene() {
        if (isEmpty()) {
            return nullptr;
        }
        Customer *c = quene[front];
        front = (front + 1) % capacity;
        size--;
        return c;
    }

    // 查看队首元素
    Customer *peek() {
        if (isEmpty()) {
            return nullptr;
        }
        return quene[front];
    }
};

// 模拟银行
void SimulateBank(int N, int total, int close_time, int average_time, Customer customers[]) {
    // 建立两个队列
    Quene q1(N);
    Quene q2(N);

    int Current_Time = 0;     // 目前的时间
    int Total_Wait_Time = 0;  // 所有客户总等待时间
    int Total_Moeny = total;  // 目前银行储备金

    // 将客户按顺序入队
    for (int i = 0; i < N; i++) {
        q1.enquene(&customers[i]);
        customers[i].wait_time = 0;
    }

    while (!q1.isEmpty() || !q2.isEmpty()) {
        // 处理队列 1
        while (!q1.isEmpty() && Current_Time < close_time) {
            Customer *current_customer1 = q1.peek();

            int Fore_Total = Total_Moeny;

            // 更新目前的时间，如果他晚来，就以到达时间为准；早来则以前方处理时间为准
            Current_Time = Current_Time >= current_customer1->time ? Current_Time : current_customer1->time;

            // 客户需求能直接被满足，直接出队, 否则进入队 2
            if (current_customer1->amount + Total_Moeny >= 0) {
                q1.outquene();
                Total_Moeny += current_customer1->amount;

                current_customer1->wait_time = Current_Time - current_customer1->time;
                Current_Time += average_time;
            } else {
                q1.outquene();
                q2.enquene(current_customer1);
            }

            int q2_size = q2.size;
            // 处理队列 2
            for (int i = 0; Total_Moeny > Fore_Total && Current_Time < close_time && !q2.isEmpty() && i < q2_size; i++) {
                Customer *current_customer2 = q2.peek();

                Current_Time = Current_Time >= current_customer2->time ? Current_Time : current_customer2->time;

                // 客户需求能直接被满足，直接出队, 否则重新进入队列 2
                if (current_customer2->amount + Total_Moeny >= 0) {
                    q2.outquene();
                    Total_Moeny += current_customer2->amount;

                    current_customer2->wait_time = Current_Time - current_customer2->time;
                    Current_Time += average_time;
                } else {
                    q2.enquene(q2.outquene());
                }
            }
        }

        // 当 Current_Time > close_time 时，若还有在队列 2 中的，直接处理他们的 wait_time
        while (!q2.isEmpty()) {
            Customer *current = q2.outquene();
            current->wait_time = close_time - current->time;
        }
    }

    // 输出结果到文件
    std::ofstream outputFile("Bank_customer_waiting_output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Cannot open the output file!" << std::endl;
        return;
    }

    for (int i = 0; i < N; i++) {
        outputFile << customers[i].wait_time << std::endl;
        Total_Wait_Time += customers[i].wait_time;
    }
    outputFile << (Total_Wait_Time / N) << std::endl;
    outputFile.close();
}

int main() {
    std::ifstream inputFile("Bank_customer_input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open the input file!" << std::endl;
        return 1;
    }

    // 读取客户数据
    int N, total, close_time, average_time;
    inputFile >> N >> total >> close_time >> average_time;

    Customer *customers = new Customer[N];

    for (int i = 0; i < N; i++) {
        inputFile >> customers[i].amount >> customers[i].time;
    }

    SimulateBank(N, total, close_time, average_time, customers);

    inputFile.close();
    return 0;
}



```

------



## 3. 调试分析

### 3.1 测试数据选择

为了验证程序的正确性，我们使用了多个包含不同客户数量和需求金额的数据集进行测试。

#### 测试数据：

```
4 10000 600 10
-2000 0
-11000 10
-10000 30
2000 50
```

#### 预期输出：

```
0 
590 
30
0
155
```

### 3.2 调试问题

1. **队列操作**：初期在队列的出队操作中遇到了一些边界问题，导致客户未能正确处理。通过仔细检查队列的大小、前后指针的更新及循环队列的处理，解决了这一问题。
2. **时间计算**：在处理时间更新时，曾忽略了客户到达时间与当前时间的比较，导致客户等待时间计算错误。通过修正这一部分，确保客户在等待时能够正确地等待。



## 4. 算法的时空分析

### 4.1 时间复杂度

1. **入队与出队**：队列的入队和出队操作的时间复杂度为 O(1)。
2. **处理过程**：程序会在每个客户到达时处理一次，时间复杂度为 O(N)，其中 N 是客户的数量。

因此，总时间复杂度为 O(N)。

### 4.2 空间复杂度

1. **客户数据存储**：客户数据的存储需要 O(N) 空间。
2. **队列存储**：每个队列需要 O(N) 空间。

因此，总空间复杂度为 O(N)。



## 5. 测试结果及分析

### 5.1 测试数据

**输入数据**：

```
4 10000 600 10
-2000 0
-11000 10
-10000 30
2000 50
```

**输出数据**：

```
0 
590 
30
0
155
```

###  5.2 测试结果分析

程序能够正确模拟银行处理客户的过程。每个客户的等待时间根据其到达时间、银行的储备金和交易时间等因素进行计算。程序的输出符合预期，且能够正确计算客户的平均等待时间。



## 6. 实验体会和收获

通过本次实验，我深入理解了队列的应用，尤其是在模拟银行排队系统中的作用。栈和队列的基本操作在模拟过程中尤为重要，理解它们的基本操作和优化技巧对程序的效率提升至关重要。在调试过程中，我加强了对时间计算和队列操作的掌握，提升了编程和调试的能力。

