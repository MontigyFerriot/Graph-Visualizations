/** @file */
#pragma once

#include "grlib/adj_list.hpp"
#include "grlib/dfs.hpp"

#include <stack>
#include <functional>

namespace grlib {

/**
 * Topological sorting algorithm
 * @param alist: adjacency list that will be processed by the algorithm
 * @param start: index of the starting vertex
 */
template<typename T>
std::vector<int> tpsort(grlib::adj_list<T>& alist, int start)
{
        std::vector<int> sorted;

        auto process_vertex_early = [&] ([[maybe_unused]]int v,
                        [[maybe_unused]]grlib::dfs_context<T>& c) {
        };

        auto process_edge = [&] ([[maybe_unused]]int x, int y, grlib::dfs_context<T>& dfs) {
                if (dfs.discovered(y) and !dfs.processed(y))
                        throw std::runtime_error("directed cycle found. Can't perform sccs() on not DAG graph.");
        };

        auto process_vertex_late = [&] (int v, [[maybe_unused]]grlib::dfs_context<T>&) {
                sorted.push_back(v);
        };

        grlib::dfs_context<T> dfs_context{alist, start,
                process_vertex_early, process_edge, process_vertex_late};

        for (size_t i = 0; i < alist.edges.size(); i++)
                if (!dfs_context.discovered(i) and !alist.edges[i].empty()) {
                        dfs_context.start = i;
                        grlib::dfs(dfs_context);
                }

        return sorted;
}

}; // namespace grlib

