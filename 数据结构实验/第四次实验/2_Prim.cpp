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
