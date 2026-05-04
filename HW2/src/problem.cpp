
#include <iostream>
using namespace std;

const int MAX = 100;

int graph[MAX][MAX];
bool visited[MAX];
int n, e;

void DFS(int v) {
    visited[v] = true;
    cout << v << " ";

    for (int i = 0; i < n; i++) {
        if (graph[v][i] == 1 && !visited[i]) {
            DFS(i);
        }
    }
}

void BFS(int start) {
    int queue[MAX];
    int front = 0, rear = 0;

    visited[start] = true;
    queue[rear++] = start;

    while (front < rear) {
        int v = queue[front++];
        cout << v << " ";

        for (int i = 0; i < n; i++) {
            if (graph[v][i] == 1 && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;
            }
        }
    }
}

void resetVisited() {
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }
}

void connectedComponents() {
    resetVisited();
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            count++;
            cout << "Component " << count << ": ";
            DFS(i);
            cout << endl;
        }
    }

    cout << "Total: " << count << endl;
}

int main() {
    cin >> n >> e;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = 0;

    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        graph[a][b] = 1;
        graph[b][a] = 1;
    }

    resetVisited();
    cout << "DFS: ";
    DFS(0);
    cout << endl;

    resetVisited();
    cout << "BFS: ";
    BFS(0);
    cout << endl;

    connectedComponents();

    return 0;
}
