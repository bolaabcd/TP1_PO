#include "solution.hpp"
#include "tableau.hpp"
#include <vector>
#include <gmp.h>
#include <assert.h>
#include <iostream>

Solution::Solution(Tableau &t)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);
    assert(this->inv_cert.size() == 0);

    int n = t.get_n();
    int m = t.get_m() - n; // this m is the auxiliar number of columns
    this->sol.resize(m + n);
    for (int i = 0; i < m; i++)
        mpq_init(this->sol[i]);
    for (int i = 0; i < n; i++)
        mpq_set(this->sol[m + i], t.tab[m + 1][i + 1]);
    this->basis.resize(n);
    for (int i = 0; i < n; i++)
        this->basis[i] = m + i;

    this->solve(t);
}

Solution::Solution(Tableau &t, Solution &aux_sol)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);
    assert(this->inv_cert.size() == 0);

    int n = t.get_n(), m = t.get_m();
    this->sol = aux_sol.sol;
    this->basis = aux_sol.basis;

    this->solve(t);
}

bool Solution::is_zero()
{
    assert(this->sol.size() != 0);
    assert(this->basis.size() != 0);
    assert(this->inv_cert.size() != 0);

    return mpq_cmp_si(this->solval, 0, 1);
}

void Solution::print_inv_cert(std::ostream &out)
{
    assert(this->inv_cert.size() == this->basis.size());
    for (int i = 0; i < this->inv_cert.size() - 1; i++)
        out << this->inv_cert[i] << " ";
    out << this->inv_cert[this->inv_cert.size() - 1] << std::endl;
}

std::ostream &operator<<(std::ostream &out, Solution &s)
{
    if (s.infinite)
        out << "ilimitada" << std::endl;
    else
        out << "otima" << std::endl
            << mpq_get_d(s.solval) << std::endl;

    for (int i = 0; i < s.sol.size() - 1; i++)
        out << s.sol[i] << " ";
    out << s.sol[s.sol.size() - 1] << std::endl;

    for (int i = 0; i < s.viab_cert.size() - 1; i++)
        out << s.viab_cert[i] << " ";
    out << s.viab_cert[s.viab_cert.size() - 1] << std::endl;
}

void Solution::solve(Tableau &t)
{
    // rewrite in canonical form
    this->canon(t);

    for (int j = 0; j < t.m; j++)
        if (t.tab[0][j] > 0)
        {
            mpq_t maxi;
            int maxii = -1;
            mpq_set_si(maxi, -1, 1);
            for (int i = 1; i <= t.n; i++)
            {
                if (t.tab[i][j] > 0)
                {
                    // found positive a
                    mpq_t div;
                    mpq_div(div, t.tab[i][t.m + 1], t.tab[i][j]);
                    if (mpq_cmp(maxi, div) > 0)
                    { // if it's strictly smaller than the current maximum
                        mpq_set(maxi, div);
                        maxii = i;
                    }
                }
            }
            if (maxii != -1) // maxii is o r, k is o j, at the pseudocode we saw in class
            {
                // now we remove the maxii'th identity column and add column j.
                this->basis[maxii] = j;
                this->solve(t);
                return;
            }

            // if didn't find positive a, it's ilimited
            this->ilim();
            return;
        }
    // if didn't find positive c, it's already optimal
    this->optim();
}

void Solution::canon(Tableau &t)
{
    assert(this->basis.size() == t.m);
    for (int i = 0; i < this->basis.size(); i++)
    {
        // make this line and column be one by gaussian elimination
        // this may cause problems. Think about it later, if it's impossible to do it in some case.
        t.makeone(this->basis[i], i + 1);
    }
}

Solution::~Solution()
{
    this->sol.clear();
    this->basis.clear();
}