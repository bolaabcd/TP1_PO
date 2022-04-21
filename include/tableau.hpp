#ifndef TABLEAU_CLASS_4_LINEAR_PROGRAMMING
#define TABLEAU_CLASS_4_LINEAR_PROGRAMMING

#include <vector>
#include <gmp.h>
#include <iostream>

class Tableau
{
    public:
        Tableau(int n, int m);
        void read(std::istream &in);
        void get_auxiliar(Tableau &aux);
        int get_n();
        int get_m();
        ~Tableau();
    private:
        int n;
        int m;
        std::vector<std::vector<mpq_t>> tab;
};

#endif