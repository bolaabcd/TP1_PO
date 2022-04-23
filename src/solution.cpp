#include "solution.hpp"
#include "tableau.hpp"
#include <vector>
#include <gmpxx.h>
#include <assert.h>
#include <iostream>

Solution::Solution(Tableau &t)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);
    assert(this->inv_cert.size() == 0);
    assert(t.tab[0].size() == t.n+1);

    int n = t.get_n();
    int m = t.get_m() - n; // this m is the auxiliar number of columns
    this->sol.resize(m + n);
    for (int i = 0; i < m; i++)
        this->sol[i] = 0;
    for (int i = 0; i < n; i++)
        this->sol[m + i] = t.tab[m + 1][i + 1];

    for (int i = 0; i < n; i++)
        this->sol[m + i] = t.tab[m + 1][i + 1];
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

    this->sol = aux_sol.sol;
    this->basis = aux_sol.basis;

    this->solve(t);
}

bool Solution::is_zero()
{
    assert(this->sol.size() != 0);
    assert(this->basis.size() != 0);
    assert(this->inv_cert.size() != 0);

    return !mpq_sgn(this->solval.get_mpq_t());
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
            << s.solval.get_d() << std::endl;

    for (int i = 0; i < s.sol.size() - 1; i++)
        out << s.sol[i].get_d() << " ";
    out << s.sol[s.sol.size() - 1].get_d() << std::endl;

    for (int i = 0; i < s.viab_cert.size() - 1; i++)
        out << s.viab_cert[i].get_d() << " ";
    out << s.viab_cert[s.viab_cert.size() - 1].get_d() << std::endl;
    return out;
}

void Solution::solve(Tableau &t)
{
    // rewrite in canonical form
    this->canon(t);

    for (int j = 0; j < t.m; j++)
        if (t.tab[0][j] > 0)
        {
            mpq_class maxi;
            int maxii = -1;
            maxi = -1;
            for (int i = 1; i <= t.n; i++)
            {
                if (t.tab[i][j] > 0)
                {
                    // found positive a
                    mpq_class div;
                    div = t.tab[i][t.m + 1] / t.tab[i][j];
                    if (div > maxi)
                    { // if it's strictly bigger than the current maximum
                        maxi = div;
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
            this->ilim(j, t);
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
        // make this line and column be one by gaussian elimination. viab_cert is used to update
        // the optmality certification.
        t.makeone(this->basis[i], i + 1, this->viab_cert);
        // this may cause problems. Think about it later, if it's impossible to do it in some case.
    }
}

void Solution::ilim(int negvar, Tableau &t)
{
    this->infinite = true;
    this->viab_cert.clear();
    this->viab_cert.resize(t.m);

    for (int i = 0; i < t.m; i++)
        this->viab_cert[i] = 0;

    assert(this->basis.size() == this->sol.size());
    for (int i = 0; i < this->basis.size(); i++)
        this->viab_cert[this->basis[i]] = this->sol[i];

    assert(mpq_cmp_si(this->viab_cert[negvar].get_mpq_t(), 0, 1));
    this->viab_cert[negvar] = 1;
}

void Solution::optim()
{
    this->infinite = false;
    this->solval = -this->solval;
    assert(this->viab_cert.size());
    for (int i = 0; i < this->viab_cert.size(); i++)
        this->viab_cert[i] = -this->viab_cert[i];
}

Solution::~Solution()
{
    this->sol.clear();
    this->basis.clear();
}