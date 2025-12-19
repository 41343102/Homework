# 41343102

**問題**

## 解題說明

以**循環單向鏈結串列（circular linked list）並搭配表頭節點（header node）**來表示多項式。表頭節點本身不存放資料，當多項式為空時，其指標會指回自己，能簡化插入與刪除操作。

多項式中的每一項包含係數、指數與指向下一項的指標，並依指數遞減順序儲存。輸入時，程式會依序讀入多項式的項數與各項的係數與指數，透過排序插入方式放入鏈結串列中，若遇到相同指數則進行係數合併，合併後係數為零的項目會被刪除。

加法與減法運算採用同時走訪兩個已排序串列的方式進行，比較各項指數大小後將結果加入新的多項式；乘法則使用雙層迴圈，將兩多項式的每一項相乘，並將產生的項目插入結果多項式中，同樣由插入函式負責合併相同指數的項目。多項式代入計算則是逐項計算 
coef×X^exp 並加總。

整體實作能正確支援多項式的輸入、輸出、加減乘運算與數值計算，且資料結構清楚，便於後續擴充記憶體管理機制。

## 程式實作

```cpp
#include <iostream>
#include <cmath>

using namespace std;
using std::istream;
using std::ostream;


class Polynomial {
public:
    struct TermNode {
        int coef;
        int exp;
        TermNode* link;
        TermNode(int c = 0, int e = 0, TermNode* l = nullptr)
            : coef(c), exp(e), link(l) {}
    };

    Polynomial() {
        header = new TermNode();
        header->link = header;
    }

    Polynomial(const Polynomial& a) : Polynomial() {
        CopyFrom(a);
    }

    Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            Clear();
            CopyFrom(a);
        }
        return *this;
    }

    ~Polynomial() {
        Clear();
        delete header;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        x.Clear();
        int n;
        is >> n;
        for (int i = 0; i < n; ++i) {
            int c, e;
            is >> c >> e;
            x.InsertTermSorted(c, e);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        int n = 0;
        for (TermNode* p = x.header->link; p != x.header; p = p->link)
            ++n;

        os << n;
        for (TermNode* p = x.header->link; p != x.header; p = p->link)
            os << " " << p->coef << " " << p->exp;

        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        TermNode* p = header->link;
        TermNode* q = b.header->link;

        while (p != header && q != b.header) {
            if (p->exp > q->exp) {
                result.InsertTermSorted(p->coef, p->exp);
                p = p->link;
            }
            else if (p->exp < q->exp) {
                result.InsertTermSorted(q->coef, q->exp);
                q = q->link;
            }
            else {
                result.InsertTermSorted(p->coef + q->coef, p->exp);
                p = p->link;
                q = q->link;
            }
        }

        while (p != header) {
            result.InsertTermSorted(p->coef, p->exp);
            p = p->link;
        }

        while (q != b.header) {
            result.InsertTermSorted(q->coef, q->exp);
            q = q->link;
        }

        return result;
    }

  
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        TermNode* p = header->link;
        TermNode* q = b.header->link;

        while (p != header && q != b.header) {
            if (p->exp > q->exp) {
                result.InsertTermSorted(p->coef, p->exp);
                p = p->link;
            }
            else if (p->exp < q->exp) {
                result.InsertTermSorted(-q->coef, q->exp);
                q = q->link;
            }
            else {
                result.InsertTermSorted(p->coef - q->coef, p->exp);
                p = p->link;
                q = q->link;
            }
        }

        while (p != header) {
            result.InsertTermSorted(p->coef, p->exp);
            p = p->link;
        }

        while (q != b.header) {
            result.InsertTermSorted(-q->coef, q->exp);
            q = q->link;
        }

        return result;
    }

    
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;

        for (TermNode* p = header->link; p != header; p = p->link) {
            for (TermNode* q = b.header->link; q != b.header; q = q->link) {
                result.InsertTermSorted(
                    p->coef * q->coef,
                    p->exp + q->exp
                );
            }
        }
        return result;
    }

    float Evaluate(float x) const {
        float sum = 0.0f;
        for (TermNode* p = header->link; p != header; p = p->link)
            sum += static_cast<float>(p->coef) *
            pow(x, static_cast<float>(p->exp));
        return sum;
    }

private:
    TermNode* header;

    void Clear() {
        TermNode* p = header->link;
        while (p != header) {
            TermNode* del = p;
            p = p->link;
            delete del;
        }
        header->link = header;
    }

    void CopyFrom(const Polynomial& a) {
        for (TermNode* p = a.header->link; p != a.header; p = p->link)
            InsertTermSorted(p->coef, p->exp);
    }

    void InsertTermSorted(int c, int e) {
        if (c == 0) return;

        TermNode* prev = header;
        TermNode* cur = header->link;

        while (cur != header && cur->exp > e) {
            prev = cur;
            cur = cur->link;
        }

        if (cur != header && cur->exp == e) {
            cur->coef += c;
            if (cur->coef == 0) {
                prev->link = cur->link;
                delete cur;
            }
        }
        else {
            prev->link = new TermNode(c, e, cur);
        }
    }
};

int main() {
    Polynomial a, b;

    cin >> a >> b;

    cout << a + b << endl;
    cout << a - b << endl;
    cout << a * b << endl;
    cout << a.Evaluate(2.0f) << endl;

    return 0;
}
```
## 效能分析

在輸入與插入項目時，最壞情況需走訪整個串列以找到正確位置，時間複雜度為 O(n)。

加法與減法運算同時走訪兩個已排序的多項式串列，各項最多只會被處理一次，因此時間複雜度為 O(n+m)。

乘法運算採用雙層迴圈，需將第一個多項式的每一項與第二個多項式的每一項相乘，其時間複雜度為 O(nm)，
其中 n 與 m 為兩多項式的項數。

多項式代入計算需逐項計算並加總，時間複雜度為 O(n)。
整體而言，本實作在多項式項數不大的情況下具有良好可讀性與可維護性，效能表現亦符合本作業需求。

## 測試與驗證

### 測試案例


| 測試案例 | 輸入參數 $x$ | 預期輸出 x | 實際輸出 x |
|----------|--------------|----------|----------|
| 測試  | $(3,5,3,2,1,-1,0)$  |          |          |
|          |  $(2,1,1,1,0)$     |           |          |
|    +   |                 |  2 5 3 3 1|2 5 3 3 1|

| 測試案例 | 輸入參數 $x$ | 預期輸出 x | 實際輸出 x |
|----------|--------------|----------|----------|
| 測試  | $(3,5,3,2,1,-1,0)$  |          |          |
|          |  $(2,1,1,1,0)$     |           |          |
|    -   |                 |  3 5 3 1 1 -2 0|3 5 3 1 1 -2 0|

| 測試案例 | 輸入參數 $x$ | 預期輸出 x | 實際輸出 x |
|----------|--------------|----------|----------|
| 測試  | $(3,5,3,2,1,-1,0)$  |          |          |
|          |  $(2,1,1,1,0)$     |           |          |
|    *   |                 |  5 5 4 5 3 2 2 1 1 -1 0|5 5 4 5 3 2 2 1 1 -1 0|

## 申論及開發報告

本次作業透過實作多項式（Polynomial）類別，實際應用課堂中所學的鏈結串列與物件導向設計概念。程式設計上選擇以循環單向鏈結串列搭配表頭節點來表示多項式，能有效簡化邊界情況的處理，例如空多項式或節點的插入與刪除。

在開發過程中，最大的挑戰在於多項式運算的正確性，特別是加法、減法與乘法時，需同時考慮指數排序、相同指數的合併，以及係數為零時的節點刪除。透過將這些邏輯集中於排序插入函式中，不僅降低了重複程式碼，也讓整體結構更清楚且容易維護。

本作業同時實作了複製建構子、指定運算子與解構子，使多項式物件在複製與釋放時能正確管理記憶體，避免資源錯誤。透過本次實作，更加理解資料結構在實際程式設計中的應用方式，以及良好類別設計對程式可讀性與穩定性的影響。

