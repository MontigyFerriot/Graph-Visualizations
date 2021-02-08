/** @file */
#include <algorithm>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include <grlib/adj_list.hpp>
#include <grlib/bfs.hpp>
#include <grlib/grlib.hpp>
#include <grlib/tpsort.hpp>
#include <grlib/utility.hpp>

#include "graphviz/wrapper.hpp"

namespace Option {
        std::string dpi = "100";
        std::string layout = "neato";
        std::string input_file = "";
        std::string log_file = "";
        std::string extension = "png";
        bool should_output = false;
        std::ofstream log_stream;

        std::string help =
R"(Generate breadth-first search visualization of provided graph.
Usage: bfs_vizu [OPTION]... [FILE]
Program options:
--dpi=<val>:                 dpi of generated files
[--extension -e]=<val>:      extension of generated files, png is default
--help -h:                   show help
--layout=<val>:              graph layout (neato default, see GraphViz documentation for possible layouts)
--log=<file>:                print program informations to the provided file.
--verbose -v:                print program informations to the standard input
[FILE]:                      file containing description of the graph, formats .txt and .dot are possible

Examples:
        $> bfs_vizu --log=out.log --layout=neato --dpi=300 graph.dot
        $> bfs_vizu -v --extension=jpg --layout=neato --dpi=300 graph.dot
)";
}

std::ostream& out()
{
        return Option::log_file != "" ? Option::log_stream : std::cout;
}

void process_args(int argc, char** argv)
{
        if (argc == 1) {
                std::cerr << Option::help;
                exit(0);
        }

        const char* const short_opts = "o:e:hv";

        const option long_opts[] = {
                {"dpi", required_argument, nullptr, 'd'},
                {"extension", required_argument, nullptr, 'e'},
                {"layout", required_argument, nullptr, 'l'},
                {"log", required_argument, nullptr, 'g'},
                {"help", no_argument, nullptr, 'h'},
                {"verbose", no_argument, nullptr, 'v'},
                {nullptr, no_argument, nullptr, 0}
        };

        while (true) {
                const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

                if (opt == -1)
                    break;

                switch (opt) {
                case 'd':
                        Option::dpi = optarg;
                break;
                case 'e':
                        Option::extension = optarg;
                break;
                case 'l':
                        Option::layout = optarg;
                break;
                case 'v':
                        Option::should_output = true;
                break;
                case 'g':
                        Option::log_file = optarg;
                        Option::should_output = true;
                break;
                case 'h':
                case '?':
                default:
                        std::cerr << Option::help;
                        exit(0);
                break;
                }
        }

        if (optind < argc) {
                Option::input_file = argv[optind];
        } else {
                std::cerr << "No input file_name provided.\n";
        }

        if (Option::log_file != "") {
                Option::log_stream = std::ofstream(Option::log_file);
                if (!Option::log_stream) {
                        std::cerr << "cannot open: " << Option::log_file << "\n";
                        exit(0);
                }
        }

        // check if dpi is a number
        for (char ch : Option::dpi)
                if (!std::isdigit(ch)) {
                        std::cerr << "dpi is not numeric value: " << Option::dpi << "\n";
                        exit(0);
                }

        std::vector<std::string> layouts = {"dot", "neato",
                "fdp", "sfdp", "twopi", "circo"};

        // validate layout
        if (!std::any_of(layouts.begin(), layouts.end(),
                        [&] (auto val) { return val == Option::layout; })) {
                std::cerr << "Invalid layout: " << Option::layout << "\n";
                exit(0);
        }
}

int main(int argc, char** argv)
{
        process_args(argc, argv);

        gviz::cgraph cgraph(Option::input_file);

        if (!cgraph) {
                out() << "Reading file: " << Option::input_file << " failed.\n";
                return 0;
        }

        if (!cgraph.def_graph_attr("dpi", Option::dpi)) {
                out() << "Setting dpi failed!\n";
                return 0;
        }

        gviz::graphviz_context context(cgraph, Option::layout);

        grlib::adj_list<grlib::Basic_edge> alist(cgraph);

        try {
                gviz::render(context, cgraph, "0", Option::extension);
        } catch (std::exception& e) {
                out() << e.what() << "\n";
        }

        int frame_number = 1;

        using bfs_context = grlib::bfs_context<grlib::Basic_edge>;

        auto process_vertex_early = [&] (int v, bfs_context& bfs) {
                if (Option::should_output) {
                        out() << "Early[" << bfs.entry_time(v) << "]: \""
                                << alist.vmap.names[v] << "\"\n";
                }

                gviz::Node node(cgraph.find_node(alist.vmap.names[v]));

                if (!node.set_attr("fillcolor", "#ff4000")) {
                        grlib_log("failed in set_attr_safe(%s, %s) on frame %d\n",
                                "fillcolor", "#D2691E", frame_number);
                        return;
                }

                try {
                        gviz::render(context, cgraph, std::to_string(frame_number++), Option::extension);
                } catch (std::exception& e) {
                        out() << e.what() << "\n";
                }
        };

        auto process_edge = [&] ([[maybe_unused]]int x, int y, [[maybe_unused]]bfs_context& bfs) {
                if (Option::should_output) {
                        out() << "Edge: \"" << alist.vmap.names[x] << "\" -> \""
                                << alist.vmap.names[y] << "\"\n";
                }

                gviz::Node node1 = cgraph.find_node(alist.vmap.names[x]);
                gviz::Node node2 = cgraph.find_node(alist.vmap.names[y]);

                gviz::Edge edge = cgraph.find_edge(node1, node2);

                if (!edge.set_attr("color", "red")) {
                        out() << "edge: failed in set_attr_safe(color, red) on frame "
                                << frame_number << "\n";

                        return;
                }

                if (!node2.set_attr_safe("fillcolor", "#0080ff", "blue")) {
                        out() << "failed in set_attr_safe(fillcolor, #D2691E) on frame "
                                                << frame_number << "\n";

                        return;
                }

                try {
                        gviz::render(context, cgraph, std::to_string(frame_number++), Option::extension);
                } catch (std::exception& e) {
                        out() << e.what() << "\n";
                }
        };

        auto process_vertex_late = [&] (int v, bfs_context& bfs) {
                if (Option::should_output) {
                        out() << "Late[" << bfs.exit_time(v) << "]: \"" << alist.vmap.names[v] << "\"\n";
                }
        };

        bfs_context bfs_cxt(alist, 0,
                        process_vertex_early, process_edge, process_vertex_late);

        grlib::bfs(bfs_cxt);

        return 0;
}

