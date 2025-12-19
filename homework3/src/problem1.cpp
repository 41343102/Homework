
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
