#include <iostream>
using namespace std;

int Ack(int m, int n) {
    if (m == 0) return n + 1;
    else if (m == 1) return n + 2;
    else if (m == 2) return 2 * n + 3;
    else if (m == 3) {
        int result = 1;              
        for (int i = 0; i < n + 3; i++) {
            result *= 2;
        }
        return result - 3;
    }
    else {
        cout << "m 太大，非遞迴版本不支援 m > 3\n";
        return 0;
    }
}

int main() {
    int m, n;
    cout << "m: ";
    cin >> m;
    cout << "n: ";
    cin >> n;

    cout << "Ack(" << m << "," << n << ") = " << Ack(m, n) << "\n";
    return 0;
}
