#ifndef TABLEAU_CLASS_4_LINEAR_PROGRAMMING
#define TABLEAU_CLASS_4_LINEAR_PROGRAMMING

#include <vector>
#include <gmpxx.h>
#include <iostream>
// #include "solution.hpp"

class Tableau
{
    public:
        Tableau(int n, int m);
        void read(std::istream &in);
        void get_auxiliar(Tableau &aux);
        int get_n();
        int get_m();
        void makeone(int lin, int col, std::vector<mpq_class> &viab_cert);
        ~Tableau();
        friend class Solution;

    private:
        int n;
        int m;
        void print_tab();// Debugging purposes
        void div(std::vector<mpq_class> &first, mpq_class val);
        void sub(std::vector<mpq_class> &first, std::vector<mpq_class> &second, mpq_class val);
        std::vector<std::vector<mpq_class>> tab;
};

#endif