#include <iostream>
using namespace std;

void sub(char set[], int subset[], int subsetSize) {
    cout << "{ ";
    for (int i = 0; i < subsetSize; i++) {
        cout << set[subset[i]] << " ";
    }
    cout << "}" << endl;
}

void powerset(char set[], int n, int subset[], int subsetSize, int index) {
    if (index == n) { 
        sub(set, subset, subsetSize);
        return;
    }

    powerset(set, n, subset, subsetSize, index + 1);

    subset[subsetSize] = index;
    powerset(set, n, subset, subsetSize + 1, index + 1);
}

int main() {
    char set[] = { 'a', 'b', 'c' };
    int n = sizeof(set) / sizeof(set[0]);
    int subset[10];

    cout << "Power set of S = {a, b, c} is:" << endl;
    powerset(set, n, subset, 0, 0);

    return 0;
}
