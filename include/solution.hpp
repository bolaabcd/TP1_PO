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
        void print_yt(std::ostream &out);
        friend std::ostream &operator<<(std::ostream &os, Solution &s);
    private:
        void solve(Tableau &t);
        std::vector<mpq_t> sol;
        std::vector<int> basis;
};

#endif