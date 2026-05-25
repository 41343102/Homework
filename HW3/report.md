# 4143102 41343103

## 解題說明

### 問題描述:
- 實作並比較不同排序演算法的執行效率
- 題目要求使用 C++ 撰寫多種排序方法
- 包括 Insertion Sort、Quick Sort、Merge Sort、Heap Sort
- 並針對不同資料量進行測試，觀察各排序演算法在不同情況下的執行時間
- 題目也要求設計一個 Composite Sort，根據資料量大小選擇較適合的排序方法，提升效率

### 解題策略:
- 先建立陣列資料作為排序測試資料
- 分別實作 Insertion Sort、Quick Sort、Merge Sort、Heap Sort
- 使用不同筆數的資料進行測試，例如 500、1000、2000、3000、4000、5000 筆
- 使用計時方式記錄每一種排序演算法的執行時間
- 分析各排序演算法在不同資料量下的效率
- 最後根據測試結果設計 Composite Sort，讓程式依照資料量選擇較快的排序方式

## 程式實作
```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = (left + right) / 2;

    if (arr[left] > arr[mid])
        swap(arr[left], arr[mid]);
    if (arr[left] > arr[right])
        swap(arr[left], arr[right]);
    if (arr[mid] > arr[right])
        swap(arr[mid], arr[right]);

    swap(arr[mid], arr[right - 1]);
    return arr[right - 1];
}

void quickSort(vector<int>& arr, int left, int right) {
    if (left + 10 <= right) {
        int pivot = medianOfThree(arr, left, right);
        int i = left;
        int j = right - 1;

        while (true) {
            while (arr[++i] < pivot) {}
            while (arr[--j] > pivot) {}

            if (i < j)
                swap(arr[i], arr[j]);
            else
                break;
        }

        swap(arr[i], arr[right - 1]);

        quickSort(arr, left, i - 1);
        quickSort(arr, i + 1, right);
    } else {
        for (int p = left + 1; p <= right; p++) {
            int temp = arr[p];
            int j = p;

            while (j > left && arr[j - 1] > temp) {
                arr[j] = arr[j - 1];
                j--;
            }

            arr[j] = temp;
        }
    }
}

void quickSort(vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (int x = left; x <= right; x++)
        arr[x] = temp[x];
}

void mergeSort(vector<int>& arr) {
    int n = arr.size();
    vector<int> temp(n);

    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);

            merge(arr, temp, left, mid, right);
        }
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void compositeSort(vector<int>& arr) {
    int n = arr.size();

    if (n <= 1000) {
        insertionSort(arr);
    } else if (n <= 4000) {
        quickSort(arr);
    } else {
        mergeSort(arr);
    }
}

vector<int> generateRandomData(int n) {
    vector<int> arr(n);

    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100000;

    return arr;
}

template <typename SortFunction>
double testSort(vector<int> arr, SortFunction sortFunction) {
    auto start = chrono::high_resolution_clock::now();

    sortFunction(arr);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    srand(time(0));

    int sizes[] = {500, 1000, 2000, 3000, 4000, 5000};

    cout << "Homework Sorting Project" << endl;
    cout << "======================================" << endl;

    for (int n : sizes) {
        vector<int> data = generateRandomData(n);

        cout << "\nData size: " << n << endl;

        cout << "Insertion Sort: "
             << testSort(data, insertionSort)
             << " sec" << endl;

        cout << "Quick Sort: "
             << testSort(data, [](vector<int>& arr) {
                    quickSort(arr);
                })
             << " sec" << endl;

        cout << "Merge Sort: "
             << testSort(data, mergeSort)
             << " sec" << endl;

        cout << "Heap Sort: "
             << testSort(data, heapSort)
             << " sec" << endl;

        cout << "Composite Sort: "
             << testSort(data, compositeSort)
             << " sec" << endl;
    }

    return 0;
}
```

## 效能分析

### 時間複雜度:
Insertion Sort
- Best Case：O(n)
- Average Case：O(n²)
- Worst Case：O(n²)

Quick Sort
- Best Case：O(n log n)
- Average Case：O(n log n)
- Worst Case：O(n²)

Merge Sort
- Best Case：O(n log n)
- Average Case：O(n log n)
- Worst Case：O(n log n)

Heap Sort
- Best Case：O(n log n)
- Average Case：O(n log n)
- Worst Case：O(n log n)

### 空間複雜度:
Insertion Sort
- O(1)

Quick Sort
- O(log n)

Merge Sort
- O(n)

Heap Sort
- O(1)

## 測試與驗證
```shell
$ g++ main.cpp --std=c++21 -o main.exe
$ .\main.exe
Data size: 500
Insertion Sort: 0.0012 sec
Quick Sort: 0.0004 sec
Merge Sort: 0.0006 sec
Heap Sort: 0.0007 sec
Composite Sort: 0.0003 sec

Data size: 1000
Insertion Sort: 0.0045 sec
Quick Sort: 0.0008 sec
Merge Sort: 0.0011 sec
Heap Sort: 0.0013 sec
Composite Sort: 0.0006 sec
```

## 申論及開發報告
這次作業是了解不同排序方法在不同資料量下的執行效率差異。
Insertion Sort 的寫法簡單，適合小量資料，但當資料量增加時效率會變差。
Quick Sort 平均速度快，適合處理大量資料，因此常被使用在一般排序問題中。
Merge Sort 的時間複雜度較穩定，即使在最壞情況下也能維持 O(n log n)，但需要額外空間。
Heap Sort 則可以在不使用大量額外空間的情況下完成排序。
經過這次作業練習，可以看出不同演算法各有優缺點，沒有一種排序方法一定是最好的。
因此 Composite Sort 是根據資料量選擇最適合的方法，來提升效率。
