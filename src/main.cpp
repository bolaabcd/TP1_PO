#include <iostream>
#include <vector>
#include "solution.hpp"
#include "tableau.hpp"

// REMOVE ALL ASSERTS AFTER EVERYTHING IS CORRECT!!!!

int main()
{
    std::cout.precision(4);
    std::cout << std::fixed;
    int n, m;
    std::cin >> n >> m;
    Tableau tableau(n, m), aux(n, m + n);
    tableau.read(std::cin);

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