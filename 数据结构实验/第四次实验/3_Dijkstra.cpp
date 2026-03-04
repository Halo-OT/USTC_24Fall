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

