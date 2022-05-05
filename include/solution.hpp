#ifndef SOLUTION_CLASS_4_LINEAR_PROGRAMMING
#define SOLUTION_CLASS_4_LINEAR_PROGRAMMING

#include <gmpxx.h>
#include <iostream>
#include <vector>
#include "tableau.hpp"

class Solution
{
    public:
        Solution(Tableau &t, bool as_rational2);
        Solution(Tableau &t, Tableau &auxt, Solution &aux_sol, bool as_rational2);
        bool is_zero();
        void print_inv_cert(std::ostream &out);
        friend std::ostream &operator<<(std::ostream &out, Solution &s);
        ~Solution();

    private:
        bool infinite;
        bool as_rational;
        mpq_class solval;
        void solve(Tableau &t, bool is_aux);
        void canon(Tableau &t);
        void ilim(int negvar, Tableau &t);
        void optim(Tableau &t, bool is_aux);
        std::vector<mpq_class> sol;
        std::vector<std::vector<mpq_class>> cert;
        std::vector<int> basis;
};

#endif