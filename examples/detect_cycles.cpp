/** @file */
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <map>
#include <string>

#include "grlib/adj_list.hpp"
#include "grlib/cycledetect.hpp"
#include "grlib/grlib.hpp"
#include "grlib/utility.hpp"

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
R"(Detect cycles in a graph using depth-first search algorithm.
Usage: detect_cycles [OPTION]... [FILE]
Program options:
--dpi=<val>:                 dpi of generated files
[--extension -e]=<val>:      extension of generated files, png is default
--help -h:                   show help
--layout=<val>:              graph layout (neato default, see GraphViz documentation for possible layouts)
--log=<file>:                print program informations to the provided file.
--verbose -v:                print program informations to the standard input
[FILE]:                      file containing description of the graph, formats .txt and .dot are possible

Examples:
        $> detect_cycles --log=out.log --layout=neato --dpi=300 graph.dot
        $> detect_cycles -v --extension=jpg --layout=neato --dpi=300 graph.dot
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

template<typename T, typename Func>
void find_path_base(int start, int end, grlib::dfs_context<T>& cxt, Func& func)
{
        if ((start == end) || (end == -1))
                func(start);
        else {
                find_path_base(start, cxt.vs[end].parent, cxt, func);
                func(end);
        }
}

template<typename T, typename Func>
void find_path(int start, int end, grlib::dfs_context<T>& cxt, Func& func)
{
        find_path_base(start, end, cxt, func);
}

int main(int argc, char** argv)
{
        process_args(argc, argv);

        gviz::cgraph cgraph(Option::input_file);

        if (!cgraph) {
                out() << "Reading file:" << Option::input_file << " failed.\n";
                return 0;
        }

        if (!cgraph.def_graph_attr("dpi", Option::dpi)) {
                out() << "Setting dpi: " << Option::dpi << " failed!\n";
                return 0;
        }

        gviz::graphviz_context context(cgraph, Option::layout);

        grlib::adj_list<grlib::Basic_edge> graph(cgraph);

        gviz::render(context, cgraph, "0", Option::extension);
        int frame_number = 1;

        using dfs_context = grlib::dfs_context<grlib::Basic_edge>;

        std::vector<std::pair<std::string, std::string>> vertices_in_path;

        std::function<void(int, int, dfs_context&)> cycle_detected =
        [&] (int x, int y,[[maybe_unused]] dfs_context& cxt) {
                auto p = [&] (int v) {
                        std::string vname = graph.vmap.names[v];
                        gviz::Node node(cgraph.find_node(vname));
                        std::string vcolor = node.get_attr("fillcolor");

                        vertices_in_path.emplace_back(std::make_pair(vname, vcolor));

                        if (!node.set_attr_safe("fillcolor", "#DC143C", "blue")) {
                                 grlib_log("failed in set_attr_safe(%s, %s, %s) on frame %d\n",
                                        "fillcolor", "#D2691E", "blue", frame_number);

                                return;
                        }
                };

                find_path(y, x, cxt, p);

                try {
                        gviz::render(context, cgraph, std::to_string(frame_number++), "png");
                } catch (std::exception& e) {
                        std::cout << e.what() << "\n";
                }

                for (const auto& pair : vertices_in_path) {
                        gviz::Node node = cgraph.find_node(pair.first);
                        node.set_attr_safe("fillcolor", pair.second, "blue");
                }
        };

        grlib::cycles_detection(graph, 0, cycle_detected);

        return 0;
}

