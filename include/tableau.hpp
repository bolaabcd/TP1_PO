#ifndef TABLEAU_CLASS_4_LINEAR_PROGRAMMING
#define TABLEAU_CLASS_4_LINEAR_PROGRAMMING

#include <vector>
#include <gmp.h>
#include <iostream>
#include "solution.hpp"

class Tableau
{
    public:
        Tableau(int n, int m);
        void read(std::istream &in);
        void get_auxiliar(Tableau &aux);
        int get_n();
        int get_m();
        void makeone(int lin, int col, std::vector<mpq_t> &viab_cert);
        ~Tableau();
        friend Solution;

    private:
        int n;
        int m;
        void div(std::vector<mpq_t> &first, mpq_t &val);
        void sub(std::vector<mpq_t> &first, std::vector<mpq_t> &second, mpq_t &val);
        std::vector<std::vector<mpq_t>> tab;
};

#endif