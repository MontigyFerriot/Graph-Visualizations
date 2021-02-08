/** @file */
#include <iostream>
#include <random>

#include "grlib/adj_matrix.hpp"

int randomize(int min,int max)
{
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min,max);

        return dis(gen);
}

struct Edge {
        int weight;
};

std::ostream& operator<<(std::ostream& ost, const Edge& e)
{
        ost << e.weight;
        return ost;
}

int main()
{
        grlib::adj_matrix<Edge> graph(5);

        Edge edge;
        edge.weight = -1;

        for (size_t i = 0; i < graph.edges.rows(); ++i)
                for (size_t j = 0; j < graph.edges.columns(); ++j) {
                        edge.weight = randomize(1, 9);
                        graph.edges[i][j] = edge;
                }

        print_matrix(graph.edges);
        return 0;
}

