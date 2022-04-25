#include <iostream>
#include <fstream>
#include <vector>
#include <gmpxx.h>
#include <string>

using namespace std;

typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int main(int argc, char **argv)
{
    ifstream in(argv[1]); // Input

    vector<mpq_class> c, b;
    vector<vector<mpq_class>> A;

    int n, m;
    in >> n >> m;

    c.resize(m);
    b.resize(n);
    A.resize(n, vector<mpq_class>(m));

    for (int i = 0; i < m; i++)
    {
        string s;
        in >> s;
        c[i].set_str(s, 10);
    }
    for (int i = 0; i < m + 1; i++)
    {
    cout << "C" << endl;
        if (i == m + 1)
        {
            string s;
            in >> s;
            b[i].set_str(s, 10);
        }
        else
            for (int j = 0; j < n; j++)
            {
                string s;
                in >> s;
                A[i][j].set_str(s, 10);
            }
    }

    in.close();

    ifstream out(argv[2]); // output

    out.close();
}
