/** @file */
#pragma once

#include "grlib/adj_list.hpp"
#include "grlib/adj_matrix.hpp"

#include <algorithm>
#include <stack>
#include <functional>

namespace grlib {

template<typename T>
struct dfs_context {
        using vertex_callback = std::function<void(int, dfs_context<T>&)>;
        using edge_callback = std::function<void(int, int, dfs_context<T>&)>;

        dfs_context() = delete;

        /**
         * Initialize context
         * @param alist: adjacency list used for the algorithm
         * @param start: index of starting vertex
         * @param process_vertex_early: callback invoked when node is initially processed
         * @param process_edge: callback invoked when edge is processed
         * @param process_vertex_late: callback invoked when node is lately processed
         */
        dfs_context(grlib::adj_list<T>& alist, int start,
                vertex_callback&& process_vertex_early, edge_callback&& process_edge,
                vertex_callback&& process_vertex_late)
        :alist(&alist),
         start(start),
         finished(false),
         time(0),
         process_vertex_early(process_vertex_early),
         process_edge(process_edge),
         process_vertex_late(process_vertex_late),
         vs(alist.vertices_capacity(), dfs_vertex_state()) { }

        grlib::adj_list<T>* alist;
        int start; /// starting vertex index
        bool finished; /// optional flag allowing early exit
        int time; /// used for timing

        vertex_callback process_vertex_early;
        edge_callback process_edge;
        vertex_callback process_vertex_late;

        struct dfs_vertex_state {
                dfs_vertex_state()
                :discovered(false), processed(false), entry_time(0), exit_time(0), parent(-1) { }
                bool discovered; /// has vertex been discovered
                bool processed; /// has vertex been discovered
                int entry_time; /// entry timing of the vertex
                int exit_time; /// exit timing of the vertex
                int parent; /// parent of the vertex
        };

        bool& discovered(int v) { return vs[v].discovered; }
        bool& processed(int v) { return vs[v].processed; }
        int& entry_time(int v) { return vs[v].entry_time; }
        int& exit_time(int v) { return vs[v].exit_time; }
        int& parent(int v) { return vs[v].parent; }
        std::vector<dfs_vertex_state> vs; /// vector of state of each vertex
};

template<typename T>
void dfs(dfs_context<T>& dfs)
{
        dfs_impl(dfs.start, dfs);
}

/**
 * Depth-first search implementation based on Steven S. Skiena "The algorithm design manual".
 * @param x: starting vertex
 * @param dfs_context: context that algorithm will process
 */
template<typename T>
void dfs_impl(int x, dfs_context<T>& dfs)
{
        grlib::adj_list<T>& alist = *dfs.alist;

        if (dfs.finished)
                return;

        dfs.discovered(x) = true;
        dfs.time++;
        dfs.entry_time(x) = dfs.time;
        dfs.process_vertex_early(x, dfs);

        for (const auto& edge : alist.edges[x]) {
                int y = edge.y;

                if (!dfs.discovered(y)) {
                        dfs.parent(y) = x;
                        dfs.process_edge(x, y, dfs);
                        dfs_impl(y, dfs);
                } else if (!dfs.processed(y) or alist.directed) {
                        dfs.process_edge(x, y, dfs);
                }

                if (dfs.finished)
                        return;
        }

        dfs.processed(x) = true;
        dfs.time++;
        dfs.exit_time(x) = dfs.time;
        dfs.process_vertex_late(x, dfs);
}

}; // namespace grlib

