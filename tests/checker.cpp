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
vector<vector<mpq_class>> A;

int n, m;


void invi(ifstream &inp) {
    vector<mpq_class> yta(m, 0);
    mpq_class fb = 0;

    for (int i = 0; i < n; i++)
    {
        mpq_class cval;
        string s;
        inp >> s;
        cval.set_str(s, 10);
        //sum_prod(yta,A[i],cval);
        fb += cval * b[i];
    }

    assert(fb < 0);

}

void ilim(ifstream &inp) {

}

void otim(ifstream &inp) {

}


int main(int argc, char **argv)
{
    ifstream in(argv[1]); // Input

    in >> n >> m;
    c.resize(m);
    b.resize(n);
    A.resize(n, vector<mpq_class>(m + n, 0));

    for (int i = 0; i < m; i++)
    {
        string s;
        in >> s;
        c[i].set_str(s, 10);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m+1; j++)
            if (j == m + 1)
            {
                string s;
                in >> s;
                b[i].set_str(s, 10);
            } else {
                string s;
                in >> s;
                A[i][j].set_str(s, 10);
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
