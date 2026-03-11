#include <iostream>
#include <queue>
using namespace std;

template <class T> class Tree;

template <class T>
class TreeNode {
    friend class Tree<T>;
public:
    TreeNode(TreeNode<T>* left, const T& item, TreeNode<T>* right)
        : data(item), leftChild(left), rightChild(right) {}

private:
    T data;
    TreeNode<T>* leftChild = nullptr;
    TreeNode<T>* rightChild = nullptr;
};

template <class T>
class Tree {
public:
    Tree() = default;
    explicit Tree(TreeNode<T>* r) : root(r) {}

    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    ~Tree() {
        clear(root);
        root = nullptr;
    }

    void setRoot(TreeNode<T>* r) { root = r; }

    TreeNode<T>* makeNode(TreeNode<T>* left, const T& item, TreeNode<T>* right) {
        return new TreeNode<T>(left, item, right);
    }

    void preorder() const {
        preorder(root);
        cout << endl;
    }

    void inorder() const {
        inorder(root);
        cout << endl;
    }

    void postorder() const {
        postorder(root);
        cout << endl;
    }

    void levelorder() const {
        if (!root) {
            cout << endl;
            return;
        }

        queue<TreeNode<T>*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode<T>* cur = q.front();
            q.pop();

            cout << cur->data << ' ';

            if (cur->leftChild) q.push(cur->leftChild);
            if (cur->rightChild) q.push(cur->rightChild);
        }
        cout << endl;
    }

private:
    TreeNode<T>* root = nullptr;

    static void preorder(TreeNode<T>* n) {
        if (!n) return;
        cout << n->data << ' ';
        preorder(n->leftChild);
        preorder(n->rightChild);
    }

    static void inorder(TreeNode<T>* n) {
        if (!n) return;
        inorder(n->leftChild);
        cout << n->data << ' ';
        inorder(n->rightChild);
    }

    static void postorder(TreeNode<T>* n) {
        if (!n) return;
        postorder(n->leftChild);
        postorder(n->rightChild);
        cout << n->data << ' ';
    }

    static void clear(TreeNode<T>* n) {
        if (!n) return;
        clear(n->leftChild);
        clear(n->rightChild);
        delete n;
    }
};

int main() {
    Tree<char> t;

    auto* B = t.makeNode(nullptr, 'B', nullptr);
    auto* A = t.makeNode(nullptr, 'A', nullptr);
    auto* E = t.makeNode(B, 'E', A);
    auto* I = t.makeNode(E, 'I', nullptr);
    auto* D = t.makeNode(nullptr, 'D', nullptr);
    auto* H = t.makeNode(nullptr, 'H', D);
    auto* K = t.makeNode(I, 'K', H);
    auto* C = t.makeNode(nullptr, 'C', nullptr);
    auto* G = t.makeNode(C, 'G', nullptr);
    auto* F = t.makeNode(nullptr, 'F', nullptr);
    auto* J = t.makeNode(G, 'J', F);
    auto* L = t.makeNode(K, 'L', J);

    t.setRoot(L);

    cout << "preorder: ";
    t.preorder();

    cout << "inorder: ";
    t.inorder();

    cout << "postorder: ";
    t.postorder();

    cout << "levelorder: ";
    t.levelorder();

    return 0;
}
