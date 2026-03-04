# 实验报告：Kruskal与Prim算法实现最小生成树

## 1. 问题的描述

### 1.1 设计目标

本次实验的目标是实现图的两种经典最小生成树（MST）算法：Kruskal算法和Prim算法。通过给定一个图的边及其权值，求出该图的最小生成树。最小生成树是一个包含图中所有节点的连通子图，且边的总权重最小。我们将使用不同的算法来实现该问题，并对比其优缺点。

### 1.2 输入数据

- `n`：图中节点的数量。
- `m`：图中边的数量。
- 接下来输入`m`条边，每条边包含三个整数`u`, `v`, `weight`，表示从节点`u`到节点`v`的边，权重为`weight`。

### 1.3 输出结果

- 输出图的最小生成树的总权重。

------

## 2. 算法的描述

### 2.1 数据结构的描述

#### 逻辑结构

- **图的存储**：图采用邻接矩阵的形式存储，其中`graph[i][j]`表示节点`i`与节点`j`之间的边的权重。
- **父节点数组**（仅用于Kruskal算法）：记录每个节点的父节点，用于并查集操作，确保不会形成环。
- **秩数组**（仅用于Kruskal算法）：用于优化并查集合并操作，按秩合并使树保持较低的高度。

#### 存储结构

- **邻接矩阵**：用于存储图的边，大小为`(n+1) x (n+1)`，其中`graph[i][j]`存储节点`i`到节点`j`的边的权重。
- **访问数组**：`visited[]`用来标记节点是否已经访问。
- **最小边数组**：`minEdge[]`用来记录从树中节点到未加入树的其他节点的最小边的权值。

### 2.2 程序结构的描述

#### 函数原型

- **Kruskal算法**：
  - `int kruskal(int n, int m)`：Kruskal算法的核心，求图的最小生成树的权重。
- **Prim算法**：
  - `int prim(int n)`：Prim算法的核心，求图的最小生成树的权重。

#### 功能描述

- **Kruskal算法**：
  - 功能：利用并查集实现Kruskal算法，按权值升序选择边，构建最小生成树。
  - 输入：节点数`n`，边数`m`。
  - 输出：最小生成树的总权重。
- **Prim算法**：
  - 功能：从某个节点开始，逐步选取最小权重的边加入最小生成树。
  - 输入：节点数`n`。
  - 输出：最小生成树的总权重。

------

### 2.3 全部代码

####  2.3.1 Kruskal算法：

```cpp
#include <iostream>
using namespace std;

const int MAXE = 100000;
const int MAXV = 10000;

struct Edge {
    int u, v, weight;
};

Edge edges[MAXE];
int parent[MAXV + 1];
int r[MAXV + 1];

void initializeUnionFind(int n) {
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        r[i] = 0;
    }
}

int find(int x) {
    if (x != parent[x]) {
        parent[x] = find(parent[x]); // 路径压缩
    }
    return parent[x];
}

void unionSets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
        if (r[rootX] > r[rootY]) {
            parent[rootY] = rootX;
        } else if (r[rootX] < r[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            r[rootX]++;
        }
    }
}

int kruskal(int n, int m) {
    // 边按权值排序（简单冒泡排序）
    for (int i = 0; i < m - 1; i++) {
        for (int j = 0; j < m - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    initializeUnionFind(n);
    int totalWeight = 0, edgesUsed = 0;

    for (int i = 0; i < m && edgesUsed < n - 1; i++) {
        int u = edges[i].u, v = edges[i].v, weight = edges[i].weight;
        if (find(u) != find(v)) {
            unionSets(u, v);
            totalWeight += weight;
            edgesUsed++;
        }
    }
    return totalWeight;
}

int main() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    cout << kruskal(n, m) << endl;

    return 0;
}

```

#### 2.3.2 Prim算法：

```cpp
#include <iostream>
using namespace std;

const int MAXV = 10000;
const int INF = 1e9;

int graph[MAXV + 1][MAXV + 1]; // 邻接矩阵
bool visited[MAXV + 1];        // 标记是否访问
int minEdge[MAXV + 1];         // 记录当前最小权值

int prim(int n) {
    // 初始化
    for (int i = 1; i <= n; i++) {
        minEdge[i] = INF;
        visited[i] = false;
    }

    minEdge[1] = 0; // 从节点 1 开始
    int totalWeight = 0;

    for (int i = 1; i <= n; i++) {
        int u = -1;

        // 找到未访问的最小权值顶点
        for (int j = 1; j <= n; j++) {
            if (!visited[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                u = j;
            }
        }

        visited[u] = true;
        totalWeight += minEdge[u];

        // 更新邻接顶点的最小边权值
        for (int v = 1; v <= n; v++) {
            if (!visited[v] && graph[u][v] < minEdge[v]) {
                minEdge[v] = graph[u][v];
            }
        }
    }
    return totalWeight;
}

int main() {
    int n, m;
    cin >> n >> m;

    // 初始化邻接矩阵
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            graph[i][j] = INF; // 初始化为无穷大
        }
    }

    // 输入边
    for (int i = 0; i < m; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        graph[a][b] = w;
        graph[b][a] = w; // 无向图
    }

    cout << prim(n) << endl;

    return 0;
}

```

------



## 3. 调试分析

### 3.1 程序调试中遇到的问题及解决方法

#### 问题1：Kruskal算法的并查集初始化问题

- **描述**：在Kruskal算法中，程序初始化并查集时没有正确设置每个节点的父节点，导致后续合并操作出错。
- **解决方法**：通过`initializeUnionFind`函数确保每个节点的父节点初始化为节点自身，并初始化为0。

#### 问题2：Prim算法的最小权值更新问题

- **描述**：在Prim算法中，初始化时没有正确更新最小权值的数组`minEdge[]`，导致错误的边被选入最小生成树。
- **解决方法**：每次选择未访问节点中最小的权值边，并确保每个节点的最小权值被正确更新。

#### 问题3：图的边权排序问题

- **描述**：Kruskal算法中，使用了冒泡排序，时间复杂度较高，影响性能。
- **解决方法**：虽然使用冒泡排序在小规模数据下是有效的，但可以使用更高效的排序算法（如快速排序）进行优化。



### 3.2 **测试样例**

**输入**：

```
4 5
1 2 2
1 3 2
1 4 3
2 3 4
3 4 3
```

- 图的节点数：4
- 图的边数：5
- 边的信息：
  - 边1：节点1与节点2，权重为2
  - 边2：节点1与节点3，权重为2
  - 边3：节点1与节点4，权重为3
  - 边4：节点2与节点3，权重为4
  - 边5：节点3与节点4，权重为3

------

### **Kruskal算法的执行过程**

1. **边排序**：
   - 按照边的权重进行升序排序：`(1,2,2)`, `(1,3,2)`, `(1,4,3)`, `(3,4,3)`, `(2,3,4)`。
2. **构建最小生成树**：
   - 初始时，每个节点是独立的。
   - 选择最小的边`(1,2,2)`，合并节点1和节点2。
   - 选择下一条边`(1,3,2)`，合并节点1和节点3。
   - 选择边`(1,4,3)`，合并节点1和节点4。
   - 现在已经遍历了所有节点，形成的最小生成树包含的边为`(1,2)`, `(1,3)`, `(1,4)`，总权重为`2 + 2 + 3 = 7`。

**Kruskal算法输出**：

```
7
```

------

### **Prim算法的执行过程**

1. **初始化**：
   - 从节点1开始，初始化`minEdge[1] = 0`，其他节点的最小权重初始化为`INF`。
   - 初始化`visited[]`数组来记录节点是否已访问。
2. **步骤**：
   - 初始时，选择节点1，遍历与节点1相连的所有边，更新`minEdge[]`：`minEdge[2] = 2`, `minEdge[3] = 2`, `minEdge[4] = 3`。
   - 选择最小权值边，节点2或节点3都可以作为下一个节点，选择节点2（按顺序选择最小节点）。
   - 更新与节点2相连的边，`minEdge[3] = 4`（不更新，因为已经有更小的边`minEdge[3] = 2`），`minEdge[4] = 3`。
   - 选择节点3，更新与节点3相连的边，`minEdge[4] = 3`，无变化。
   - 选择节点4，所有节点已访问，完成最小生成树的构建。

**Prim算法输出**：

```
7
```

------

### **总结与分析**

- **Kruskal算法**：
  - Kruskal算法首先对所有的边按权值排序，然后逐步选择最小的边，加入最小生成树，同时使用并查集来确保没有形成环。
  - 在此测试样例中，Kruskal算法得出的最小生成树的总权重为`7`。
- **Prim算法**：
  - Prim算法通过从一个节点开始，逐步选择与树中节点相连的最小边，扩展最小生成树，直到所有节点都被加入树中。
  - 在此测试样例中，Prim算法得出的最小生成树的总权重也为`7`。

### **两者的比较**：

- **算法的实现方式不同**：
  - **Kruskal算法**：是基于边的排序和并查集操作来构建最小生成树，适合处理边数较多的稀疏图。
  - **Prim算法**：是基于节点的扩展，通过选择与当前最小生成树相连的最小边来扩展树，适合处理节点数较多的图。
- **结果一致**：
  - 在这个测试样例中，Kruskal和Prim两种算法都正确地构建了最小生成树，结果一致。

------



## 4. 算法的时空分析

### 4.1 时间复杂度

- **Kruskal算法**：
  - **排序**：对边进行排序的时间复杂度为`O(m log m)`，其中`m`为边的数量。
  - **并查集操作**：每次`find`和`union`操作的时间复杂度为`O(α(n))`，其中`α(n)`为反阿克曼函数，几乎为常数。
  - **总时间复杂度**：排序主导了时间复杂度，因此总体为`O(m log m)`。
- **Prim算法**：
  - **选择最小边**：选择最小边的操作需要遍历所有未访问的节点，因此时间复杂度为`O(n)`。
  - **更新权值**：每次更新最小边时，需要遍历所有节点，因此时间复杂度为`O(n)`。
  - **总时间复杂度**：主导因素是遍历和更新最小边，因此时间复杂度为`O(n^2)`。

### 4.2 空间复杂度

- **Kruskal算法**：
  - **边数组**：存储`m`条边，空间复杂度为`O(m)`。
  - **并查集数组**：`parent[]`和`r[]`数组的空间复杂度为`O(n)`。
  - **总空间复杂度**：`O(n + m)`。
- **Prim算法**：
  - **邻接矩阵**：存储图的边信息，空间复杂度为`O(n^2)`。
  - **访问数组**：`visited[]`和`minEdge[]`数组的空间复杂度为`O(n)`。
  - **总空间复杂度**：`O(n^2)`。

------



## 5. 实验体会和收获

通过本次实验，我深入理解了Kruskal和Prim算法的原理和实现方法，并学习了如何应用并查集和贪心策略解决最小生成树问题。通过比较两种算法的实现，我体会到在不同场景下选择合适的算法会显著影响程序的性能和效率。在调试过程中，我学会了如何有效处理图的输入输出、边权排序等问题，并加强了我对图算法和数据结构的掌握。

通过优化Kruskal算法中的排序方式，我认识到排序算法的时间复杂度对整个程序的效率有着重要影响。总体来说，本次实验帮助我加深了对图算法的理解，并提升了我的编程能力和算法优化能力。