/** @file */
#include <iostream>

#include "grlib/adj_list.hpp"

int main()
{
        constexpr int size = 3;
        grlib::adj_list<grlib::Basic_edge> graph((size_t)size);

        for (int i = 0; i < size; ++i)
                for (int j = 0; j < size; ++j) {
                        graph.insert_edge(i, grlib::Basic_edge(j, -1));
                        graph.insert_edge(j, grlib::Basic_edge(i, -1));
                }

        grlib::print_adj_list(graph);
        return 0;
}

