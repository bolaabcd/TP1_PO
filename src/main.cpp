#include <iostream>
#include <vector>
#include <assert.h>
#include "solution.hpp"
#include "tableau.hpp"

// CHECK IF SOLUTION CANON MAKEONE WORKS: yes, if it's a basis it can be the identity
// REMOVE ALL ASSERTS AFTER EVERYTHING IS CORRECT!!!!

// TEST CYCLING EXAMPLES
// ADD EXACT ANSWER OPTION
// SEE IF ITS'S SLOW, AND IF SO MAKE SOLVE() NON-RECURSIVE

int main()
{
    std::cout.precision(4);
    std::cout << std::fixed;
    int n, m;
    std::cin >> n >> m;
    Tableau tableau(n, m);
    tableau.read(std::cin);
    if (!tableau.rem_extra(std::cout))
        return 0;
    tableau.positive_b();

    n = tableau.get_n();
    assert(tableau.get_m() == m);
    Tableau aux(n, m + n);

    tableau.get_auxiliar(aux);
    Solution aux_sol(aux);

    if (aux_sol.is_zero())
    {
        Solution final_sol(tableau, aux_sol);
        std::cout << final_sol << std::endl;
    }
    else // If auxiliar optimal is not zero, it's not possible to satisfy the requirements
    {
        std::cout << "inviavel" << std::endl;
        aux_sol.print_inv_cert(std::cout);
        std::cout << std::endl;
    }

    return 0;
}