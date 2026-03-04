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


