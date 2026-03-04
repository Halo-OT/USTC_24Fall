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
