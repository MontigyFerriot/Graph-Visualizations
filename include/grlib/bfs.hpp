/** @file */
#pragma once

#include "grlib/adj_list.hpp"
#include "grlib/adj_matrix.hpp"

#include <queue>
#include <functional>

namespace grlib {

template<typename T>
struct bfs_context {
        using vertex_callback = std::function<void(int, bfs_context<T>&)>;
        using edge_callback = std::function<void(int, int, bfs_context<T>&)>;
        bfs_context() = delete;

        /**
         * Initialize context
         * @param alist: adjacency list used for the algorithm
         * @param start: index of starting vertex
         * @param process_vertex_early: callback invoked when node is initially processed
         * @param process_edge: callback invoked when edge is processed
         * @param process_vertex_late: callback invoked when node is lately processed
         */
        bfs_context(grlib::adj_list<T>& alist, int start,
                vertex_callback&& process_vertex_early,
                edge_callback&& process_edge,
                vertex_callback&& process_vertex_late)
        :alist(&alist),
         start(start),
         time(0),
         process_vertex_early(process_vertex_early),
         process_edge(process_edge),
         process_vertex_late(process_vertex_late),
         vs(alist.vertices_capacity(), bfs_vertex_state()) { }

        grlib::adj_list<T>* alist;
        int start; /// starting vertex index
        int time; /// used for timing

        vertex_callback process_vertex_early;
        edge_callback process_edge;
        vertex_callback process_vertex_late;

        struct bfs_vertex_state {
                bfs_vertex_state()
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
        std::vector<bfs_vertex_state> vs; /// vector of state of each vertex
};

/**
 * Breadth first search implementation based on Steven S. Skiena "The algorithm design manual".
 * @param bfs_context: context that algorithm will process
 */
template<typename T>
void bfs(bfs_context<T>& bfs)
{
        grlib::adj_list<T>& alist = *bfs.alist;

        std::queue<int> q;
        int x;

        q.push(bfs.start);
        bfs.discovered(bfs.start) = true;

        while (!q.empty()) {
                x = q.front();
                q.pop();

                bfs.time++;
                bfs.entry_time(x) = bfs.time;
                bfs.process_vertex_early(x, bfs);
                bfs.processed(x) = true;

                for (const auto& edge : alist.edges[x]) {
                        int y = edge.y;

                        if (!bfs.processed(y) or alist.directed)
                                bfs.process_edge(x, y, bfs);

                        if (!bfs.discovered(y)) {
                                bfs.discovered(y) = true;
                                q.emplace(y);
                                bfs.parent(y) = x;
                        }

                }

                bfs.time++;
                bfs.exit_time(x) = bfs.time;
                bfs.process_vertex_late(x, bfs);
        }
}

}; // namespace grlib

