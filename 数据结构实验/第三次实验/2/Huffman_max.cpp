#include<iostream>
#include<string>
#include<fstream>

using namespace std;

// 哈夫曼树 节点
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;
    HuffmanNode(char c, int f): ch(c), freq(f), left(nullptr), right(nullptr) {} 
};

// 哈夫曼树 存放节点（比较的链表）
struct ListNode {
    HuffmanNode* huffnode;
    ListNode* next;
    ListNode(HuffmanNode* node): huffnode(node), next(nullptr) {}
};

// 插入到队列中（链表排序，按照 freq 的优先顺序： 升序）
void insert(ListNode*& head, HuffmanNode* node) {
    ListNode* newnode = new ListNode(node);
    if (!head || head->huffnode->freq > node->freq) {
        newnode->next = head;
        head = newnode;
    }
    else {
        ListNode* current =  head;
        while (current->next && current->next->huffnode->freq <= node->freq) {
            current = current->next;
        }   
        newnode->next = current->next;
        current->next = newnode;     
    }
}

// 从队列（链表）中取出 freq 最小的， 拿来自底向上构造 Huffman 树
HuffmanNode* PopMin(ListNode*& head) {
    if (!head) return nullptr;
    ListNode* temp = head;
    HuffmanNode* node = head->huffnode;
    head = head->next;
    delete temp;
    return node;
}

// 统计电文中 不同字符 出现的次数 freq
void calculateFrequency(int freq[], const string &text) {
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
    }
    for(char ch : text) {
        freq[(unsigned char) ch]++;
    }
}

// 构建 Huffman 树
HuffmanNode* Build_Huffman_Tree(int freq[]) {
    ListNode* head = nullptr;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            HuffmanNode* node = new HuffmanNode((char)i, freq[i]);
            insert(head, node);
        }
    }

    while(head && head->next) {
        HuffmanNode* left = PopMin(head);
        HuffmanNode* right = PopMin(head);
        
        int sum = left->freq + right->freq;
        HuffmanNode* combine_node = new HuffmanNode('\0', sum);
        combine_node->left = left;
        combine_node->right = right;

        insert(head,combine_node);
    }

    return PopMin(head);
}

// 对 HuffmanTree 进行 编码 left-0， right-1, str 为编码结果
void encodeHuffmanTree(HuffmanNode* root, string str, string HuffmanCode[]) {
    if (!root) return;
    if (!root->left && !root->right) {
        HuffmanCode[(unsigned char) root->ch] = str;
    }
    else {
        encodeHuffmanTree(root->left, str + "0", HuffmanCode);
        encodeHuffmanTree(root->right, str + "1", HuffmanCode);
    }
}

// 编码输入的文本
string encodeText(const string &text, string HuffmanCode[]) {
    string encodedString;
    for (char ch : text) {
        encodedString += HuffmanCode[(unsigned char)ch];
    }
    return encodedString;
}

// 根据 Huffman 树对 比特流进行解码
string decodeText(HuffmanNode* root, const string &encodedString, int bitCount) {
    string decodedString;
    HuffmanNode* current = root;
    int bitsRead = 0;
    for (char bit : encodedString) {
        if (bitsRead >= bitCount) break;
        current = (bit == '0') ? current->left : current->right;
        bitsRead++;

        if (!current->left && !current->right) {
            decodedString += current->ch;
            current = root;
        }
    }
    return decodedString;
}

// 压缩文件
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cout << "无法打开文件: " << inputFile << endl;
        return;
    }
    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    int freq[256] = {0};
    calculateFrequency(freq, text);

    HuffmanNode* root = Build_Huffman_Tree(freq);
    string huffmanCode[256];
    encodeHuffmanTree(root, "", huffmanCode);

    ofstream out(outputFile, ios::binary);
    if (!out) {
        cout << "无法创建文件: " << outputFile << endl;
        return;
    }

    int freqCount = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) freqCount++;
    }
    out.write(reinterpret_cast<const char*>(&freqCount), sizeof(freqCount));

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            out.put(i);
            out.write(reinterpret_cast<const char*>(&freq[i]), sizeof(freq[i]));
        }
    }

    string encodedString = encodeText(text, huffmanCode);
    int bitCount = encodedString.size();
    out.write(reinterpret_cast<const char*>(&bitCount), sizeof(bitCount));

    int bitWritten = 0;
    unsigned char byte = 0;
    for (char bit : encodedString) {
        byte = (byte << 1) | (bit - '0');
        bitWritten++;
        if (bitWritten == 8) {
            out.put(byte);
            bitWritten = 0;
            byte = 0;
        }
    }
    if (bitWritten > 0) {
        byte <<= (8 - bitWritten);
        out.put(byte);
    }

    out.close();
    cout << "文件已成功压缩为: " << outputFile << endl;
}

void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cout << "无法打开文件: " << inputFile << endl;
        return;
    }

    int freqCount;
    in.read(reinterpret_cast<char*>(&freqCount), sizeof(freqCount));

    int freq[256] = {0};
    for (int i = 0; i < freqCount; i++) {
        char ch;
        int frequency;
        in.get(ch);
        in.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        freq[(unsigned char)ch] = frequency;
    }

    HuffmanNode* root = Build_Huffman_Tree(freq);

    int bitCount;
    in.read(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));

    string encodedString;
    unsigned char byte;
    while (in.get((char&)byte)) {
        for (int i = 7; i >= 0; i--) {
            encodedString += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    in.close();

    string decodedString = decodeText(root, encodedString, bitCount);

    ofstream out(outputFile, ios::binary);
    if (!out) {
        cout << "无法创建文件: " << outputFile << endl;
        return;
    }
    out << decodedString;
    out.close();

    cout << "文件已成功解压缩为: " << outputFile << endl;
}

int main() {
    int choice = 1;
    while (choice) {
        cout << "请输入你的选择 1.压缩  2.解压  0.退出: ";
        cin >> choice;
        if (choice == 1) {
            string inputFilename, outputFilename;
            cout << "请输入要压缩的文件名: ";
            cin >> inputFilename;
            outputFilename = inputFilename + ".huff";
            compressFile(inputFilename, outputFilename);
        }
        else if (choice == 2) {
            string inputFilename, outputFilename;
            cout << "请输入要解压的文件名: ";
            cin >> inputFilename;
            if (inputFilename.size() > 5 && inputFilename.substr(inputFilename.size() - 5) == ".huff") {
                outputFilename = inputFilename.substr(0, inputFilename.size() - 5);
                decompressFile(inputFilename, outputFilename);
            } else {
                cout << "文件名错误" << endl;
            }
        }
        else if (choice == 0) {
            break;
        }
        else {
            cout << "选择错误！" << endl;
        }
    }
    return 0;
}
