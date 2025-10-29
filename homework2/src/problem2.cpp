#include <iostream>
using namespace std;

class Polynomial;
class Term {
    friend class Polynomial;
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coeff;
    int   exp;
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
                ++i; ++j;
            }
            else if (termArray[i].exp > poly.termArray[j].exp) {
                res.pushBack(termArray[i].coeff, termArray[i].exp);
                ++i;
            }
            else {
                res.pushBack(poly.termArray[j].coeff, poly.termArray[j].exp);
                ++j;
            }
        }
        while (i < terms) { res.pushBack(termArray[i].coeff, termArray[i].exp); ++i; }
        while (j < poly.terms) { res.pushBack(poly.termArray[j].coeff, poly.termArray[j].exp); ++j; }
        res.normalize();
        return res;
    }
    Polynomial Mult(Polynomial poly) const {
        Polynomial res;
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < poly.terms; ++j) {
                float c = termArray[i].coeff * poly.termArray[j].coeff;
                int   e = termArray[i].exp + poly.termArray[j].exp;
                if (c != 0.0f) res.pushBack(c, e);
            }
        }
        res.normalize();
        return res;
    }
    float Eval(float f) const {
        float ans = 0.0f;
        for (int i = 0; i < terms; ++i) {
            ans += termArray[i].coeff * ipow(f, termArray[i].exp);
        }
        return ans;
    }
    void NewTerm(float c, int e) {
        if (c == 0.0f) return;
        pushBack(c, e);
        normalize();
    }
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
private:
    Term* termArray;
    int   capacity;
    int   terms;
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
            }
            termArray[k + 1] = key;
        }
        int w = 0;
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
            }
            r = r2;
        }
        terms = w;
    }
};
istream& operator>>(istream& is, Polynomial& p) {
    int k;
    if (!(is >> k)) return is;
    p.terms = 0;
    p.reserve(k > 1 ? k : 1);

    for (int i = 0; i < k; ++i) {
        float c; int e;
        is >> c >> e;
        if (!is) return is;
        if (c != 0.0f) p.pushBack(c, e);
    }
    p.normalize();
    return is;
}
ostream& operator<<(ostream& os, const Polynomial& p) {
    os << "terms=" << p.terms << "  ";
    if (p.terms == 0) { os << "0"; return os; }
    for (int i = 0; i < p.terms; ++i) {
        float c = p.termArray[i].coeff;
        int   e = p.termArray[i].exp;

        if (i > 0 && c >= 0) os << "+";
        if (e == 0) {
            os << c;
        }
        else {
            os << c << "X";
            if (e != 1) os << "^" << e;
        }
    }
    return os;
}
int main() {
    Polynomial p, q;
    cout << "p:";
    cin >> p;
    cout << "q:";
    cin >> q;
    cout << "p:   " << p << "\n";
    cout << "q:   " << q << "\n";
    Polynomial s = p.Add(q);
    Polynomial m = p.Mult(q);
    cout << "p+q: " << s << "\n";
    cout << "p*q: " << m << "\n";
    float x;
    cout << "x:";
    cin >> x;
    cout << "p(" << x << ") = " << p.Eval(x) << "\n";
    return 0;
}