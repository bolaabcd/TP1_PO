#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <gmpxx.h>
#include <string>

using namespace std;

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

vector<mpq_class> c, b;
vector<vector<mpq_class>> A, At;

int n, m;

void sum_prod(vector<mpq_class> &result, vector<mpq_class> &vec, mpq_class mult) {
    // result += vec * mult
    // assert(result.size() == vec.size()); // isso eh falso pq o certificado eh incompleto
    for (int i = 0; i < result.size(); i++)
        result[i] += vec[i] * mult;
}

void invi(ifstream &inp) {
    mpq_class fb = 0;
    vector<mpq_class> result(m, 0);

    for (int i = 0; i < n; i++)
    {
        mpq_class cval;
        string s;
        inp >> s;
        cval.set_str(s, 10);
        sum_prod(result,A[i],cval);

        fb += cval * b[i];
    }

    assert(fb < 0);

    for(mpq_class &i : result)
        assert(i >= 0);

}

void checksol(ifstream &inp, mpq_class &expected, bool check_expected) {
    vector<mpq_class> solv(n, 0);
    for (int i = 0; i < m; i++) {
        mpq_class sval;
        string s;
        inp >> s;
        sval.set_str(s, 10);
        assert(sval >= 0); // x >= 0
        sum_prod(solv, At[i], sval);

        expected -= c[i] * sval;
    }

    for(int i = 0; i < n; i++)
        assert(solv[i] <= b[i]); // Ax <= b

    if (check_expected)
        assert(expected == 0); // solucao tem valor otimo falado
}

void ilim(ifstream &inp) {
    mpq_class fc = 0;
    vector<mpq_class> result(m, 0);

    checksol(inp, fc, false);

    for (int i = 0; i < m; i++)
    {
        mpq_class cval;
        string s;
        inp >> s;
        cval.set_str(s, 10);
        sum_prod(result,At[i], cval);

        assert(cval >= 0); // db >= 0
        fc += cval * c[i];
    }

    assert(fc > 0); // cT db > 0
    for (mpq_class &i : result)
        assert(i <= 0); // Adb <= 0
}

void otim(ifstream &inp) {
    mpq_class optv, optvcopy;
    string s;
    inp >> s;
    optv.set_str(s, 10);
    optvcopy.set_str(s, 10);

    checksol(inp, optvcopy, true);

    vector<mpq_class> result(m, 0);
    mpq_class tot = 0;
    for (int i = 0; i < n; i++) {
        mpq_class cval;
        inp >> s;
        cval.set_str(s, 10);
        sum_prod(result, A[i], cval);
        tot += cval * b[i];

        assert(cval >= 0); // yt >= 0
    }

    for (int i = 0; i < m; i++)
        assert(result[i] >= c[i]);//ytA >= ct

    assert(tot == optv);// ytb == ctz
}


int main(int argc, char **argv)
{
    ifstream in(argv[1]); // Input

    in >> n >> m;
    c.resize(m);
    b.resize(n);
    A.resize(n, vector<mpq_class>(m + n, 0));
    At.resize(m + n, vector<mpq_class>(n));

    for (int i = 0; i < m; i++)
    {
        string s;
        in >> s;
        c[i].set_str(s, 10);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m+1; j++)
            if (j == m)
            {
                string s;
                in >> s;
                b[i].set_str(s, 10);
            } else {
                string s;
                in >> s;
                A[i][j].set_str(s, 10);
                At[j][i].set_str(s,10);
            }

    for (int i = 0; i < n; i ++)
    {
        A[i][m+i] = 1;
    }


    in.close();

    ifstream out(argv[2]); // output

    string type;
    out >> type;

    if (type == "inviavel")
        invi(out);
    else if (type == "ilimitada")
        ilim(out);
    else
    {
        assert(type == "otima");
        otim(out);
    }

    out.close();
    return 0;
}
