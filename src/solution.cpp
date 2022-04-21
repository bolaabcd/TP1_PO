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
}

Solution::~Solution()
{
    this->sol.clear();
    this->basis.clear();
}