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
    }
    else {
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
    }
    else if (n <= 4000) {
        quickSort(arr);
    }
    else {
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
    int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };
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