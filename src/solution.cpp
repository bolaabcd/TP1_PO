#include "solution.hpp"
#include "tableau.hpp"
#include <vector>
#include <gmp.h>
#include <assert.h>

Solution::Solution(Tableau &t)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);
    assert(this->cert.size() == 0);

    int n = t.get_n();
    int m = t.get_m()-n;// this m is the auxiliar number of columns
    this->sol.resize(m+n);
    for(int i = 0; i < m; i++)
        mpq_init(this->sol[i]);
    for(int i = 0; i < n; i++)
        mpq_set(this->sol[m+i],t.tab[m+1][i+1]);
    this->basis.resize(n);
    for(int i = 0; i < n; i++)
        this->basis[i] = m+i;
    
    this->solve(t);
}

Solution::Solution(Tableau &t, Solution &aux_sol)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);
    assert(this->cert.size() == 0);

    int n = t.get_n(), m = t.get_m();
    this->sol = aux_sol.sol;
    this->basis = aux_sol.basis;

    this->solve(t);
}

bool Solution::is_zero()
{
}

void Solution::print_yt(std::ostream &out)
{
}

std::ostream &operator<<(std::ostream &os, Solution &s)
{
}

void Solution::solve(Tableau &t)
{
}

Solution::~Solution(){
    this->sol.clear();
    this->basis.clear();
}