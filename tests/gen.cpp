#include <random>
#include <iostream>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

int rand(int a, int b)
{ // [a,b]
    return rand() % (b - a + 1) + a;
}

int main(int argc, char **argv)
{
    srand(atoi(argv[1]));
    int n = rand(1, 50), m = rand(1, 50);
    cout << n << ' ' << m << endl;

    for (int i = 0; i < m; i++)
        cout << rand(1, 10) << " ";
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cout << rand(1, 20) << " ";
        cout << rand(1, 10) << endl;
    }
}
