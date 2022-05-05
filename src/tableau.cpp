#include "tableau.hpp"

#include <vector>
#include <gmpxx.h>
#include <iostream>

Tableau::Tableau(int nc, int mc) : n(nc), m(mc)
{
    this->tab.resize(nc + 1, std::vector<mpq_class>(mc + 1));
}

void Tableau::read(std::istream &in)
{
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
    this->invs.resize(this->n + 1, false);
}

void Tableau::get_auxiliar(Tableau &aux, bool first)
{
    int k = 0, mc = this->m, nc = this->n;
    for (int i = 0; i < nc + 1; i++)
    {
        for (int j = 0; j < mc; j++)
            aux.tab[i][j] = this->tab[i][j];
        if (i == 0)
        {
            if(first){
                for (int j = 0; j < mc; j++)
                    aux.tab[i][j] = 0;
                for (int j = mc; j < mc + nc; j++)
                    aux.tab[i][j] = -1;
                aux.tab[i][mc + nc] = 0;
            }
        }
        else
        {
            for (int j = mc; j < mc + nc; j++)
                aux.tab[i][j] = 0;
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
    this->div(viab_cert[lin], this->tab[lin][col]);
    this->div(this->tab[lin], this->tab[lin][col]); // first /= val
    

    for (int i = 0; i < this->n + 1; i++)
        if (i != lin && this->tab[i][col] != 0)
        {
            this->sub(viab_cert[i], viab_cert[lin], this->tab[i][col]);
            this->sub(this->tab[i], this->tab[lin], this->tab[i][col]); // first -= second*val
        }
}

void Tableau::div(std::vector<mpq_class> &first, mpq_class val)
{
    for (int i = 0; i < (int)first.size(); i++)
        first[i] = first[i] / val;
}

void Tableau::sub(std::vector<mpq_class> &first, std::vector<mpq_class> &second, mpq_class val)
{
    for (int i = 0; i < (int)first.size(); i++)
    {
        first[i] -= second[i] * val;
    }
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
    for (int i = 1; i < (int)this->tab.size(); i++)
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
                    int posit = copy.tab[i][j] < 0;
                    posit = posit * 2 - 1;
                    for (int k = 0; k < (int)tmp[i].size() - 1; k++)
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
        if (j < (int)this->rems.size() && i == this->rems[j])
        {
            j++;
        }
        else
        {
            for (int k = 0; k < this->m + 1; k++)
                tab2[ni][k] = tab[i][k];
            ni++;
        }
    }
    for (int i = 0; i < this->m + 1; i++)
        tab2[0][i] = tab[0][i];
    this->n = newn;
    this->tab = tab2;
    return true;

}

Tableau::~Tableau()
{
    this->n = -1;
    this->m = -1;
    this->tab.clear();
}
