/** @file */
#include <iostream>

#include "grlib/graph_alist.hpp"
#include "grlib/utility.hpp"
#include "grlib/bfs.hpp"

struct Edge {
        Edge() = default;
        Edge(int y, int weight)
        : y{y}, weight{weight} { }

        int y;
        int weight;
};

int main()
{
        constexpr size_t size = 10;
        graph_alist<Edge> graph{size};

        if (read_graph("g1.txt", graph) < 0) {
                std::cout << "Error of reading a file!\n";
                return 0;
        }

        bfs_alist_context<Edge> context{graph, 0,
        [&](int v) {
                std::cout << "Vertex early: " << v << "\n";
                std::cout << "(btw.) size: " << size << "\n";
        },
        [](int x, int y) {
                printf("Edge (%d, %d)\n", x, y);
        },
        [](int v) {
                std::cout << "Vertex late: " << v << "\n";
        }};

        if (bfs(context) < 0) {
                std::cout << "dfs error.\n";
                return 0;
        }

        print_adj_list(graph);
        return 0;
}

