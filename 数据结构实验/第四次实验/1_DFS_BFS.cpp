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