#ifndef SOLUTION_CLASS_4_LINEAR_PROGRAMMING
#define SOLUTION_CLASS_4_LINEAR_PROGRAMMING

#include <gmpxx.h>
#include <iostream>
#include <vector>
#include "tableau.hpp"

class Solution
{
    public:
        Solution(Tableau &t);
        Solution(Tableau &t, Solution &aux_sol);
        bool is_zero();
        void print_inv_cert(std::ostream &out);
        friend std::ostream &operator<<(std::ostream &out, Solution &s);
        ~Solution();

    private:
        bool infinite;
        mpq_class solval;
        void solve(Tableau &t);
        void canon(Tableau &t);
        void ilim(int negvar, Tableau &t);
        void optim();
        std::vector<mpq_class> sol;
        std::vector<int> inv_cert;
        std::vector<mpq_class> viab_cert;
        std::vector<int> basis;
};

#endif