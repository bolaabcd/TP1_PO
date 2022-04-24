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

    this->solve(t);
}

Solution::Solution(Tableau &t, Solution &aux_sol, bool as_rational2) : as_rational(as_rational2)
{
    assert(this->sol.size() == 0);
    assert(this->basis.size() == 0);

    // std::cout << "POSTAUX" << std::endl;
    // std::cout << aux_sol.solval.get_str() << std::endl;
    // for (int i = 0; i < t.n + t.m; i++)
    // {
    //     std::cout << aux_sol.sol[i].get_str() << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "basis_post:" << std::endl;
    // for (int i = 0; i < t.n; i++)
    // {
    //     std::cout << aux_sol.basis[i] << " ";
    // }
    // std::cout << std::endl;
    // t.print_tab();

    assert(aux_sol.sol.size() == t.m + t.n);
    for (int i = 0; i < t.n; i++)
    {
        // std::cout << aux_sol.sol[t.m + i].get_str() << std::endl;
        assert(aux_sol.sol[t.m + i] == 0);
    }

    assert(aux_sol.basis.size() == t.n);
    for (int i = 0; i < t.n; i++)
    {
        assert(aux_sol.basis[i] >= 0);
        assert(aux_sol.basis[i] < t.m);
    }

    this->sol = aux_sol.sol;
    this->basis = aux_sol.basis;

    this->sol.resize(t.m);

    this->cert.resize(t.n + 1, std::vector<mpq_class>(t.n, 0));
    for (int i = 1; i < t.n + 1; i++)
        this->cert[i][i - 1] = 1;

    this->solve(t);
}

bool Solution::is_zero()
{
    assert(this->sol.size() != 0);
    assert(this->basis.size() != 0);
    assert(this->cert.size() != 0);
    assert(this->cert[0].size() != 0);
    // std::cout << this->solval.get_str() << std::endl;
    return !mpq_sgn(this->solval.get_mpq_t());
}

void Solution::print_inv_cert(std::ostream &out)
{
    assert(this->cert[0].size() == this->basis.size());
    for (int i = 0; i < this->cert[0].size() - 1; i++)
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

    for (int i = 0; i < s.sol.size() - 1; i++)
        if (!s.as_rational)
            out << s.sol[i].get_d() << " ";
        else
            out << s.sol[i].get_str() << " ";

    if (!s.as_rational)
        out << s.sol[s.sol.size() - 1].get_d() << std::endl;
    else
        out << s.sol[s.sol.size() - 1].get_str() << std::endl;

    for (int i = 0; i < s.cert[0].size() - 1; i++)
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

void Solution::solve(Tableau &t)
{
    assert(t.m >= t.n);
    // t.print_tab();
    // std::cout << "SOL: " << std::endl;
    // for (int i = 0; i < this->sol.size(); i++)
    // {
    //     std::cout << this->sol[i].get_str() << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "BASIS: " << std::endl;
    // for (int i = 0; i < this->basis.size(); i++)
    // {
    //     std::cout << this->basis[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "CERTIFICATE: " << std::endl;
    // for (int i = 0; i < this->cert.size(); i++)
    // {
    //     for (int j = 0; j < this->cert[i].size(); j++)
    //         std::cout << this->cert[i][j].get_str() << " ";
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    for (int i = 0; i < t.n; i++)
        assert(t.tab[i][t.m] >= 0);

    // rewrite in canonical form
    this->canon(t);
    this->solval = t.tab[0][t.m];

    // std::cout << "POSTCANON:" << std::endl;
    // t.print_tab();

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
                    // std::cout << "DIV " << div.get_str() << " " << i << " " << j << std::endl;
                    if (div < maxi || maxii == -1)
                    { // if it's strictly bigger than the current maximum
                        maxi = div;
                        maxii = i - 1; // variable to remove from basis
                    }
                }
            }
            if (maxii != -1) // maxii is o r, k is o j, at the pseudocode we saw in class
            {
                assert(this->basis.size() == t.n);
                // now we remove the maxii'th identity column and add column j.
                for (int i = 0; i < t.n; i++)
                {
                    // assert(this->basis[i] >= 0);
                    // assert(this->basis[i] < t.m);
                    this->sol[this->basis[i]] -= maxi * t.tab[i + 1][j];
                    // assert(this->basis[i] != j);
                }
                this->sol[j] = maxi;
                assert(this->sol[this->basis[maxii]] == 0);
                // this->sol[this->basis[maxii]] = 0;
                this->basis[maxii] = j;
                this->solve(t);
                return;
            }

            // if didn't find positive a, it's ilimited
            this->ilim(j, t);
            return;
        }
    // if didn't find positive c, it's already optimal
    this->optim(t);
}

void Solution::canon(Tableau &t)
{
    assert(this->basis.size() == t.n);
    for (int i = 0; i < t.n; i++)
    {
        // make this line and column be one by gaussian elimination. cert is used to update
        // the optmality certification.
        t.makeone(i + 1, this->basis[i], this->cert);
        // this may cause problems. Think about it later, if it's impossible to do it in some case.
    }
}

void Solution::ilim(int negvar, Tableau &t)
{
    assert(negvar >= 0);
    assert(negvar < t.m);
    this->infinite = true;
    std::vector<mpq_class> infcert(t.m);
    // this->cert.clear();
    // this->cert.resize(t.m);

    for (int i = 0; i < t.m; i++)
        infcert[i] = 0;

    assert(infcert.size() == t.m);
    for (int i = 0; i < this->basis.size(); i++)
        infcert[this->basis[i]] = -t.tab[i + 1][negvar];

    assert(infcert[negvar] <= 0);
    infcert[negvar] = 1;
    this->cert[0] = infcert;
}

void Solution::optim(Tableau &t)
{
    this->infinite = false;
    this->solval = -this->solval;
    assert(this->cert.size());
    assert(this->cert[0].size());
    for (int i = 0; i < this->cert[0].size(); i++)
        this->cert[0][i] = -this->cert[0][i] * (1 - 2 * t.invs[i + 1]);
    // std::cout << "Cert: " << std::endl;
    // for(int i = 0; i < this->cert[0].size(); i++)
    //     std::cout << this->cert[0][i].get_str() << std::endl;
    // std::cout << "Rems: " << std::endl;
    // for(int i = 0; i < t.rems.size(); i++)
    //     std::cout << t.rems[i] << std::endl;

    this->cert[0].resize(this->cert[0].size() + t.rems.size());
    for (int i = 0; i < t.rems.size(); i++)
    {
        // rems values are "1-based"
        for (int j = t.rems[i]; j < this->cert[0].size(); j++)
            this->cert[0][j] = this->cert[0][j - 1];
        this->cert[0][t.rems[i] - 1] = 0;
    }
}

Solution::~Solution()
{
    this->sol.clear();
    this->basis.clear();
}