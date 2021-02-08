/** @file */
#include <iostream>

#include "grlib/adj_list.hpp"
#include "grlib/tpsort.hpp"
#include "grlib/utility.hpp"
#include "graphviz/wrapper.hpp"

int main(int argc, char** argv)
{
        if (argc < 2) {
                std::cout << "No file provided\n";
                return 0;
        }

        gviz::cgraph cgraph{argv[1]};

        if (!cgraph) {
                std::cout << "Reading file:" << argv[1] << " failed.\n";
                return 0;
        }

        grlib::adj_list<grlib::Basic_edge> alist(cgraph);

        std::vector<int> sorted = grlib::tpsort(alist, 0);

        grlib::print_adj_list(alist);

        std::cout << "[";
        size_t end = sorted.size();

        for (size_t i = 0; i < end - 1; i++)
                std::cout << "\"" << alist.vmap.names[sorted[i]] << "\", ";

        std::cout << "\"" << alist.vmap.names[sorted[end - 1]] << "\"]\n";

        return 0;
}

