/** @file */
#include <iostream>

#include <grlib/graph_alist.hpp>
#include <grlib/utility.hpp>
#include <grlib/bfs.hpp>
#include <grlib/grlib.hpp>

#include "wrapper.hpp"

int main(int argc, char** argv)
{
        if (argc < 2) {
                std::cout << "No file provided\n";
                return 0;
        }

        int res;

        gviz::cgraph cgraph{argv[1]};

        if (!cgraph) {
                std::cout << "Reading file:" << argv[1] << " failed.\n";
                return 0;
        }

        grlib::Vertices_map vmap(cgraph.nodes_number());
        Agnode_t *node;
        int i = 0;

        graph_for_each_node(node, cgraph.data())
                vmap.add(i++, agnameof(node));

        print_vmap(vmap);

        return 0;
}

