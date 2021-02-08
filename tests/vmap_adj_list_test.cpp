/** @file */
#include <iostream>

#include <grlib/graph_alist.hpp>
#include <grlib/utility.hpp>

#include "wrapper.hpp"

struct Edge {
        Edge() = default;
        Edge(int y, int weight)
        : y{y}, weight{weight} { }

        int y;
        int weight;
};

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

        graph_alist<Edge> graph;
        Agnode_t *node;
        int i = 0;

        graph_for_each_node(node, cgraph.data())
                graph.vmap.add(i++, agnameof(node));

        print_vmap(graph.vmap);

        return 0;
}

