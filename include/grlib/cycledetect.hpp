/** @file */
#pragma once

#include "grlib/adj_list.hpp"
#include "grlib/dfs.hpp"

#include <functional>

namespace grlib {

/**
 * Cycle detection algorithm
 * @param alist: adjacency list used for depth-first search
 * @param start: index of starting vertex
 * @param cycle_found: callback called when cycle is detected
 * @return throws exception when cycle is found
 */
template<typename T>
void cycles_detection(grlib::adj_list<T>& alist, int start,
                std::function<void(int, int, grlib::dfs_context<T>&)>& cycle_found)
{
        auto process_vertex_early = [] ([[maybe_unused]]int v,
                        [[maybe_unused]]grlib::dfs_context<T>&) {
        };

        auto process_edge = [&] (int x, int y, grlib::dfs_context<T>& dfs) {
                if (dfs.discovered(y) and !dfs.processed(y))
                        cycle_found(x, y, dfs);
        };

        auto process_vertex_late = [] ([[maybe_unused]] int v,
                        [[maybe_unused]]grlib::dfs_context<T>&) {
        };

        grlib::dfs_context<T> dfs_context{alist, start,
                process_vertex_early, process_edge, process_vertex_late};

        for (size_t i = 0; i < alist.edges.size(); i++)
                if (!dfs_context.discovered(i) and !alist.edges[i].empty()) {
                        dfs_context.start = i;
                        grlib::dfs(dfs_context);
        }
}

}; // namespace grlib

