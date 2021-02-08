/** @file */
#include <iostream>
#include <iomanip>
#include "grlib/matrix.hpp"

int main()
{
        matrix<int> mat{15, 15};

        for (size_t i = 0; i < mat.rows(); ++i)
                for (size_t j = 0; j < mat.columns(); ++j)
                        edge.weight = randomize(1, 9);

        print_matrix(mat);
        return 0;
}
