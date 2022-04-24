#include <iostream>
#include <vector>
#include <assert.h>
#include "solution.hpp"
#include "tableau.hpp"

// CHECK IF SOLUTION CANON MAKEONE WORKS: yes, if it's a basis it can be the identity
// REMOVE ALL ASSERTS AFTER EVERYTHING IS CORRECT!!!!

// REVERT NEGATIVE B
// REMOVE EXTRA RESTRICTIONS
// TEST CYCLING EXAMPLES
// ADD EXACT ANSWER OPTION
// IF ITS'S SLOW, SOLVE() NON-RECURSIVE

int main()
{
    std::cout.precision(4);
    std::cout << std::fixed;
    int n, m;
    std::cin >> n >> m;
    Tableau tableau(n, m), aux(n, m + n);
    tableau.read(std::cin);
    // tableau.rem_extra();
    // tableau.positive_b();


    n = tableau.get_n();
    assert(tableau.get_m() == m);

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