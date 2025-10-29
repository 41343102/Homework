#include <iostream>
using namespace std;

//-------------------------
// �h�������@���Gcoeff * x^exp
//-------------------------
class Term {
    friend class Polynomial;
private:
    float coeff;  // �Y��
    int   exp;    // ����
};

//-------------------------
// �h�������O�]Fig.1 + Fig.2�^
//-------------------------
class Polynomial {
public:
    // �غc p(x)=0
    Polynomial()
        : termArray(nullptr), capacity(0), terms(0) {
        reserve(1);
    }

    // �`����
    Polynomial(const Polynomial& other)
        : termArray(nullptr), capacity(0), terms(0) {
        reserve(other.capacity);
        terms = other.terms;
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }

    // ����
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        reserve(other.capacity);
        terms = other.terms;
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
        return *this;
    }

    // �Ѻc
    ~Polynomial() { delete[] termArray; }

    // ------- ADT �����]Fig.1�^-------
    // this + poly
    Polynomial Add(Polynomial poly) const {
        Polynomial res;
        // ��}�C�Ҭ������]normalize() �|�O�ҡ^
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
        res.normalize(); // �X�֦P���]�z�פW�w�X�֡^�A�]�B�z���� 0 �����p
        return res;
    }

    // this * poly
    Polynomial Mult(Polynomial poly) const {
        Polynomial res;
        // �����¤�k�G���ۭ����i res�]�Ȥ��X�֡^�A�̫� normalize �X�֦P��
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

    // �N�J f �p�� p(f)
    float Eval(float f) const {
        float ans = 0.0f;
        for (int i = 0; i < terms; ++i) {
            ans += termArray[i].coeff * ipow(f, termArray[i].exp);
        }
        return ans;
    }

    //�]�i��^�s�W�@���G�����թΫغc�ɥ�
    void NewTerm(float c, int e) {
        if (c == 0.0f) return;
        pushBack(c, e);
        normalize();
    }

    //�]�i��^������X
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
    // Fig.2 ���p������
    Term* termArray;  // �D�s���}�C
    int   capacity;   // �}�C�e�q
    int   terms;      // �ثe����

    // --------- �����u�� ---------
    static float ipow(float base, int exp) {
        // �u�ΰj��A���� <cmath>
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

    // �N termArray�G
    // 1) �̫��ƭ����Ƨ�
    // 2) �X�֦P��
    // 3) �h���Y�Ƭ� 0 ����
    void normalize() {
        if (terms <= 1) {
            // �i��涵�Y�Ƭ� 0
            if (terms == 1 && termArray[0].coeff == 0.0f) terms = 0;
            return;
        }

        // insertion sort�G�� exp ����
        for (int i = 1; i < terms; ++i) {
            Term key = termArray[i];
            int k = i - 1;
            while (k >= 0 && termArray[k].exp < key.exp) {
                termArray[k + 1] = termArray[k];
                --k;
            }
            termArray[k + 1] = key;
        }

        // �X�֦P����e�q
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
            }
            r = r2;
        }
        terms = w;

        // ���� 0 �� �M��
        if (terms == 0) return;
    }
};


int main() {
    Polynomial p, q;
    // p = 3x^2 + 2x + 1
    p.NewTerm(3, 2);
    p.NewTerm(2, 1);
    p.NewTerm(1, 0);

    // q = -x^2 + 4
    q.NewTerm(-1, 2);
    q.NewTerm(4, 0);

    Polynomial s = p.Add(q);   // (3x^2 - x^2) + 2x + (1+4) = 2x^2 + 2x + 5
    Polynomial m = p.Mult(q);  // (3x^2+2x+1)*(-x^2+4)

    cout << "p(x) = "; p.debugPrint(); cout << "\n";
    cout << "q(x) = "; q.debugPrint(); cout << "\n";
    cout << "p+q  = "; s.debugPrint(); cout << "\n";
    cout << "p*q  = "; m.debugPrint(); cout << "\n";
    cout << "p(2) = " << p.Eval(2) << "\n";
}