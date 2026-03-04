#include <iostream>
using namespace std;

// 哈希函数
int hashfunction(int key, int p) {
    return key % p;
}

// 线性探测哈希表
class HashTableLinear {
private:
    int* table;     // 存储数据
    bool* occupied; // 当前位置是否被占用
    int size;       // 表的大小
    int p;          // 哈希表的模数

public:
    // 构造函数
    HashTableLinear(int TableSize, int mod): size(TableSize), p(mod) {
        table = new int[size];
        occupied = new bool[size];

        // 初始化哈希表
        for (int i = 0; i < size; i++) {
            table[i] = -1;          // -1 表示空槽
            occupied[i] = false;
        }
    }

    // 插入元素
    void insert(int key) {
        int index = hashfunction(key, p);
        while (occupied[index]) {
            index = (index + 1) % size;
        }
        table[index] = key;
        occupied[index] = true;
    }

    // 查找元素，返回成功与否以及探测次数
    int search(int key, bool& success) {
        int index = hashfunction(key, p);
        int probes = 0;
        while (probes < size) {
            probes++;
            if (!occupied[index]) {
                success = false;
                return probes; // 查找失败
            }
            if (table[index] == key) {
                success = true;
                return probes; // 查找成功
            }
            index = (index + 1) % size;
        }
        success = false;
        return probes;
    }

    // 计算失败查找次数：对于有关键字的失败查找，查找会遇到空槽
    int getFailureProbes(int key) {
        int index = hashfunction(key, p);
        int probes = 0;
        while (probes < size) {
            probes++;
            if (!occupied[index]) {
                return probes;  // 遇到空槽，返回失败探测次数
            }
            index = (index + 1) % size;
        }
        return probes;
    }

    // 打印哈希表并计算探测次数
    void printTable() {
        cout << "哈希表的地址:\t";
        for (int i = 0; i < size; i++) {
            cout << i << "\t";
        }
        cout << endl;

        cout << "表中的关键字:\t";
        for (int i = 0; i < size; i++) {
            if (occupied[i]) {
                cout << table[i] << "\t";
            } else {
                cout << "-\t";
            }
        }
        cout << endl;

        // 初始化成功和失败查找次数数组
        int* successProbes = new int[size]();
        int* failureProbes = new int[size]();

        // 计算每个元素的成功查找次数
        for (int i = 0; i < size; i++) {
            if (occupied[i]) {
                bool success;
                successProbes[i] = search(table[i], success);
            }
        }

        // 计算每个元素的失败查找次数
        for (int i = 0; i < size; i++) {
            if (occupied[i]) {
                failureProbes[i] = size - i - 1;
            } else {
                // 空槽的失败查找次数为 1
                failureProbes[i] = 1;
            }
        }

        // 输出探测次数
        cout << "成功查找次数:\t";
        for (int i = 0; i < size; i++) {
            cout << successProbes[i] << "\t";
        }
        cout << endl;

        cout << "失败查找次数:\t";
        for (int i = 0; i < size; i++) {
            cout << failureProbes[i] << "\t";
        }
        cout << endl;

        // 计算平均查找长度
        double successTotal = 0, failureTotal = 0;
        int successCount = 0, failureCount = 0;

        for (int i = 0; i < size; i++) {
            if (occupied[i]) {
                successTotal += successProbes[i];
                successCount++;
            }
            failureTotal += failureProbes[i];
            failureCount++;
        }

        cout << "查找成功的平均查找长度: " 
             << (successCount > 0 ? successTotal / successCount : 0) << endl;
        cout << "查找失败的平均查找长度: " 
             << (failureCount > 0 ? failureTotal / failureCount : 0) << endl;

        delete[] successProbes;
        delete[] failureProbes;
    }
};

// 链表Hash
class HashTableChaining {
private: 
    struct Node {
        int key;
        Node* next;
        Node(int k): key(k), next(nullptr) {}
    };

    Node** table;
    int size;
    int p;

public: 
    HashTableChaining(int TableSize, int mod): size(TableSize), p(mod) {
        table = new Node*[size];
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    void insert(int key) {
        int index = hashfunction(key, p);
        Node* newNode = new Node(key);
        newNode->next = table[index];
        table[index] = newNode;

    }

    int search(int key, bool& success) {
        int index = hashfunction(key, p);
        Node* current = table[index];
        int probes = 0;
        while (probes < size) {
            probes++;
            if (!current) {
                success = false;
                return probes;
            }
            if (current->key == key) {
                success = true;
                return probes;
            }
            current = current->next;
        
        }
        success = false;
        return probes;
    }
    void printTable() {
        cout << "哈希表的地址:\t";
        cout << "表中的关键字:\t";
        cout << endl;
        for (int i = 0; i < size; i++) {
            cout << i << "\t";
            Node* current = table[i];
            if (current) {
                while (current) {
                    cout << current->key << " ";
                    current = current->next;
                }
                cout << "\t";
            } else {
                cout << "-\t";
            }
            cout << endl;
        }
        cout << endl;


        // 初始化探测次数数组
        int* successProbes = new int[size];
        int* failureProbes = new int[size];
        for (int i = 0; i < size; i++) {
            successProbes[i] = 0; // 初始化为 0
            failureProbes[i] = 0; // 初始化为 0
        }

        // 计算每个槽位的成功和失败查找次数
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            int chainLength = 0; // 当前槽位链表长度
            while (current) {
                chainLength++;
                cout << current->key << "对应成功查找次数:\t" << chainLength << endl;
                successProbes[i] += chainLength; // 累加成功查找的探测次数

                current = current->next;
        }
            failureProbes[i] = chainLength + 1; // 失败查找的探测次数 = 链表长度 + 1
        }
        cout << endl;
        cout << "失败查找次数:\t";
        for (int i = 0; i < size; i++) {
        cout << failureProbes[i] << "\t";
    }
        cout << endl;

        // 计算成功和失败的平均查找长度
        double successTotal = 0, failureTotal = 0;
        int successCount = 0, failureCount = size;

        for (int i = 0; i < size; i++) {
        if (table[i]) {
            successTotal += successProbes[i];
            Node* current = table[i];
            while (current) {
                successCount++; // 统计总的成功查找数
                current = current->next;
            }
        }
        failureTotal += failureProbes[i];
    }

        cout << "查找成功的平均查找长度: " << (successCount > 0 ? successTotal / successCount : 0) << endl;
        cout << "查找失败的平均查找长度: " << (failureCount > 0 ? failureTotal / failureCount : 0) << endl;

        delete[] successProbes;
        delete[] failureProbes;
}
    /*
    void printTable() {
        cout << "哈希表的地址:\t";
        for (int i = 0; i < size; i++) {
            cout << i << "\t";
        }
        cout << endl;

        cout << "表中的关键字:\t";
        for (int i = 0; i < size; i++) {
            Node* current = table[i];
            if (current) {
                while (current) {
                    cout << current->key << " ";
                    current = current->next;
                }
                cout << "\t";
            } else {
                cout << "-\t";
            }
        }
        cout << endl;


        int* successProbes = new int[size]; // 动态分配内存
        int* failureProbes = new int[size]; // 动态分配内存
        for (int i = 0; i < size; i++) {
            successProbes[i] = 0;
            failureProbes[i] = 0;
        }


        // 计算每个槽位的成功和失败查找次数
        for (int i = 0; i < size; i++) {
            if (table[i]) {
                // 查找成功的探测长度
                Node* current = table[i];
                while (current) {
                    bool success;
                    successProbes[i] += search(current->key, success);
                    current = current->next;
                }
            }
            // 查找失败的探测长度（假设查找一个不存在的关键字）
            int dummyKey = -1;
            bool success;
            failureProbes[i] = search(dummyKey, success);
        }

        cout << "成功查找次数:\t";
        for (int i = 0; i < size; i++) {
            cout << successProbes[i] << "\t";
        }
        cout << endl;

        cout << "失败查找次数:\t";
        for (int i = 0; i < size; i++) {
            cout << failureProbes[i] << "\t";
        }
        cout << endl;

        // 计算成功和失败的平均查找长度
        double successTotal = 0, failureTotal = 0;
        int successCount = 0, failureCount = size;

        for (int i = 0; i < size; i++) {
            if (table[i]) {
                successTotal += successProbes[i];
                successCount++;
            }
            failureTotal += failureProbes[i];
        }

        cout << "查找成功的平均查找长度: " << (successCount > 0 ? successTotal / successCount : 0) << endl;
        cout << "查找失败的平均查找长度: " << (failureCount > 0 ? failureTotal / failureCount : 0) << endl;

        delete[] successProbes;
        delete[] failureProbes;
    
    }
    */
};


// 主函数
int main() {
    int tableSize, mod, numKeys;
    cout << "请输入哈希表大小: ";
    cin >> tableSize;
    cout << "请输入模数: ";
    cin >> mod;
    cout << "请输入关键字数量: ";
    cin >> numKeys;

    HashTableLinear hashTable1(tableSize, mod);
    HashTableChaining hashTable2(tableSize, mod);

    cout << "请输入关键字: ";
    for (int i = 0; i < numKeys; i++) {
        int key;
        cin >> key;
        hashTable1.insert(key);
        hashTable2.insert(key);
    }

    hashTable1.printTable();
    hashTable2.printTable();
    return 0;
}

