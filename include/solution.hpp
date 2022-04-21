#ifndef SOLUTION_CLASS_4_LINEAR_PROGRAMMING
#define SOLUTION_CLASS_4_LINEAR_PROGRAMMING

#include "tableau.hpp"
#include <gmp.h>
#include <iostream>
#include <vector>

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
        mpq_t solval;
        void solve(Tableau &t);
        std::vector<mpq_t> sol;
        std::vector<int> inv_cert;
        std::vector<int> viab_cert;
        std::vector<int> basis;
};

#endif