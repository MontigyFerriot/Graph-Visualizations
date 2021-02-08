/** @file */
#include <iostream>

#include "grlib/adj_list.hpp"
#include "grlib/sccs.hpp"
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

        grlib::sccs_context<grlib::Basic_edge> sccs_cxt(alist);
        grlib::sccs(sccs_cxt);

        print_adj_list(alist);

        std::cout << "number of components: " << sccs_cxt.components_number << std::endl;

        for (size_t i = 0; i < alist.edges.size(); i++)
                 std::cout << "\"" << alist.vmap.names[i] << "\": " << sccs_cxt.scc[i] << "\n";
        return 0;
}

