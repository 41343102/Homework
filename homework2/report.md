# 41343102

問題一

## 解題說明

用 Term* termArray 動態陣列存放項，並維護 capacity、terms。

每次插入或運算後以 normalize()：

- 依指數降冪排序

- 合併同冪（相同 exp 的係數相加）
 
- 去零項（係數為 0 刪除）

加法(Add)：視為兩個已排序陣列的合併

乘法(Mult)：雙重迴圈逐項相乘，最後 normalize() 合併同冪

代入求值(Eval)：將每項 coeff * x^exp 相加

## 程式實作


```cpp
#include <iostream>
using namespace std;

class Term {
    friend class Polynomial;
private:
    float coeff;  // 係數
    int   exp;    // 指數
};
class Polynomial {
public:
    Polynomial()
        : termArray(nullptr), capacity(0), terms(0) {
        reserve(1);
    }
    Polynomial(const Polynomial& other)
        : termArray(nullptr), capacity(0), terms(0) {
        reserve(other.capacity);
        terms = other.terms;
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        reserve(other.capacity);
        terms = other.terms;
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
        return *this;
    }
    ~Polynomial() { delete[] termArray; }
    Polynomial Add(Polynomial poly) const {
        Polynomial res;
        int i = 0, j = 0;
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float c = termArray[i].coeff + poly.termArray[j].coeff;
                if (c != 0.0f) res.pushBack(c, termArray[i].exp);
                ++i; ++j; }
            else if (termArray[i].exp > poly.termArray[j].exp) {
                res.pushBack(termArray[i].coeff, termArray[i].exp);
                ++i;}
            else {
                res.pushBack(poly.termArray[j].coeff, poly.termArray[j].exp);
                ++j;
            }
        }
        while (i < terms) { res.pushBack(termArray[i].coeff, termArray[i].exp); ++i; }
        while (j < poly.terms) { res.pushBack(poly.termArray[j].coeff, poly.termArray[j].exp); ++j; }
        res.normalize(); // 合併同冪
        return res;
    }
    Polynomial Mult(Polynomial poly) const {
        Polynomial res;
        //兩兩相乘後丟進 res，最後 normalize 合併同冪
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < poly.terms; ++j) {
                float c = termArray[i].coeff * poly.termArray[j].coeff;
                int   e = termArray[i].exp + poly.termArray[j].exp;
                if (c != 0.0f) res.pushBack(c, e);
            }
        } res.normalize();
        return res;
    }
    float Eval(float f) const {
        float ans = 0.0f;
        for (int i = 0; i < terms; ++i) {
            ans += termArray[i].coeff * ipow(f, termArray[i].exp);
        } return ans;
    }
    void NewTerm(float c, int e) {
        if (c == 0.0f) return;
        pushBack(c, e);
        normalize();
    }
    void debugPrint() const {
        if (terms == 0) { cout << "0"; return; }
        for (int i = 0; i < terms; ++i) {
            const Term& t = termArray[i];
            if (i && t.coeff >= 0) cout << "+";
            cout << t.coeff;
            if (t.exp != 0) cout << "x^" << t.exp;
        }
    }
private:
    Term* termArray;  // 非零項陣列
    int   capacity;   // 陣列容量
    int   terms;      // 目前項數
    static float ipow(float base, int exp) {
        if (exp == 0) return 1.0f;
        float r = 1.0f;
        for (int i = 0; i < exp; ++i) r *= base;
        return r;
    }
    void reserve(int cap) {
        if (cap <= capacity) return;
        if (cap < 1) cap = 1;
        Term* nArr = new Term[cap];
        for (int i = 0; i < terms; ++i) nArr[i] = termArray[i];
        delete[] termArray;
        termArray = nArr;
        capacity = cap;
    }
    void pushBack(float c, int e) {
        if (terms == capacity) reserve(capacity * 2);
        termArray[terms].coeff = c;
        termArray[terms].exp = e;
        ++terms;
    }
    void normalize() {
        if (terms <= 1) {
            
            if (terms == 1 && termArray[0].coeff == 0.0f) terms = 0;
            return;
        }
        for (int i = 1; i < terms; ++i) {
            Term key = termArray[i];
            int k = i - 1;
            while (k >= 0 && termArray[k].exp < key.exp) {
                termArray[k + 1] = termArray[k];
                --k;
            } termArray[k + 1] = key;
        }
        int w = 0; // write index
        for (int r = 0; r < terms; ) {
            float c = termArray[r].coeff;
            int   e = termArray[r].exp;
            int   r2 = r + 1;
            while (r2 < terms && termArray[r2].exp == e) {
                c += termArray[r2].coeff;
                ++r2;
            }
            if (c != 0.0f) {
                termArray[w].coeff = c;
                termArray[w].exp = e;
                ++w;
            } r = r2;
        }terms = w;
        if (terms == 0) return;
    }
};


int main() {
     Polynomial p, q;
     p.NewTerm(3, 2);
     p.NewTerm(2, 1);     
     p.NewTerm(1, 0);
     q.NewTerm(-1, 2);
     q.NewTerm(4, 0);

     Polynomial s = p.Add(q);  
     Polynomial m = p.Mult(q); 

     cout << "p(x) = "; p.debugPrint(); cout << "\n";
     cout << "q(x) = "; q.debugPrint(); cout << "\n";
     cout << "p+q  = "; s.debugPrint(); cout << "\n";
     cout << "p*q  = "; m.debugPrint(); cout << "\n";
     cout << "p(2) = " << p.Eval(2) << "\n";
 }

```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(\log n)$。
2. 空間複雜度：空間複雜度為 $O(100\times \log n + \pi)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因
