#include <iostream>
using namespace std;

class Polynomial; // forward declaration

class Term {
    friend Polynomial;   // 只允許 Polynomial 存取 Term 的私有成員
private:
    float coef;          // 係數 (非 0)
    int   exp;           // 指數 (>=0)
};

class Polynomial {
private:
    Term* termArray;   // 動態陣列：存放非零項
    int   capacity;    // 陣列容量
    int   terms;       // 目前項數

    // ---- 內部工具 ----
    void ensureCapacity() {
        if (terms < capacity) return;
        capacity *= 2;
        Term* tmp = new Term[capacity];
        for (int i = 0; i < terms; ++i) tmp[i] = termArray[i]; // 手動複製（只用 iostream）
        delete[] termArray;
        termArray = tmp;             // 重新指向新陣列（關鍵）
    }
    // 追加在尾端（僅限已知輸入按 exp 遞減時使用）
    void pushBack(float c, int e) {
        if (c == 0) return;
        ensureCapacity();
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        ++terms;
    }
    // 依指數由大到小插入；若同指數則合併
    void insertMerge(float c, int e) {
        if (c == 0) return;
        // 找到應插入的位置
        int pos = 0;
        while (pos < terms && termArray[pos].exp > e) ++pos;

        if (pos < terms && termArray[pos].exp == e) {
            termArray[pos].coef += c;               // 同冪相加
            if (termArray[pos].coef == 0) {         // 變 0 則刪除
                for (int k = pos + 1; k < terms; ++k) termArray[k - 1] = termArray[k];
                --terms;
            }
            return;
        }
        // 需要插入新節點
        ensureCapacity();
        for (int k = terms; k > pos; --k) termArray[k] = termArray[k - 1]; // 右移
        termArray[pos].coef = c;
        termArray[pos].exp = e;
        ++terms;
    }

public:
    // Construct the polynomial p(x) = 0
    Polynomial() : termArray(nullptr), capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    // Return the sum of the polynomials *this and poly.
    Polynomial Add(Polynomial poly) {
        Polynomial c;
        int i = 0, j = 0;
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float sum = termArray[i].coef + poly.termArray[j].coef;
                if (sum != 0) c.pushBack(sum, termArray[i].exp);
                ++i; ++j;
            }
            else if (termArray[i].exp > poly.termArray[j].exp) {
                c.pushBack(termArray[i].coef, termArray[i].exp);
                ++i;
            }
            else {
                c.pushBack(poly.termArray[j].coef, poly.termArray[j].exp);
                ++j;
            }
        }
        while (i < terms) { c.pushBack(termArray[i].coef, termArray[i].exp); ++i; }
        while (j < poly.terms) { c.pushBack(poly.termArray[j].coef, poly.termArray[j].exp); ++j; }
        return c;
    }

    // Return the product of the polynomials *this and poly.
    Polynomial Mult(Polynomial poly) {
        Polynomial c;
        for (int i = 0; i < terms; ++i)
            for (int j = 0; j < poly.terms; ++j)
                c.insertMerge(termArray[i].coef * poly.termArray[j].coef,
                    termArray[i].exp + poly.termArray[j].exp);
        return c;
    }

    // Evaluate *this at f and return the result.
    float Eval(float f) {
        // 直接按各項 c * f^e 逐項累加（只用 iostream，自己算 f^e）
        auto fpow = [](float x, int p) {
            float r = 1.0f;
            while (p--) r *= x;
            return r;
        };
        float ans = 0.0f;
        for (int i = 0; i < terms; ++i)
            ans += termArray[i].coef * fpow(f, termArray[i].exp);
        return ans;
    }

    // 友元輸入輸出（方便主程式使用；維持介面仍符合圖片的 public 區）
    friend istream& operator>>(istream& is, Polynomial& P);
    friend ostream& operator<<(ostream& os, const Polynomial& P);

    // 解構
    ~Polynomial() { delete[] termArray; }
};

// 讀入：先讀項數 n，再讀 n 組 (coef exp)
// 假設輸入已照「指數遞減」排序
istream& operator>>(istream& is, Polynomial& P) {
    int n; if (!(is >> n)) return is;
    P.terms = 0; // 重新填入
    for (int i = 0; i < n; ++i) {
        float c; int e; is >> c >> e;
        P.pushBack(c, e);
    }
    return is;
}

// 輸出：aX^e + aX^e + ...
ostream& operator<<(ostream& os, const Polynomial& P) {
    if (P.terms == 0) { os << 0; return os; }
    for (int i = 0; i < P.terms; ++i) {
        if (i) os << " + ";
        os << P.termArray[i].coef << "X^" << P.termArray[i].exp;
    }
    return os;
}

// -------------------- 範例主程式：兩個多項式相加 --------------------
int main() {
    Polynomial A, B;
    // 輸入格式：n  (c1 e1) (c2 e2) ...  然後 m (d1 f1) ...
    // 例如： A= 5X^2 + 3X^1 + 7X^0  -> 3 5 2 3 1 7 0
    //       B= 1X^1 + 2X^0          -> 2 1 1 2 0
    cin >> A >> B;

    Polynomial C = A.Add(B);
    cout << C << "\n";
    return 0;
}
