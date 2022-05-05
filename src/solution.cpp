#include "solution.hpp"
#include "tableau.hpp"
#include <vector>
#include <gmpxx.h>
#include <assert.h>
#include <iostream>

Solution::Solution(Tableau &t, bool as_rational2) : as_rational(as_rational2)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);

    int n = t.get_n();
    int m = t.get_m() - n; // this m is the auxiliar number of columns
    assert(m > 0);
    this->sol.resize(m + n);
    for (int i = 0; i < m; i++)
        this->sol[i] = 0;

    for (int i = 0; i < n; i++)
    {
        this->sol[m + i] = t.tab[i + 1][n + m];
    }

    for (int i = 0; i < n; i++)
    {
        this->sol[m + i] = t.tab[i + 1][n + m];
    }
    this->basis.resize(n);
    std::vector<bool> valid(n, false);
    for (int i = 1; i < n + 1; i++)
        for (int j = 0; j < m; j++)
        {
            if (t.tab[i][j] != 0)
            {
                valid[i - 1] = true;
                break;
            }
        }
    for (int i = 0; i < n; i++)
        if (valid[i])
            this->basis[i] = m + i;
        else
            assert(t.tab[i + 1][m] == 0);

    this->cert.resize(n + 1, std::vector<mpq_class>(n, 0));
    for (int i = 1; i < n + 1; i++)
        this->cert[i][i - 1] = 1;

    this->solve(t, true);
}

Solution::Solution(Tableau &t, Tableau &auxt, Solution &aux_sol, bool as_rational2) : as_rational(as_rational2)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);

    assert((int)aux_sol.sol.size() == t.m + t.n);
    for (int i = 0; i < t.n; i++)
    {
        assert(aux_sol.sol[t.m + i] == 0);
    }


    assert((int)aux_sol.basis.size() == t.n);
    for (int i = 0; i < t.n; i++)
    {
        assert(aux_sol.basis[i] >= 0);
    }

    assert(t.n == auxt.n);
    assert(t.m + t.n == auxt.m);

    for (int i = 1; i < t.n + 1; i++) {
        t.tab[i][t.m] = auxt.tab[i][t.m + t.n];
    }
    for (int i = 1; i < t.n + 1; i++) {
        for (int j = 0; j < t.m; j++) {
            t.tab[i][j] = auxt.tab[i][j];
        }
    }
    t.tab[0][t.m] = 0;

    this->cert = aux_sol.cert;


    this->sol = aux_sol.sol;
    this->basis = aux_sol.basis;

    this->sol.resize(t.m);

    this->solve(t, false);
}

bool Solution::is_zero()
{
    assert(this->sol.size() != 0);
    assert(this->basis.size() != 0);
    assert(this->cert.size() != 0);
    assert(this->cert[0].size() != 0);
    return !mpq_sgn(this->solval.get_mpq_t());
}

void Solution::print_inv_cert(std::ostream &out)
{
    assert(this->cert[0].size() == this->basis.size());
    for (int i = 0; i < (int)this->cert[0].size() - 1; i++)
        if (!this->as_rational)
            out << this->cert[0][i].get_d() << " ";
        else
            out << this->cert[0][i].get_str() << " ";
    if (!this->as_rational)
        out << this->cert[0][this->cert[0].size() - 1].get_d() << std::endl;
    else
        out << this->cert[0][this->cert[0].size() - 1].get_str() << std::endl;
}

std::ostream &operator<<(std::ostream &out, Solution &s)
{
    if (s.infinite)
        out << "ilimitada" << std::endl;
    else
    {
        if (!s.as_rational)
            out << "otima" << std::endl
                << s.solval.get_d() << std::endl;
        else
            out << "otima" << std::endl
                << s.solval.get_str() << std::endl;
    }

    for (int i = 0; i < (int)s.sol.size() - 1; i++)
        if (!s.as_rational)
            out << s.sol[i].get_d() << " ";
        else
            out << s.sol[i].get_str() << " ";

    if (!s.as_rational)
        out << s.sol[s.sol.size() - 1].get_d() << std::endl;
    else
        out << s.sol[s.sol.size() - 1].get_str() << std::endl;

    for (int i = 0; i < (int)s.cert[0].size() - 1; i++)
        if (!s.as_rational)
            out << s.cert[0][i].get_d() << " ";
        else
            out << s.cert[0][i].get_str() << " ";

    if (!s.as_rational)
        out << s.cert[0][s.cert[0].size() - 1].get_d() << std::endl;
    else
        out << s.cert[0][s.cert[0].size() - 1].get_str() << std::endl;

    return out;
}

void Solution::solve(Tableau &t, bool is_aux)
{
    assert(t.m >= t.n);

    for (int i = 1; i < t.n; i++) // checking if b is positive
        assert(t.tab[i][t.m] >= 0);

    // rewrite in canonical form
    this->canon(t);
    this->solval = t.tab[0][t.m];

    for (int i = 1; i < t.n; i++)
        assert(t.tab[i][t.m] >= 0);

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
                    div = t.tab[i][t.m] / t.tab[i][j];
                    if (div < maxi || maxii == -1)
                    { // if it's strictly bigger than the current maximum
                        maxi = div;
                        maxii = i - 1; // variable to remove from basis
                    }
                }
            }
            if (maxii != -1) // maxii is r, k is j, at the pseudocode we saw in class
            {
                assert((int)this->basis.size() == t.n);
                // now we remove the maxii'th identity column and add column j.
                for (int i = 0; i < t.n; i++)
                {
                    this->sol[this->basis[i]] -= maxi * t.tab[i + 1][j];
                }
                this->sol[j] = maxi;
                assert(this->sol[this->basis[maxii]] == 0);
                this->basis[maxii] = j;
                this->solve(t, is_aux);
                return;
            }

            // if didn't find positive a, it's ilimited
            this->ilim(j, t);
            return;
        }
    // if didn't find positive c, it's already optimal
    this->optim(t, is_aux);
}

void Solution::canon(Tableau &t)
{
    assert((int)this->basis.size() == t.n);
    std::vector<bool> marc(t.n,false);
    bool entrou = true;
    while(entrou) {
        entrou = false;
        for (int i = 0; i < t.n; i++)
        {
            if(!marc[i] && t.tab[i+1][this->basis[i]] != 0){
                marc[i] = true;
                entrou = true;
                // make this line and column be one by gaussian elimination. cert is used to update
                // the optmality certification.
                t.makeone(i + 1, this->basis[i], this->cert);
                // this may cause problems. Think about it later, if it's impossible to do it in some case.
            }
        }
    }
    for(bool b: marc)
        assert(b);
}

void Solution::ilim(int negvar, Tableau &t)
{
    assert(negvar >= 0);
    assert(negvar < t.m);
    this->infinite = true;
    std::vector<mpq_class> infcert(t.m);

    for (int i = 0; i < t.m; i++)
        infcert[i] = 0;

    assert((int)infcert.size() == t.m);
    for (int i = 0; i < (int)this->basis.size(); i++)
        infcert[this->basis[i]] = -t.tab[i + 1][negvar];

    assert(infcert[negvar] <= 0);
    infcert[negvar] = 1;
    this->cert[0] = infcert;
    this->cert[0].resize(t.m - t.n);
    this->sol.resize(t.m - t.n);
}

void Solution::optim(Tableau &t, bool is_aux)
{
    if (is_aux && this->solval == 0) {
        assert((int)this->basis.size() == t.n);
        bool entrou = true;
        while (entrou){
            entrou = false;
            for (int i = 0; i < t.n; i++) {
                if (this->basis[i] >= t.m - t.n) {
                    entrou = true;

                    int maxjj = -1 ;
                    mpq_class maxj;

                    for (int j = 0; j < t.m - t.n; j++) {
                        if(this->sol[j] != 0 || t.tab[i+1][j] == 0)// t.tab[0][j] == 0 || 
                            continue;
                        assert(t.tab[0][j] < 0);
                        mpq_class div = t.tab[0][j] / t.tab[i+1][j];
                        if (maxjj == -1 || div > maxj) {
                            maxjj = j;
                            maxj = div;
                        }
                    }

                    assert(maxjj != -1);
                    // now we remove this->basis[i] from the basis and add maxjj
                    assert(t.tab[i+1].back() == 0);

                    this->basis[i] = maxjj;

                    t.makeone(i+1,maxjj,this->cert);
                }
            }
        }
    }

    this->infinite = false;
    this->solval = -this->solval;
    assert(this->cert.size());
    assert(this->cert[0].size());
    for (int i = 0; i < (int)this->cert[0].size(); i++)
        this->cert[0][i] = -this->cert[0][i] * (1 - 2 * t.invs[i + 1]);

    this->cert[0].resize(this->cert[0].size() + t.rems.size());
    for (int i = 0; i < (int)t.rems.size(); i++)
    {
        // rems values are "1-based"
        for (int j = t.rems[i]; j < (int)this->cert[0].size(); j++)
            this->cert[0][j] = this->cert[0][j - 1];
        this->cert[0][t.rems[i] - 1] = 0;
    }

    if(!is_aux)
        this->sol.resize(t.m - t.n);
}

Solution::~Solution()
{
    this->sol.clear();
    this->basis.clear();
}