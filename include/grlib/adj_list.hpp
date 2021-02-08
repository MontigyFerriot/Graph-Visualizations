/** @file */
#pragma once

#include <iostream>
#include <vector>
#include <list>

#include "grlib/rep_base.hpp"

namespace grlib {

struct Basic_edge {
        Basic_edge() = default;
        Basic_edge(grlib::vertex_id y, int weight)
        : y(y), weight(weight) { }

        grlib::vertex_id y; /// index of the vertex
        int weight; /// weight of the edge
};

template<typename Edge>
struct adj_list : public Representation_base {
        static constexpr size_t default_capacity = CONFIG_ADJ_LIST_DEFAULT_CAP;

        adj_list();

        /**
         * Define graph attribute
         * @param size: number/range of preallocated nodes
         * @param directed: whether graph is directed
         */
        adj_list(size_t size, bool directed = false);

#ifdef GRLIB_SYNC_WITH_GRAPHVIZ
        /**
         * Initialize adjacency list using graph structure
         * @param cgraph: Graphviz graph
         */
        adj_list(gviz::cgraph& cgraph);
#endif
        /**
         * Insert x -> y edge into adjacency list
         * @param x: index of first vertex
         * @param edge: weight structure of the graph
         */
        void insert_edge(grlib::vertex_id x, Edge& edge);

        /**
         * Insert x -> y edge into adjacency list
         * @param x: index of first vertex
         * @param edge: weight structure of the graph
         */
        void insert_edge(grlib::vertex_id x, Edge&& edge);

        size_t vertices_capacity() const;

        std::vector<std::list<Edge>> edges; /// adjacency list - list of edges of each edge
};

template<typename Edge>
adj_list<Edge>::adj_list()
: Representation_base(default_capacity, false, 0UL),
  edges(default_capacity)
{
}

template<typename Edge>
adj_list<Edge>::adj_list(size_t size, bool directed)
: Representation_base(size, directed, 0UL),
  edges(size)
{
}

#ifdef GRLIB_SYNC_WITH_GRAPHVIZ

template<typename Edge>
adj_list<Edge>::adj_list(gviz::cgraph& cgraph)
: adj_list<Edge>(cgraph.nodes_number(), cgraph.is_directed())
{
        for (const auto& node : cgraph)
                vmap.push(node.name());

        grlib::vertex_id x, y;
        for (gviz::Node node : cgraph)
                for (gviz::Edge edge : node) {
                        x = vmap.index(edge.tail().name());
                        y = vmap.index(edge.head().name());

                        insert_edge(x, Edge(y, 0));

                        if (!cgraph.is_directed())
                                insert_edge(y, Edge(x, 0));
                }
}

#endif

template<typename Edge>
void adj_list<Edge>::insert_edge(grlib::vertex_id x, Edge& edge)
{
        if (x + 1 > (int)edges.size())
                edges.resize(2 * x);

        edges[x].push_back(edge);

        enumber++;
}

template<typename Edge>
void adj_list<Edge>::insert_edge(grlib::vertex_id x, Edge&& edge)
{
        if (x + 1 > (int)edges.size())
                edges.resize(2 * x);

        edges[x].emplace_back(edge);

        enumber++;
}

template<typename Edge>
size_t adj_list<Edge>::vertices_capacity() const
{
        return edges.size();
}

template<typename Edge>
void print_adj_list(adj_list<Edge>& list)
{
        std::cout << "-------------------\n";

        for (size_t i = 0; i < list.edges.size(); ++i) {
                if (list.edges[i].empty())
                        continue;

                std::cout << "\"" << list.vmap.names[i] << "\"" << ": ";

                for (const auto &edge : list.edges[i])
                        std::cout << "\"" << list.vmap.names[edge.y] << "\" -> ";

                std::cout << "$\n";
        }
        std::cout << "-------------------\n";
}

}; // namespace grlib

