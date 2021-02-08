/** @file */
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <map>
#include <string>

#include <iostream>

#include "grlib/adj_list.hpp"
#include "grlib/sccs.hpp"
#include "grlib/utility.hpp"
#include <grlib/grlib.hpp>

#include "graphviz/wrapper.hpp"

namespace Option {
        std::string dpi = "100";
        std::string layout = "neato";
        std::string input_file = "";
        std::string output_file = "";
        std::string extension = "";
        std::string log_file = "";
        bool should_output = false;
        std::ofstream log_stream;
        bool randomize = false;

        std::string help =
R"(Tarjan's strongly connected components algorithm visualization of provided graph.
Usage: sccs [OPTION]... [FILE]
Program options:
--dpi=<val>:                 dpi of generated files
--help -h:                   show help
--layout=<val>:              graph layout (neato default, see GraphViz documentation for possible layouts)
--log=<file>:                print program informations to the provided file.
--verbose -v:                print program informations to the standard input
--output -o:                 output file
--randomize -r:              randomize color of components
[FILE]:                      file containing description of the graph, formats .txt and .dot are possible

Examples:
        $> sccs --log=out.log --layout=neato --dpi=300 -o out.png graph.dot
        $> sccs -v  --layout=neato --dpi=300 -o out.jpg graph.dot
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

        const char* const short_opts = "o:hvr";

        const option long_opts[] = {
                {"dpi", required_argument, nullptr, 'd'},
                {"output", required_argument, nullptr, 'o'},
                {"layout", required_argument, nullptr, 'l'},
                {"log", required_argument, nullptr, 'g'},
                {"help", no_argument, nullptr, 'h'},
                {"verbose", no_argument, nullptr, 'v'},
                {"randomize", no_argument, nullptr, 'r'},
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
                case 'o':
                        Option::output_file = optarg;
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
                case 'r':
                        Option::randomize = true;
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

        if (Option::output_file == "") {
                std::cerr << "No output file provided\n";
                exit(0);
        }

        size_t dot_index = Option::output_file.find_last_of(".");
        Option::extension = Option::output_file.substr(dot_index + 1);
        Option::output_file = Option::output_file.substr(0, dot_index);
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

        grlib::adj_list<grlib::Basic_edge> alist(cgraph);

        grlib::sccs_context<grlib::Basic_edge> sccs_cxt(alist);

        grlib::sccs(sccs_cxt);

        if (Option::should_output) {
                out() << "number of components: " << sccs_cxt.components_number << std::endl;
                for (size_t i = 0; i < alist.edges.size(); i++)
                        out() << "\"" << alist.vmap.names[i] << "\": " << sccs_cxt.scc[i] << "\n";
        }

        Color_pool color_pool;
        std::vector<std::string> colors(sccs_cxt.components_number);

        for (size_t i = 0; i < colors.size(); i++) {
                try {
                        colors[i] = Option::randomize ? color_pool.random_unique_color() : color_pool.next_color();
                } catch (std::exception& e) {
                        std::cout << "Not enough colors for vizualization\n";
                        exit(0);
                }
        }

        for (gviz::Node node : cgraph) {
                int node_index = alist.vmap.indexes[node.name()];
                int color_index = sccs_cxt.scc[node_index] - 1;

                if (!node.set_attr_safe("fillcolor", colors[color_index], "white")) {
                        out() << "set_attr_safe(fillcolor, " << colors[color_index] << ", white) failed\n";
                }
        }

        try {
                gviz::render(context, cgraph, Option::output_file, Option::extension);
        } catch (std::exception& e) {
                out() << e.what() << "\n";
        }

        return 0;
}

