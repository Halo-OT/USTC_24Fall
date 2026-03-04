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
        // 遍历找到 合适的插入位置， 插入
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
    // 构建 freq 升序链表
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            HuffmanNode* node = new HuffmanNode((char)i, freq[i]);
            insert(head, node);
        }
    }

    // 合并 freq 链表, 构建 huffman tree
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
string decodeText(HuffmanNode* root, const string &encodedString) {
    string decodedString;
    HuffmanNode* current = root;
    for (char bit : encodedString) {
        current = (bit == '0') ? current->left : current->right;

        if (!current->left && !current->right) {
            decodedString += current->ch;
            current = root;
        }
    }
    return decodedString;
}

int main() {
    string text;
    string filename;
    cout << "请输入文件名: " ;
    cin >> filename;
    ifstream inputFile(filename);
    ofstream outputFile("output.txt");

    // 从文件读取电文字符串
    if (inputFile) {
        string line;
        while (getline(inputFile, line)) {
            text += line + "\n";  // 将每行追加到 text 中，保留换行符
        }
    } else {
        cout << "无法打开输入文件!" << endl;
        return 1;
    }

    // 统计字符频率
    int freq[256];
    calculateFrequency(freq, text);

    // 构建哈夫曼树
    HuffmanNode* root = Build_Huffman_Tree(freq);

    // 生成编码
    string huffmanCode[256];
    encodeHuffmanTree(root, "", huffmanCode);

    // 写入字符的哈夫曼编码到文件
    outputFile << "字符的哈夫曼编码: \n";
    for (int i = 0; i < 256; i++) {
        if (!huffmanCode[i].empty()) {
            outputFile << (char)i << ": " << huffmanCode[i] << "\n";
        }
    }

    // 编码文本
    string encodedString = encodeText(text, huffmanCode);
    outputFile << "编码后的文本: " << encodedString << "\n";

    // 解码文本
    string decodedString = decodeText(root, encodedString);
    outputFile << "解码后的文本: " << decodedString << "\n";

    outputFile.close();

    cout << "编码和解码结果已写入output.txt文件中。" << endl;
    return 0;
}