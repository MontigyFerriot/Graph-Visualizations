/** @file */
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>

#include <grlib/adj_list.hpp>
#include <grlib/utility.hpp>
#include <grlib/dfs.hpp>
#include <grlib/grlib.hpp>

#include "graphviz/wrapper.hpp"

int main(int argc, char** argv)
{
        if (argc < 2) {
                std::cout << "No file provided\n";
                return 0;
        }

        gviz::cgraph cgraph{argv[1]};

        if (!cgraph) {
                std::cout << "Reading file:" << argv[1] << " failed.\n";
                return 0;
        }

        if (!cgraph.set_attr("dpi", "300")) {
                std::cout << "Setting dpi failed!\n";
                return 0;
        }

        gviz::graphviz_context context;
        context.set_layout(cgraph, "neato");

        grlib::adj_list<grlib::Basic_edge> graph(cgraph);

        grlib::print_adj_list(graph);

        gviz::render(context, cgraph, "0", "png");

        using dfs_context = grlib::dfs_context<grlib::Basic_edge>;

        auto process_vertex_early = [&] (int v, [[maybe_unused]]dfs_context& cxt) {
                std::cout << "Early[" << cxt.vs[v].entry_time << "]: \"" << graph.vmap.names[v] << "\"\n";

        };

        auto process_edge = [&] ([[maybe_unused]]int x, int y, [[maybe_unused]]dfs_context& cxt) {
                std::cout << "Edge: \"" << graph.vmap.names[x] << "\" -> \""
                        << graph.vmap.names[y] << "\"\n";
        };

        auto process_vertex_late = [&] ([[maybe_unused]] int v, [[maybe_unused]]dfs_context& cxt) {
                std::cout << "Late[" << cxt.vs[v].exit_time << "]: \"" << graph.vmap.names[v] << "\"\n";
        };

        dfs_context cxt(graph, 0,
                process_vertex_early, process_edge, process_vertex_late);

        grlib::dfs(cxt);

        return 0;
}

