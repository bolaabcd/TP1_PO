#include "tableau.hpp"

#include <assert.h>
#include <vector>
#include <gmpxx.h>
#include <iostream>

Tableau::Tableau(int nc, int mc) : n(nc), m(mc)
{
    assert(nc > 0);
    assert(mc > 0);
    // assert(mc >= nc);
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
    assert(this->invs.size() == 0);
    this->invs.resize(this->n + 1, false);
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
            // bool ok = false;
            // for (int j = 0; j < mc; j++)
            //     if (this->tab[i][j] != 0)
            //     {
            //         ok = true;
            //         break;
            //     }
            for (int j = mc; j < mc + nc; j++)
                aux.tab[i][j] = 0;
            assert(k < nc);
            // if (ok)
            aux.tab[i][mc + k++] = 1;
            aux.tab[i][mc + nc] = this->tab[i][mc];
        }
    }
    aux.rems = this->rems;
    aux.invs = this->invs;
}

int Tableau::get_n()
{
    return this->n;
}

int Tableau::get_m()
{
    return this->m;
}

void Tableau::makeone(int lin, int col, std::vector<std::vector<mpq_class>> &viab_cert)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(viab_cert.size() == this->tab.size());
    assert(viab_cert[0].size() == this->tab.size() - 1);
    assert(lin > 0);
    assert(col < this->m);
    assert(this->tab[lin][col] != 0);
    assert(!this->invs[0]);

    // std::cout << lin << " " << col  << " BEFORE: "<< std::endl;
    // this->print_tab();

    // mpq_class dval;
    // dval = this->tab[0][col] / this->tab[lin][col];
    // viab_cert[lin - 1] = viab_cert[lin - 1] - dval;

    assert(mpq_sgn(this->tab[lin][col].get_mpq_t()));
    this->div(viab_cert[lin], this->tab[lin][col]);
    this->div(this->tab[lin], this->tab[lin][col]); // first /= val
    

    for (int i = 0; i < this->n + 1; i++)
        if (i != lin && this->tab[i][col] != 0)
        {
            this->sub(viab_cert[i], viab_cert[lin], this->tab[i][col]);
            this->sub(this->tab[i], this->tab[lin], this->tab[i][col]); // first -= second*val
        }

    // this->print_tab();
}

void Tableau::div(std::vector<mpq_class> &first, mpq_class val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    // assert(first.size() == this->m + 1);//this was before the certificate
    for (int i = 0; i < first.size(); i++)
        first[i] = first[i] / val;
}

void Tableau::sub(std::vector<mpq_class> &first, std::vector<mpq_class> &second, mpq_class val)
{
    assert(this->n > 0);
    assert(this->m > 0);
    assert(this->tab.size() == this->n + 1);
    assert(this->tab[0].size() == this->m + 1);
    assert(second.size() == first.size());
    // assert(first.size() == this->m + 1);
    for (int i = 0; i < first.size(); i++)
    {
        // std::cout << i << "x" << first[this->m].get_str() <<"y" << second[this->m].get_str() << "z"
        // << val.get_str() << "ç";
        // std::cout << ((mpq_class)(first[this->m]-second[this->m]*val)).get_str() << " ";
        first[i] -= second[i] * val;
    }
    // std::cout << std::endl;
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
        {
            this->invs[i] = true;
            for (int j = 0; j < this->m + 1; j++)
                this->tab[i][j] = -this->tab[i][j];
        }
}

bool Tableau::rem_extra(std::ostream &out, bool as_rational)
{
    std::vector<std::vector<mpq_class>> tmp(this->tab.size(), std::vector<mpq_class>(this->tab.size() - 1, 0));
    for (int i = 1; i < this->tab.size(); i++)
        tmp[i][i - 1] = 1;

    Tableau copy(*this);
    for (int i = 1; i < this->n + 1; i++)
    {
        bool remthis = true;
        for (int j = 0; j < this->m + 1; j++)
        {
            if (copy.tab[i][j] != 0)
            {
                if (j == this->m)
                {
                    out << "inviavel" << std::endl;
                    // tmp.clear();
                    // tmp.resize(this->tab.size(), std::vector<mpq_class>(this->tab.size() - 1, 0));
                    // for (int k = 1; k < this->tab.size(); k++)
                    //     tmp[k][k - 1] = 1;
                    // for (int kkk = 0; kkk < tmp.size(); kkk++)
                    // {
                    //     for (int jjj = 0; jjj < tmp[kkk].size(); jjj++)
                    //         std::cout << tmp[kkk][jjj].get_str() << " ";
                    //     std::cout << std::endl;
                    // }
                    // std::cout << std::endl;
                    // Tableau copy2(*this);
                    // for (int k = 0; k < this->m + 1; k++)
                    //     copy2.tab[0][k] = this->tab[i][k];
                    // for (int k = 1; k < this->n + 1; k++)
                    //     for (int r = 0; r < this->m + 1; r++)
                    //         if (k == i && r == j)
                    //         {
                    //             int posit = copy2.tab[k][r] > 0;
                    //             posit = posit * 2 - 1;
                    //             assert(tmp[0][i - 1] == 0);
                    //             // tmp[i - 1] = 1;
                    //             for (int k2 = 0; k2 < tmp.size() - 1; k2++)
                    //                 if (!as_rational)
                    //                     out << ((mpq_class)(tmp[0][k2] * posit)).get_d() << " ";
                    //                 else
                    //                     out << ((mpq_class)(tmp[0][k2] * posit)).get_str() << " ";
                    //             if (!as_rational)
                    //                 out << ((mpq_class)(tmp[0].back() * posit)).get_d() << std::endl;
                    //             else
                    //                 out << ((mpq_class)(tmp[0].back() * posit)).get_str() << std::endl;
                    //         }
                    //         else if (copy2.tab[k][r] != 0)
                    //         {
                    //             if (r == j)
                    //                 std::cout << i << " " << j << " " << k << " " << r << std::endl;
                    //             copy2.makeone(k, r, tmp);
                    //             break;
                    //         }

                    // tmp[i - 1] = 1;
                    int posit = copy.tab[i][j] < 0;
                    posit = posit * 2 - 1;
                    for (int k = 0; k < tmp[i].size() - 1; k++)
                        if(!as_rational)
                            out << ((mpq_class)(posit*tmp[i][k])).get_d() << " ";
                        else
                            out << ((mpq_class)(posit*tmp[i][k])).get_str() << " ";
                    if(!as_rational)
                        out << ((mpq_class)(posit*tmp[i].back())).get_d() << std::endl;
                    else
                        out << ((mpq_class)(posit*tmp[i].back())).get_str() << std::endl;
                    return false;
                }
                remthis = false;
                copy.makeone(i, j, tmp);
                break;
            }
        }
        if (remthis)
            this->rems.push_back(i);
    }
    if (this->rems.size() == 0)
        return true;
    std::vector<std::vector<mpq_class>> tab2(
        this->tab.size() - this->rems.size(),
        std::vector<mpq_class>(this->tab[0].size()));
    int newn = this->n - this->rems.size();
    int j = 0;
    for (int i = 1, ni = 1; i < this->n + 1; i++)
    {
        if (j < this->rems.size() && i == this->rems[j])
        {
            assert(this->rems[j] > 0);
            j++;
        }
        else
        {
            for (int k = 0; k < this->m + 1; k++)
                tab2[ni][k] = tab[i][k];
            ni++;
        }
    }
    assert(j == this->rems.size());
    for (int i = 0; i < this->m + 1; i++)
        tab2[0][i] = tab[0][i];
    this->n = newn;
    this->tab = tab2;
    return true;

    // int j = 0;
    // for (int i = 1; i < this->n + 1; i++)
    // {
    //     if (j < this->rems.size() && i == this->rems[j])
    //     {
    //         for (int k = 0; k < this->m + 1; k++)
    //             this->tab[i][k] = 0;
    //         j++;
    //     }
    //     else
    //     {
    //         for (int k = 0; k < this->m + 1; k++)
    //             this->tab[i][k] = tab[i][k];
    //     }
    // }
    // assert(j == this->rems.size());
    // for (int i = 0; i < this->m + 1; i++)
    //     this->tab[0][i] = tab[0][i];

    // return true;
}

Tableau::~Tableau()
{
    this->n = -1;
    this->m = -1;
    this->tab.clear();
}