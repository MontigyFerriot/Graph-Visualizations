/** @file */
#pragma once

#include "grlib/rep_base.hpp"
#include "matrix.hpp"

namespace grlib {

template<typename Edge>
struct adj_matrix : public Representation_base {
        static constexpr size_t default_capacity = CONFIG_ADJ_MATRIX_DEFAULT_CAP;

        adj_matrix();

        /**
         * Initialize adjacency matrix
         * @param width: width/height of square matrix
         */
        adj_matrix(size_t width);

        /**
         * Insert x -> y edge into adjacency list
         * @param x: index of first vertex
         * @param y: index of second vertex
         * @param edge: weight structure of the graph
         */
        void insert_edge(unsigned x, unsigned y, const Edge& edge);

        matrix<Edge> edges;
};

template<typename Edge>
adj_matrix<Edge>::adj_matrix()
: Representation_base(default_capacity, false, 0UL),
  edges(default_capacity, default_capacity)
{
}

template<typename Edge>
adj_matrix<Edge>::adj_matrix(size_t width)
: Representation_base(default_capacity, false, 0UL),
  edges(width, width)
{
}

template<typename Edge>
void adj_matrix<Edge>::insert_edge(unsigned x, unsigned y, const Edge& edge)
{
        if (x >= edges.width() or y >= edges.width())
               return;

        edges[x][y] = edge;

        enumber++;
}

}; // namespace grlib

