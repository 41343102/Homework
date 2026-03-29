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

    int height(Node* node) {
        if (node == nullptr) return 0;
        int leftH = height(node->left);
        int rightH = height(node->right);
        return max(leftH, rightH) + 1;
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

    int height() {
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
        << setw(15) << "height/log2n" << endl;

    for (int n = 100; n <= 10000; ) {
        BST tree;

        for (int i = 0; i < n; i++) {
            int value = dist(gen);
            tree.insert(value);
        }

        int h = tree.height();
        double lg = log2((double)n);
        double ratio = h / lg;

        cout << left << setw(10) << n
            << setw(12) << h
            << setw(15) << fixed << setprecision(4) << lg
            << setw(15) << fixed << setprecision(4) << ratio
            << endl;

        if (n == 100) n = 500;
        else n += 500;
    }

    return 0;
}
