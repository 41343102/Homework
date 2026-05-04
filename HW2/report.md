# 41343102、41343103

## 解題說明

### 問題描述:
- 實作 Graph 圖形結構
- 用鄰接矩陣 Adjacency Matrix 來表示圖
- 節點之間有連線，就用 1 表示，沒有的話就是 0
- 實作 DFS 深度優先搜尋
- 實作 BFS 廣度優先搜尋
- 還有用 DFS 和 BFS 來找 Connected Components 連通元件

### 解題策略:
- 先建立一個二維陣列來存圖的資料
- 再用 visited 陣列來記錄哪些節點已經走過
- DFS 的部分用遞迴來做，會一直往下一個還沒走過的節點走，走到不能走為止再回來
- BFS 則是用 Queue（佇列），先把相鄰的節點全部加入，再一層一層往外擴展
- Connected Components 的做法是把所有節點都檢查一次，如果遇到還沒被拜訪過的，就從那個節點開始做 DFS，這樣就可以找出一個連通區塊

## 程式實作
```cpp
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
```

## 效能分析

### 時間複雜度:
- DFS：O(n²)
- BFS：O(n²)
- Connected Components：O(n²) 
   
### 空間複雜度:
- 鄰接矩陣：O(n²)
- visited 陣列：O(n)
- queue 陣列：O(n)
- 總空間複雜度：O(n²)
  
## 測試與驗證


### 輸入:
```shell
$ ./graph.exe
6 4
0 1
0 2
3 4
4 5
```
### 輸出:
```shell
DFS: 0 1 2
BFS: 0 1 2
Component 1: 0 1 2
Component 2: 3 4 5
Total: 2

```
## 申論及開發報告
這次的作業中我們主要是實作 Graph，並透過 DFS、BFS 、Connected Components 來了解圖的基本操作與應用。

一開始需要理解的是 Graph 的表示方式。我們這次是使用鄰接矩陣來存資料，雖然寫起來比較直觀，但在節點數變多時，空間使用會比較大，這也是之後可以改進的地方。

DFS 的部分我們是使用遞迴的方式來實作，DFS 的特點是會一路往下走到底，再回來找其他路徑。

BFS 則是使用 Queue 來實作，這部分和 DFS 不同的是，它是一層層往外擴展。在寫的過程中需要注意 queue 的 front 和 rear 控制，否則很容易出現資料錯亂的情況。

Connected Components 的實作，讓我們更理解 DFS 的應用。透過不斷從尚未被拜訪的節點開始做 DFS，可以把整個圖分成不同的區塊。這個概念在像是社群分析或網路分群中其實很常見。

效能方面因為使用的是鄰接矩陣，每次都需要掃描整列資料，所以時間複雜度是 O(n²)。如果今天節點數非常多，這樣的做法就會變得比較沒有效率。因此，如果要優化，可以考慮改用鄰接串列，在邊數較少的情況下會更節省空間，也能提升搜尋效率。

總而言之這次的作業讓我對 Graph 的基本操作有更清楚的理解，也學會了 DFS 和 BFS 的差異以及應用情境。雖然程式本身不算複雜，但在邏輯上需要很清楚每一步在做什麼，特別是在 visited 陣列的控制上，是整個程式正確與否的關鍵。
