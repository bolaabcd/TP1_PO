#include "tableau.hpp"

#include <assert.h>
#include <vector>
#include <gmpxx.h>

Tableau::Tableau(int nc, int mc) : n(nc), m(mc)
{
    assert(nc > 0);
    assert(mc > 0);
    assert(mc >= nc);
    this->tab.resize(nc + 1, std::vector<mpq_class>(mc + 1));
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
                this->tab[i][j] = 0;
                continue;
            }
            long int v;
            in >> v;
            this->tab[i][j] = v;
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

    int k = 0, mc = this->m, nc = this->n;
    for (int i = 0; i < nc + 1; i++)
    {
        for (int j = 0; j < mc; j++)
            aux.tab[i][j] = this->tab[i][j];
        if (i == 0)
        {
            for (int j = 0; j < mc; j++)
                aux.tab[i][j] = 0;
            for (int j = mc; j < mc + nc; j++)
                aux.tab[i][j] = -1;
            aux.tab[i][mc + nc] = 0;
        }
        else
        {
            for (int j = mc; j < mc + nc; j++)
                aux.tab[i][j] = 0;
            assert(k < nc);
            aux.tab[i][mc + k++] = 1;
            aux.tab[i][mc + nc] = this->tab[i][mc];
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

void Tableau::makeone(int lin, int col, std::vector<mpq_class> &viab_cert)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(viab_cert.size() == this->tab.size() - 1);
    assert(lin > 0);
    assert(col < this->m);

    // std::cout << lin << " " << col  << " BEFORE: "<< std::endl;
    // this->print_tab();

    mpq_class dval;
    dval = this->tab[0][col] / this->tab[lin][col];
    viab_cert[lin - 1] = viab_cert[lin - 1] - dval;

    assert(mpq_sgn(this->tab[lin][col].get_mpq_t()));
    this->div(this->tab[lin], this->tab[lin][col]); // first /= val

    for (int i = 0; i < this->n + 1; i++)
        if (i != lin && this->tab[i][col] != 0)
            this->sub(this->tab[i], this->tab[lin], this->tab[i][col]); // first -= second*val

    // this->print_tab();
}

void Tableau::div(std::vector<mpq_class> &first, mpq_class val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(first.size() == this->m + 1);
    for (int i = 0; i < this->m + 1; i++)
        first[i] = first[i] / val;
}

void Tableau::sub(std::vector<mpq_class> &first, std::vector<mpq_class> &second, mpq_class val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(first.size() == this->m + 1);
    assert(second.size() == this->m + 1);
    for (int i = 0; i < this->m + 1; i++)
    {
        // std::cout << i << "x" << first[this->m].get_str() <<"y" << second[this->m].get_str() << "z"
        // << val.get_str() << "ç";
        // std::cout << ((mpq_class)(first[this->m]-second[this->m]*val)).get_str() << " ";
        first[i] -= second[i] * val;
    }
    std::cout << std::endl;
}

void Tableau::print_tab()
{
    for (int i = 0; i < this->n + 1; i++)
    {
        for (int j = 0; j < this->m + 1; j++)
        {
            std::cout << this->tab[i][j].get_str() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Tableau::positive_b()
{
    for (int i = 0; i < this->n + 1; i++)
        if (this->tab[i][this->m] < 0)
            for (int j = 0; j < this->m + 1; j++)
                this->tab[i][j] = -this->tab[i][j];
}

Tableau::~Tableau()
{
    this->n = -1;
    this->m = -1;
    this->tab.clear();
}