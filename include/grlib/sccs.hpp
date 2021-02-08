/** @file */
#pragma once

#include "grlib/adj_list.hpp"
#include "grlib/dfs.hpp"
#include "grlib/grlib.hpp"

#include <functional>
#include <numeric>
#include <stack>

namespace grlib {

template<typename edge>
struct sccs_context {
        sccs_context() = delete;
        sccs_context(grlib::adj_list<edge>& alist)
        :alist(&alist),
         components_number(0),
         low(alist.vertices_capacity()),
         scc(alist.vertices_capacity(), -1)
        {
                std::iota(low.begin(), low.end(), 0);
        }

        grlib::adj_list<edge>* alist;
        int components_number;

        std::vector<int> low;
        std::vector<int> scc;
};

template<typename T>
Edge_type edge_classification(int x, int y, dfs_context<T>& dfs)
{
        if (dfs.parent(y) == x)
                return Edge_type::tree;
        if (dfs.discovered(y) && !dfs.processed(y))
                return Edge_type::back;
        if (dfs.processed(y) && (dfs.entry_time(y) > dfs.entry_time(x)))
                return Edge_type::forward;
        if (dfs.processed(y) && (dfs.entry_time(y) < dfs.entry_time(x)))
                return Edge_type::cross;

        throw std::runtime_error("Cannot classify edge");
}

/**
 * Tarjan's strongly connected components algorithm
 * @param sccs: context that algorithm will process
 */
template<typename T>
void sccs(sccs_context<T>& sccs)
{
        grlib::adj_list<T>& alist = *sccs.alist;
        using dfs_cxt = dfs_context<T>&;

        std::stack<int> active;

        auto process_vertex_early = [&] (int v, [[maybe_unused]] dfs_cxt& dfs) {
                active.push(v);
        };

        auto process_edge = [&] (int x, int y, dfs_cxt& dfs) {
                Edge_type edge_class = edge_classification(x, y, dfs);

                if (edge_class == Edge_type::back)
                        if (dfs.entry_time(y) < dfs.entry_time(sccs.low[x]))
                                sccs.low[x] = y;

                // if cross edge && component not yet assigned
                if (edge_class == Edge_type::cross)
                        if (sccs.scc[y] == -1)
                                if (dfs.entry_time(y) < dfs.entry_time(sccs.low[x]))
                                        sccs.low[x] = y;

        };

        auto process_vertex_late = [&] (int v, dfs_cxt& dfs) {
                // edge (parent[v],v) cuts off scc
                if (sccs.low[v] == v) {
                        int t;
                        sccs.components_number++;
                        sccs.scc[v] = sccs.components_number;

                        while ((t = active.top()) != v) {
                                sccs.scc[t] = sccs.components_number;
                                active.pop();
                        }

                        active.pop();
                }

                if (dfs.entry_time(sccs.low[v]) < dfs.entry_time(sccs.low[dfs.parent(v)]))
                        sccs.low[dfs.parent(v)] = sccs.low[v];
        };

        grlib::dfs_context<T> dfs_context(*sccs.alist, 0,
                process_vertex_early, process_edge, process_vertex_late);

        for (size_t i = 0; i < alist.edges.size(); i++)
                if (!dfs_context.discovered(i) and !alist.edges[i].empty()) {
                        dfs_context.start = i;
                        grlib::dfs(dfs_context);
                }
}

}; // namespace grlib

