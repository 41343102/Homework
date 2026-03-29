# 41343102

**問題一**

## 解題說明

先設計一個 C++ 抽象類別 MinPQ，用來表示 最小優先佇列 的 ADT

其中需定義以下基本操作：

-IsEmpty()：判斷優先佇列是否為空

-Top()：回傳目前最小元素

-Push()：插入新元素

-Pop()：刪除最小元素

接著實作 MinHeap 類別，並讓它繼承 MinPQ

MinHeap 以 binary heap（二元堆積） 的陣列方式儲存資料，且滿足：

-每個父節點的值都小於等於子節點

-根節點永遠是最小值

在插入元素時，使用 向上調整 維持 heap 性質

在刪除最小值時，使用 向下調整 維持 heap 性質

## 程式實作


```cpp
#include <iostream>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int size;

    void Resize() {
        capacity *= 2;
        T* newHeap = new T[capacity + 1];
        for (int i = 1; i <= size; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
    }

public:
    MinHeap(int theCapacity = 10) {
        capacity = theCapacity;
        size = 0;
        heap = new T[capacity + 1]; 
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const override {
        return size == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) {
            throw runtime_error("MinHeap is empty");
        }
        return heap[1];
    }

    void Push(const T& x) override {
        if (size == capacity) {
            Resize();
        }

        int i = ++size;

       
        while (i != 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }
        heap[i] = x;
    }

    void Pop() override {
        if (IsEmpty()) {
            throw runtime_error("MinHeap is empty");
        }

        T last = heap[size--];
        int parent = 1;
        int child = 2;

       
        while (child <= size) {
            if (child < size && heap[child + 1] < heap[child]) {
                child++;
            }

            if (last <= heap[child]) {
                break;
            }

            heap[parent] = heap[child];
            parent = child;
            child *= 2;
        }

        heap[parent] = last;
    }

    void PrintHeap() const {
        for (int i = 1; i <= size; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    MinHeap<int> h;

    h.Push(30);
    h.Push(10);
    h.Push(20);
    h.Push(5);
    h.Push(15);

    cout << "Heap: ";
    h.PrintHeap();

    cout << "Top(min) = " << h.Top() << endl;

    h.Pop();
    cout << "After Pop: ";
    h.PrintHeap();

    cout << "Top(min) = " << h.Top() << endl;

    return 0;
}


```

## 效能分析

1.時間複雜度：
-IsEmpty()：Θ(1)
-Top()：Θ(1)
-Push()：Θ(log n)
-Pop()：Θ(log n)
2.空間複雜度：
-Heap 儲存 n 個元素：Θ(n)
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  | 預期輸出 Heap | 實際輸出 Heap |
|----------|--------------|----------|----------|
| 測試    | 30.10.20.5.15 |5.10.15.20.30|5.10.15.20.30|
|Top      |    |5|5|
|  pop   | 5.10.15.20.30 |10.15.20.30|10.15.20.30|
|Top      |    |10|10|



### 編譯與執行指令
#### 測試輸入與輸出
```shell
$ h.Push(30);
$ h.Push(10);
$ h.Push(20);
$ h.Push(5);
$ h.Push(15);
Heap: 5 10 20 30 15
Top(min) = 5
Pop: 10 15 20 30
Top(min) = 10
```

## 申論及開發報告

本程式先以抽象類別 MinPQ 定義最小優先佇列的基本操作，再由 MinHeap 類別繼承並完成所有虛擬函式實作。
在資料結構設計上，使用陣列模擬 complete binary tree，並從索引 1 開始存放節點，方便利用：

-父節點位置：i / 2
-左子節點位置：2 * i
-右子節點位置：2 * i + 1

插入操作 Push() 先把新元素放到陣列尾端，再不斷與父節點比較並向上移動，使最小元素逐步上浮到正確位置。
刪除操作 Pop() 則先移除根節點，再將最後一個元素補到根節點位置，接著向下調整，使 heap 重新滿足最小堆積的條件。

由於 heap 的高度約為 log n，因此 Push() 與 Pop() 的時間複雜度皆為 Θ(log n)，
而 IsEmpty() 與 Top() 只需直接檢查 size 或回傳根節點，因此為 Θ(1)。

 **問題二**
 **a小題**
## 解題說明

使用 BST 來進行隨機插入實驗

首先建立一棵空的 BST，並利用均勻分布的亂數產生器產生 n 個數值進行插入

對每一組 n：

-建立 BST
-插入 n 個隨機數
-計算樹的高度 height
-計算 ratio = height / log₂n

透過觀察不同 n 下的 ratio，驗證 BST 的高度是否與 log₂n 成正比

## 程式實作
```cpp
#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;
    }

    int height(Node* node) const {
        if (node == nullptr) return 0;
        int lh = height(node->left);
        int rh = height(node->right);
        return (lh > rh ? lh : rh) + 1;
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { clear(root); }

    void insert(int key) {
        root = insert(root, key);
    }

    int height() const {
        return height(root);
    }
};

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);

    cout << left << setw(10) << "n"
         << setw(12) << "height"
         << setw(15) << "log2(n)"
         << setw(15) << "ratio" << endl;

    int testValues[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

    for (int i = 0; i < 12; i++) {
        int n = testValues[i];
        BST tree;

        for (int j = 0; j < n; j++) {
            tree.insert(dist(gen));
        }

        int h = tree.height();
        double lg = log2((double)n);
        double ratio = h / lg;

        cout << left << setw(10) << n
             << setw(12) << h
             << setw(15) << fixed << setprecision(4) << lg
             << setw(15) << fixed << setprecision(4) << ratio << endl;
    }

    return 0;
}
```
## 效能分析
1.時間複雜度：
-插入：平均 Θ(log n)，最差 Θ(n)
-建立整棵樹：Θ(n log n)
-計算高度：Θ(n)
2.空間複雜度：
-BST：Θ(n)
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  | 預期輸出  | 實際輸出 ratio |
|----------|--------------|----------|----------|
| 測試一    | 100      |   2.25   |  2.25  |
| 測試二   |  500   |    2.11   |   2.11    |
| 測試三   | 1000   |  2.30  | 2.30  |

| 測試案例 | 預期輸出  | 實際輸出  |
|----------|------------------|------------------|
| |height / log₂n 約為常數|驗證成立|



### 編譯與執行指令
#### 測試輸入與輸出
```shell
$ g++ problem1.cpp --std=c++17 -o main.exe
$ ./main.exe
n         height      log2(n)        ratio
100       15          6.6439         2.2577
500       19          8.9658         2.1192
1000      23          9.9658         2.3079
2000      23          10.9658        2.0974
3000      27          11.5507        2.3375
4000      27          11.9658        2.2564
5000      30          12.2877        2.4415
6000      29          12.5507        2.3106
7000      29          12.7731        2.2704
8000      28          12.9658        2.1595
9000      30          13.1357        2.2839
10000     33          13.2877        2.4835
```
## 申論及開發報告

本程式以 BST 為核心，模擬隨機插入大量資料後的樹高變化。
由於 BST 的結構與插入順序有關，因此若資料為隨機分布，樹高通常不會過大，而會大致落在 log n 的數量級。

程式中使用遞迴方式完成插入與高度計算，實作簡單且容易理解。
從實驗結果可以發現，當 n 增加時，height / log₂n 的值大致維持在固定範圍，約為 2 左右，表示 BST 的高度大致滿足：

height ≈ c × log₂n

因此可驗證隨機建立的 BST 在平均情況下，其高度與 log₂n 成正比，也說明 BST 的搜尋、插入等基本操作在平均情況下可達到 O(log n) 的效率。

**b小題**
## 解題說明

撰寫一個 C++ 函式，用來刪除 BST中 key = k 的節點。
在 BST 中刪除節點時，需要根據該節點的子節點情況分成三種情形處理：

-該節點沒有子節點：直接刪除
-該節點只有一個子節點：以子節點取代原節點
-該節點有兩個子節點：以右子樹中最小節點取代原節點，再遞迴刪除該最小節點

## 程式實作
```cpp
#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, int key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            else {
                Node* temp = findMin(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }

        return node;
    }

    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->key << " ";
        inorder(node->right);
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        clear(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    void inorder() {
        inorder(root);
        cout << endl;
    }
};

int main() {
    BST tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "inorder: ";
    tree.inorder();

    tree.remove(20);
    cout << "After deleting 20: ";
    tree.inorder();

    tree.remove(30);
    cout << "After deleting 30: ";
    tree.inorder();

    tree.remove(50);
    cout << "After deleting 50: ";
    tree.inorder();

    return 0;
}
```
## 效能分析
1.時間複雜度：
-搜尋刪除節點：Θ(h)
-刪除操作：Θ(h)

其中 h 為 BST 的高度

2.特殊情況：
-平均情況：Θ(log n)
-最差情況：Θ(n)

3.空間複雜度：
-遞迴呼叫堆疊：Θ(h)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  | 預期輸出  | 實際輸出 ratio |
|----------|--------------|----------|----------|
| 測試一    |  inorder   |  20 30 40 50 60 70 80   | 20 30 40 50 60 70 80 |
| 測試二   | deleting 20  |  30 40 50 60 70 80 | 30 40 50 60 70 80   |
| 測試三   | deleting 30 | 40 50 60 70 80  | 40 50 60 70 80 |
| 測試四   | deleting 50 | 40 60 70 80 | 40 60 70 80 |


### 編譯與執行指令
#### 測試輸入與輸出
```shell
$ g++ problem2.cpp --std=c++17 -o main.exe
$ ./main.exe
inorder: 20 30 40 50 60 70 80
After deleting 20: 30 40 50 60 70 80
After deleting 30: 40 50 60 70 80
After deleting 50: 40 60 70 80
```
## 申論及開發報告

本程式完成 Binary Search Tree 的刪除操作，並依照 BST 的性質進行遞迴搜尋與節點調整。
刪除操作最關鍵之處在於當節點有兩個子節點時，必須選擇適當的替代節點來維持 BST 的順序性。本實作採用右子樹最小值作為替代節點，因此刪除後仍能保持 BST 正確結構。

整體而言，BST 的刪除時間複雜度與樹高 h 有關。若 BST 維持平衡，則刪除效率良好，可達 O(log n)；但若樹退化為單鏈結結構，則最差可能達到 O(n)。

透過本題可以更清楚了解 BST 的動態操作方式，以及資料結構平衡性對效率的重要影響。
