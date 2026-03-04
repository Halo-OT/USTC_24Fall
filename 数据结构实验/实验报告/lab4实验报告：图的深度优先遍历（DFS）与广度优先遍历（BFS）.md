## 实验报告：图的深度优先遍历（DFS）与广度优先遍历（BFS）

### 1. 问题的描述

**设计目标**：设计一个图的遍历程序，能够对给定的图进行深度优先遍历（DFS）和广度优先遍历（BFS）。图采用邻接矩阵表示，程序要求从指定的起始节点出发，分别输出深度优先和广度优先的遍历顺序。

**输入数据**：

- 图的节点数`V`，边数`E`。
- 接下来输入`E`条边，表示图中节点间的连接关系。
- 一个起始节点`start`，表示遍历从此节点开始。

**输出结果**：

- 程序分别输出从`start`节点出发的深度优先遍历（DFS）和广度优先遍历（BFS）顺序。

------

### 2. 算法的描述

#### (1) 数据结构的描述

- **逻辑结构**：采用图的邻接矩阵来存储图的结构，矩阵`graph[i][j]`表示节点`i`与节点`j`是否相邻（若相邻则为`1`，否则为`0`）。

- 存储结构

  ：

  - `graph[MAXV + 1][MAXV + 1]`：邻接矩阵，用来存储图中节点之间的连接信息，大小为`(MAXV + 1) * (MAXV + 1)`，最大支持`MAXV = 30`个节点。
  - `visited[MAXV + 1]`：访问数组，用于记录每个节点是否已经被访问，大小为`MAXV + 1`，避免节点重复访问。

**主要变量说明**：

- `graph[i][j]`：表示节点`i`与节点`j`之间是否有边，若有边则为`1`，否则为`0`。
- `visited[i]`：若节点`i`已被访问，则`visited[i]`为`1`，否则为`0`。
- `dfsresult[MAXV]`、`bfsresult[MAXV]`：用于存储深度优先遍历和广度优先遍历的结果。
- `dfsindex`、`bfsindex`：分别记录DFS和BFS结果数组的当前下标，用于依次填充遍历顺序。

#### (2) 程序结构的描述

**函数原型**：

- `void initializeArray(int arr[], int size, int value)`：初始化数组`arr`，将其所有元素设置为`value`。
- `void initializeMatrix(int Mat[][MAXV + 1], int size, int value)`：初始化邻接矩阵`Mat`，将其所有元素设置为`value`。
- `void dfs(int node, int n, int result[], int& index)`：深度优先遍历函数，从`node`开始，遍历图中的所有节点，结果存储在`result`数组中。
- `void bfs(int start, int n, int result[], int& index)`：广度优先遍历函数，从`start`开始，遍历图中的所有节点，结果存储在`result`数组中。
- `int main()`：主函数，负责输入图的数据，调用DFS和BFS函数，并输出遍历结果。

**功能描述**：

- **DFS函数**：
  - 功能：从指定的节点`node`开始，递归地进行深度优先遍历。
  - 输入：当前节点`node`，节点总数`n`，结果数组`result`，当前遍历结果的索引`index`。
  - 输出：将深度优先遍历的节点按顺序存储到`result`数组中。
- **BFS函数**：
  - 功能：从指定的起始节点`start`开始，按照广度优先的顺序遍历图。
  - 输入：起始节点`start`，节点总数`n`，结果数组`result`，当前遍历结果的索引`index`。
  - 输出：将广度优先遍历的节点按顺序存储到`result`数组中。

#### （3）全部代码

```cpp
#include<iostream>
using namespace std;

const int MAXV = 30;
const int MAXE = 300;

// 邻接矩阵
int graph[MAXV + 1][MAXV + 1];
// 辅助访问数组
int visited[MAXV + 1];

// 初始化数组
void initializeArray(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

void initializeMatrix(int Mat[][MAXV + 1], int size, int value) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Mat[i][j] = value;
        }
    }
}

void dfs(int node, int n, int result[], int& index) {
    visited[node] = 1;  // 本节点已访问
    result[index++] = node;

    // 依次递归dfs node 的所有邻接节点
    for (int i = 1; i <= n; i++) {
        if (graph[node][i] && !visited[i]) {
            dfs(i, n, result, index);
        }
    }
}

void bfs(int start, int n, int result[], int& index) {
    int quene[MAXV + 1]; // 模拟队列
    int front = 0, rear = 0;

    quene[rear++] = start; // 访问入队
    visited[start] = 1;

    while (rear > front) {
        int node = quene[front++];  // 取出队首元素
        result[index++] = node;     // 记录访问序列

        // 依次遍历邻接节点
        for (int i = 1; i <= n; i++) {
            if (graph[node][i] && !visited[i]) {
                
                quene[rear++] = i;  // 入队
                visited[i] = 1;     // 标记为已经访问
            }
        }
    }
}

int main() {
    int V, E;
    cin >> V >> E;

    initializeArray(visited, MAXV + 1, 0);
    initializeMatrix(graph,MAXV + 1, 0);

    // 输入，构建矩阵图
    for (int i = 0; i < E; i++) {
        int a, b;
        cin >> a >> b;
        graph[a][b] = graph[b][a] = 1;
    }

    int start;
    cin >> start;

    // dfs遍历
    int dfsresult[MAXV], dfsindex = 0;
    dfs(start, V, dfsresult, dfsindex);
    
    // 输出
    for (int i = 0; i < dfsindex; i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << dfsresult[i];
    }
    cout << endl;

    // 再次初始化
    initializeArray(visited, MAXV + 1, 0);

    // bfs
    int bfsresult[MAXV], bfsindex = 0;
    bfs(start, V, bfsresult, bfsindex);
    // 输出
    for (int i = 0; i < bfsindex; i++) {
        if(i > 0) {
            cout << " ";
        }
        cout << bfsresult[i];
    }
    cout << endl;

    return 0;

}
```



------

### 3. 调试分析

**测试数据的选择**：

为了全面测试程序的正确性，我们选择了不同结构的图进行测试：

- **测试数据1**：一个简单的无向图，包含5个节点和6条边。用于验证基本功能。
- **测试数据2**：一个包含孤立节点的图，验证程序是否能够正确处理未连接的节点。

**程序调试中遇到的问题及解决方法**：

1. **问题1：邻接矩阵初始化错误**
   - **描述**：程序在初始化邻接矩阵时没有正确设置矩阵的大小，导致部分边未能正确录入。
   - **解决方法**：通过调用`initializeMatrix`函数，确保邻接矩阵被初始化为零，避免错误的边信息影响后续计算。
2. **问题2：访问数组未初始化**
   - **描述**：在DFS和BFS中，访问数组未正确初始化，导致部分节点被重复访问。
   - **解决方法**：在每次遍历之前，都调用`initializeArray`函数初始化访问数组，确保每次遍历从头开始。
3. **问题3：输出格式问题**
   - **描述**：输出结果时，节点之间缺少空格，导致输出格式不符合要求。
   - **解决方法**：修改输出部分代码，确保每个节点之间有空格，且每个遍历的结果在行尾添加换行符。

------

### 4. 算法的时空分析

#### 时间复杂度：

- **DFS**：对于每个节点，我们最多访问一次所有邻接的节点。时间复杂度为`O(V + E)`，其中`V`为节点数，`E`为边数。
- **BFS**：每个节点被访问一次，每条边也被访问一次。时间复杂度同样为`O(V + E)`。

#### 空间复杂度：

- **DFS**：DFS使用递归栈，最大递归深度为`V`，因此空间复杂度为`O(V)`。
- **BFS**：BFS使用队列存储待访问的节点，最大队列大小为`V`，因此空间复杂度为`O(V)`。

------

### 5. 测试结果及分析

**测试数据1**：

```
输入：
5 6 
1 5
1 4
3 1
2 1
2 5
5 3
1

输出：
DFS遍历：1 2 5 3 4 
BFS遍历：1 2 3 4 5 
```

**测试数据2**：

```
输入：
4 2
1 2
2 3
1

输出：
DFS遍历：1 2 3
BFS遍历：1 2 3
```

**分析**：

- 测试数据1验证了程序在一般情况下的功能，DFS和BFS遍历顺序正确。
- 测试数据2验证了程序在处理部分孤立节点时的正确性，程序能够正确处理并输出正确的遍历顺序。

------

### 6. 实验体会和收获

通过本次实验，我深入理解了图的深度优先遍历和广度优先遍历的原理及实现方法。通过邻接矩阵表示图的结构，掌握了如何在程序中使用递归和队列来实现这两种遍历算法。同时，我也学会了如何进行图的调试和优化，确保程序在不同测试数据下都能正常工作。

实验过程中，我还遇到了数组初始化和输出格式的问题，通过解决这些问题，增强了我对程序细节的关注。总体来说，本次实验帮助我加深了对图论基础算法的理解，并提高了编程和调试的能力。