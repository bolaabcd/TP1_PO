#include <iostream>
#include <vector>
#include <string.h>
#include "solution.hpp"
#include "tableau.hpp"

int main(int argc, char **argv)
{
    bool as_rational = false;
    if (argc == 2)
        if (!strcmp(argv[1], "print_as_rational"))
            as_rational = true;
    std::cout.precision(7);
    std::cout << std::fixed;
    int n, m;
    std::cin >> n >> m;
    Tableau first_tableau(n, m), tableau(n, m + n);
    first_tableau.read(std::cin);
    first_tableau.get_auxiliar(tableau, false);

    tableau.positive_b();

    n = tableau.get_n();
    m = tableau.get_m();
    Tableau aux(n, m + n);

    tableau.get_auxiliar(aux, true);
    Solution aux_sol(aux, as_rational);
    
    if (aux_sol.is_zero())
    {
        Solution final_sol(tableau, aux, aux_sol, as_rational);
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
