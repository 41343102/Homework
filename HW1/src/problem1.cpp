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
