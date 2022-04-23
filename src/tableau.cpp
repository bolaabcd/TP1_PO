#include "tableau.hpp"

#include <assert.h>

Tableau::Tableau(int n, int m) : n(n), m(m)
{
    assert(n > 0);
    assert(m > 0);
    this->tab.resize(n + 1, std::vector<mpq_t>(m + 1));
}

void Tableau::read(std::istream &in)
{
    assert(in.good());
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    for (int i = 0; i < this->n + 1; i++)
    {
        for (int j = 0; j < this->m + 1; j++)
        {
            if (i == 0 && j == this->m)
            {
                mpq_init(this->tab[i][j]);
                continue;
            }
            long int v;
            in >> v;
            mpq_set_si(this->tab[i][j], v, 1);
        }
    }
}

void Tableau::get_auxiliar(Tableau &aux)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(aux.n == this->n);
    assert(aux.m == this->n + this->m);
    assert(aux.tab.size() == aux.n + 1);
    assert(aux.tab[0].size() == aux.m + 1);

    int k = 0, m = this->m, n = this->m;
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < m; j++)
            mpq_set(aux.tab[i][j], this->tab[i][j]);
        if (i == 0)
        {
            for (int j = m; j < m + n; j++)
                mpq_set_si(aux.tab[i][j], -1, 1);
            mpq_init(aux.tab[i][m + n]);
        }
        else
        {
            for (int j = m; j < m + n; j++)
                mpq_init(aux.tab[i][j]);
            assert(k < n);
            mpq_set_si(aux.tab[i][m + k++], 1, 1);
            mpq_set(aux.tab[i][m + n], this->tab[i][m]);
        }
    }
}

int Tableau::get_n()
{
    return this->n;
}

int Tableau::get_m()
{
    return this->m;
}

void Tableau::makeone(int lin, int col, std::vector<mpq_t> &viab_cert)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(viab_cert.size() == this->tab.size());
    assert(lin > 0);
    assert(col < this->m);

    mpq_t dval;
    mpq_div(dval, this->tab[0][col], this->tab[lin][col]);
    mpq_neg(dval, dval);
    mpq_add(viab_cert[lin], viab_cert[lin], dval);

    assert(mpq_sgn(this->tab[lin][col]));
    this->div(this->tab[lin], this->tab[lin][col]); // first /= val

    for (int i = 0; i < this->n + 1; i++)
        this->sub(this->tab[i], this->tab[lin], this->tab[i][col]); // first -= second*val
}

void Tableau::div(std::vector<mpq_t> &first, mpq_t &val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(first.size() == this->m + 1);
    for (int i = 0; i < this->m + 1; i++)
        mpq_div(first[i], first[i], val);
}

void Tableau::sub(std::vector<mpq_t> &first, std::vector<mpq_t> &second, mpq_t &val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(first.size() == this->m + 1);
    assert(second.size() == this->m + 1);
    for (int i = 0; i < this->m + 1; i++)
    {
        mpq_t mul;
        mpq_mul(mul, second[i], val);
        mpq_sub(first[i], first[i], val);
    }
}

Tableau::~Tableau()
{
    this->n = -1;
    this->m = -1;
    this->tab.clear();
}