# 实验报告：Huffman压缩与解压缩程序设计与实现

## 1. 实验目的

本实验旨在实现一个基于Huffman编码的文件压缩和解压缩程序。Huffman编码是一种常用的无损数据压缩算法，利用字符的频率信息对文本进行编码，从而减少数据所占的存储空间。本实验通过实现Huffman树的构建、编码与解码过程，模拟文件的压缩与解压操作。

## 2. 实验内容

### 2.1 任务描述

本程序的主要功能包括：

1. **压缩功能**：读取文本文件，统计字符频率，构建Huffman树，生成Huffman编码表，并根据编码表对文件内容进行压缩。
2. **解压功能**：读取压缩文件，恢复Huffman树，解码文件中的压缩数据，恢复原始文本内容。
3. **用户交互**：程序提供命令行界面，允许用户选择进行文件压缩或解压操作。

### 2.2 输入输出

#### 输入：

- 压缩操作：输入文件名，程序会将该文件压缩为`.huff`格式的压缩文件。
- 解压操作：输入压缩文件名（`.huff`格式），程序会解压该文件并恢复原始文件内容。

#### 输出：

- 压缩后的文件：以`.huff`为扩展名保存压缩文件。
- 解压后的文件：恢复为原始文件格式。

### 2.3 功能实现

#### 2.3.1 Huffman树的构建

首先，程序会读取文件中的字符，统计每个字符出现的频率。然后，根据字符频率构建一棵Huffman树。构建过程通过一个最小优先队列（链表实现），每次从队列中取出频率最小的两个节点，将其合并为一个新的节点，并将新的节点插回队列。这个过程直到队列中只剩下一个节点时结束。

```cpp
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
```

#### 2.3.2 Huffman编码

通过对Huffman树的深度优先遍历，生成每个字符的Huffman编码。左子树对应`0`，右子树对应`1`，直到叶子节点。每个字符的编码存储在一个数组中。

```cpp
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
```

#### 2.3.3 压缩与解压缩

- **压缩**：程序会将文本中的每个字符根据其对应的Huffman编码进行转换，形成一个长的比特串。并将字符的频率信息、比特串的长度以及比特串本身存储到输出文件中。
- **解压缩**：程序会读取压缩文件中的频率表，重建Huffman树，然后根据Huffman树对比特串进行解码，恢复原始文本。

```cpp
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

    // 存储频率表和比特串
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
```

------

### 2.4 全部代码

```cpp
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

```

------



## 3. 调试分析

在实验过程中，程序的实现需要考虑多个因素，如Huffman树的构建、编码与解码过程的准确性、文件操作的正确性等。以下是测试数据的选择、调试中遇到的问题以及解决方法的详细分析。

### 3.1 测试用例

#### 测试：文本压缩与解压

**输入文件**：`test.???`

**压缩**：

1. 输入文件`test.???`进行压缩，得到`test.txt.huff`。

**解压**：

1. 使用`test.txt.huff`进行解压，恢复为`test.???`。

**验证**：

- 解压后的文件内容应与原始文件一致。

### 3.2 程序调试中遇到的问题及解决方法

#### 问题1：无法正确读取和写入二进制文件

- **问题描述**：程序在读取和写入二进制文件时，部分数据丢失或者文件损坏。

- **原因分析**：C++中，默认的文件流（`ifstream`和`ofstream`）是以文本模式打开文件的，因此可能会导致某些字符（尤其是控制字符）被错误处理，导致文件读取或写入时出现问题。

- 解决方法

  ：在读取和写入二进制文件时，必须显式地指定二进制模式。使用

  ```
  ios::binary
  ```

  标志打开文件，确保数据能够按原样读写。

  ```cpp
  ifstream in(inputFile, ios::binary);
  ofstream out(outputFile, ios::binary);
  ```

#### 问题2：字符频率统计不准确

- **问题描述**：程序在统计字符频率时，某些字符频率未能正确统计。

- **原因分析**：程序使用`freq[(unsigned char) ch]`来统计字符频率，如果字符是高位字符（大于127的字符），可能会导致数组越界，或者由于ASCII码的转换错误，导致某些字符没有被统计。

- 解决方法

  ：确保

  ```
  freq
  ```

  数组的大小足够容纳所有可能的字符（0-255），并且正确地处理每个字符的频率。

  ```cpp
  int freq[256] = {0};  // 保证频率数组有足够空间
  ```

#### 问题3：Huffman编码表没有正确生成

- **问题描述**：Huffman编码生成过程中，某些字符的编码为空，或者解压后的文件与原文件不一致。

- **原因分析**：Huffman树构建过程中，可能存在某些节点没有正确连接，或者在递归编码时，某些字符没有分配到对应的编码。

- **解决方法**：

  1. 确保树的构建过程中每个节点的`left`和`right`指针正确指向左右子节点。
  2. 编码时确保遍历每个叶子节点，并将编码存储在正确的位置。

  ```cpp
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
  ```

#### 问题4：压缩效率低，文件过大

- **问题描述**：对于某些输入文件，压缩后的文件大小基本不变，甚至反而比原文件大。

- **原因分析**：Huffman编码并不适用于所有类型的文本，尤其是当字符频率分布较为均匀时，Huffman编码可能会导致文件体积膨胀。另外，压缩产生的文件要存储Huffman树的信息，导致文件内容更大。

- **解决方法**：

  ​	无，这是Huffman编码本身的局限性。

### 3.3 调试过程中总结

- **错误排查**：通过逐步调试和打印输出，分析每一部分的执行过程，发现并修正了文件读取、频率统计、树的构建等模块中的问题。
- **代码重构**：在过程中，发现了多个潜在的代码优化点，例如改进频率统计、优化节点插入和链表操作等，使程序更高效。
- **单元测试**：通过不同的数据集进行多轮测试，确保程序能够在各种情况下正常工作。



## 4. 复杂度分析

### 4.1 时间复杂度

1. **构建Huffman树**：构建过程涉及到插入和弹出操作，每个操作的时间复杂度为O(log N)，其中N是树中的节点数量。总的时间复杂度为O(N log N)。
2. **编码与解码**：编码和解码的时间复杂度为O(M)，其中M是文件的总字符数。

### 4.2 空间复杂度

1. **Huffman树**：树结构的空间复杂度为O(N)，其中N是树的节点数。
2. **频率数组和编码表**：频率数组的空间复杂度为O(256)，编码表的空间复杂度为O(256)。

因此，总体空间复杂度为O(N + M)，N为树的节点数，M为文件大小。



## 5. 总结与体会

本实验通过实现Huffman编码的压缩与解压缩过程，深入理解了Huffman算法的基本原理及其在数据压缩中的应用。实验中遇到的一些问题，如编码表的构建、比特流的处理等，帮助我加深了对数据压缩算法的理解。此外，通过实现文件操作的相关功能，我提升了自己在文件处理和算法设计方面的能力。