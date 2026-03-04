# 实验报告：基于邻接表的Dijkstra算法实现最短路径

## 1. 问题的描述

### 1.1 设计目标

本次实验的目标是实现Dijkstra算法，使用邻接表存储图，并手动实现一个优先队列来优化Dijkstra算法的性能。该算法用于解决单源最短路径问题，给定一个有权图和起始节点，计算从起始节点到目标节点的最短路径权重。如果存在路径，则输出最短路径的权重，否则输出没有路径。

### 1.2 输入数据

- 图的节点数`n`，边数`m`。
- `m`条边，每条边包含三个整数`a`, `b`, `w`，表示从节点`a`到节点`b`的边，权重为`w`。
- 起始节点`s`和目标节点`t`。

### 1.3 输出结果

- 输出从起始节点`s`到目标节点`t`的最短路径的总权重。如果节点`t`不可达，则输出`No path found`。

------

## 2. 算法的描述

### 2.1 数据结构的描述

#### 逻辑结构

- **邻接表**：使用邻接表表示图，`graph[u]`包含一个指向节点`u`的所有邻接边的链表，每条边包含终点`v`和权重`w`。
- **优先队列**：使用一个简单的手动实现的优先队列（基于数组），用于存储节点和其当前最短路径权重。每次选择最小路径权重的节点进行松弛操作。

#### 存储结构（直接从作业题中挪用）

- **Graph结构**：包含`numVertex`（节点数）和`adjList`（邻接表数组）。`adjList`的每个元素是一个链表头指针，链表节点`AdjNode`包含`vertex`（目标节点）和`weight`（边的权重）。

  ```cpp
  struct AdjNode {
      int vertex, weight;
      AdjNode* next;
      AdjNode(int v, int w) : vertex(v), weight(w), next(nullptr) {}
  };
  ```

- **PriorityQueue结构**：手动实现的优先队列，包含一个`heap`数组用于存储节点，`distance`数组用于存储每个节点的当前最短路径权重，`size`表示队列的大小。

  ```cpp
  struct PriorityQueue {
      int* heap;
      int* distance;
      int size;
      void push(int vertex, int dist);
      int pop();
      bool empty();
  };
  ```

- **`dist[]`数组**：存储从起始节点到每个节点的最短路径的当前权重。

- **`visited[]`数组**：标记每个节点是否已经被访问过。

### 2.2 程序结构的描述

#### 函数原型

- **图的结构与加边（直接从作业题中挪用）`void Graph::addEdge(int src, int dest, int weight)`**：
  - 功能：向图中添加一条边`src -> dest`，并且因为是无向图，还需添加`dest -> src`的反向边。
  - 输入：边的起点`src`，终点`dest`，权重`weight`。
- **Dijkstra算法`int dijkstra(Graph& graph, int start, int end)`**：
  - 功能：使用Dijkstra算法计算从节点`start`到节点`end`的最短路径。
  - 输入：图`graph`，起始节点`start`，目标节点`end`。
  - 输出：从`start`到`end`的最短路径权重。
- **`PriorityQueue::push(int vertex, int dist)`**：
  - 功能：向优先队列中插入一个节点`vertex`和其对应的最短路径权重`dist`。
  - 输入：节点`vertex`，权重`dist`。
- **`PriorityQueue::pop()`**：
  - 功能：从优先队列中弹出权重最小的节点并返回。
  - 输出：权重最小的节点。
- **`PriorityQueue::empty()`**：
  - 功能：判断优先队列是否为空。
  - 输出：若队列为空，返回`true`，否则返回`false`。

------

### 2.3 全部代码

```cpp
#include<iostream>
#include <climits>
using namespace std;

// 邻接表节点
struct AdjNode {
    int vertex, weight;
    AdjNode* next;
    AdjNode(int v, int w) : vertex(v), weight(w), next(nullptr) {}
};

// 图的结构
struct Graph {
    int numVertex;
    AdjNode** adjList;

    Graph(int vertices) {
        numVertex = vertices;
        adjList = new AdjNode*[vertices + 1]; // 1-based indexing
        for (int i = 0; i <= numVertex; i++) {
            adjList[i] = nullptr;
        }
    }

    // 添加边
    void addEdge(int src, int dest, int weight) {
        // 添加 src -> dest
        AdjNode* newNode = new AdjNode(dest, weight);
        newNode->next = adjList[src];
        adjList[src] = newNode;

        // 添加 dest -> src（无向图）
        newNode = new AdjNode(src, weight);
        newNode->next = adjList[dest];
        adjList[dest] = newNode;
    }

};
// 手动实现 优先队列
struct PriorityQueue {
    int* heap;
    int* distance;
    int size;

    PriorityQueue(int n) {
        heap = new int[n + 1];
        distance = new int[n + 1];
        size = 0;
    }

    void push(int vertex, int dist) {
        heap[++size] = vertex;
        distance[vertex] = dist; 
    }

    int pop() {
        int minVertex = -1, minDist = INT_MAX, minIndex = -1;
        for (int i = 1; i <= size; ++i) {
            if (distance[heap[i]] < minDist) {
                minDist = distance[heap[i]];
                minVertex = heap[i];
                minIndex = i;
            }
        }
        if (minIndex != -1) {
            // 移除堆顶元素
            heap[minIndex] = heap[size--];
        }
        return minVertex;
    }

    bool empty() {
        return size == 0;
    }
};

// Dijkstra 算法
int dijkstra(Graph& graph, int start, int end) {
    int* dist = new int[graph.numVertex + 1];
    bool* visited = new bool[graph.numVertex + 1];

    // 初始化
    for (int i = 1; i <= graph.numVertex; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[start] = 0;

    // 优先队列
    PriorityQueue pq(graph.numVertex);
    pq.push(start, 0);

    while (!pq.empty()) {
        int u = pq.pop();
        if (visited[u]) continue;
        visited[u] = true;

        // 遍历邻接表
        AdjNode* neighbor = graph.adjList[u];
        while (neighbor != nullptr) {
            int v = neighbor->vertex;
            int weight = neighbor->weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push(v, dist[v]);
            }
            neighbor = neighbor->next;
        }
    }

    int result = dist[end];
    delete[] dist;
    delete[] visited;

    return result == INT_MAX ? -1 : result;
}

int main() {
    int n, m;
    cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        graph.addEdge(a, b, w);
    }

    int s, t;
    cin >> s >> t;

    int result = dijkstra(graph, s, t);
    if (result == -1) {
        cout << "No path found" << endl;
    } else {
        cout << result << endl;
    }

    return 0;
}


```

------

## 3. 调试分析

### 3.1 程序调试中遇到的问题及解决方法

#### 问题1：优先队列的实现问题

- **描述**：在手动实现优先队列时，发现优先队列中的节点没有正确地按照最小权值排序，导致Dijkstra算法未能正确地选择最小的节点进行松弛操作。
- **解决方法**：修改了`pop()`函数，通过遍历整个队列找到最小权值节点，并移除它。

#### 问题2：邻接表的构建错误

- **描述**：在添加边时，邻接表的插入顺序不对，导致图的结构错误，影响结果。
- **解决方法**：确保每条边在邻接表中正确地反向插入，保证无向图的对称性。

#### 问题3：Dijkstra的初始化问题

- **描述**：在初始化`dist[]`数组时，未正确处理源节点的最短路径，导致结果错误。
- **解决方法**：确保在初始化时，将起始节点的`dist[start]`设置为0，其他节点设置为`INF`。

------

## 4. 算法的时空分析

### 4.1 时间复杂度

- **Dijkstra算法**：每次从优先队列中弹出一个节点，遍历该节点的邻接节点并更新最短路径。对每个节点，最多需要遍历其所有邻接边。因此，时间复杂度为`O((V + E) log V)`，其中`V`为节点数，`E`为边数。由于优先队列的每次插入和删除操作是`O(log V)`。
- **手动实现的优先队列**：每次从队列中选出最小值的操作是`O(V)`，而`V`是当前节点数，因此优化空间较大。实际应用中可以使用更高效的优先队列（如二叉堆或Fibonacci堆）。

### 4.2 空间复杂度

- **邻接表**：空间复杂度为`O(V + E)`，其中`V`是节点数，`E`是边数。
- **优先队列**：空间复杂度为`O(V)`，用于存储所有节点及其对应的最短路径。

------

## 5. 测试结果及分析

### 5.1 测试数据1

**输入**：

```
4 5
1 2 10
1 3 20
2 3 15
2 4 30
3 4 20
1 4
```

**输出**：

```
40
```

**分析**：

- 从节点1到节点4的最短路径为`1 -> 2 -> 4`或`1 -> 3 -> 4`，总权重为40。程序正确计算了最短路径。

------

## 6. 实验体会和收获

通过本次实验，我深入理解了Dijkstra算法的原理和实现，掌握了如何使用邻接表存储图以及如何手动实现优先队列来优化算法性能。通过解决实际中的调试问题，我加深了对图算法和数据结构的理解，特别是在图的表示和最短路径计算的细节方面。