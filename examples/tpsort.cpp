/** @file */
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <map>
#include <string>

#include <grlib/adj_list.hpp>
#include <grlib/tpsort.hpp>
#include <grlib/grlib.hpp>
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
R"(Topological sorting algorithm visualization of provided graph.
Usage: tpsort [OPTION]... [FILE]
Program options:
--dpi=<val>:                 dpi of generated files
[--extension -e]=<val>:      extension of generated files, png is default
--help -h:                   show help
--layout=<val>:              graph layout (neato default, see GraphViz documentation for possible layouts)
--log=<file>:                print program informations to the provided file.
--verbose -v:                print program informations to the standard input
[FILE]:                      file containing description of the graph, formats .txt and .dot are possible

Examples:
        $> tpsort --log=out.log --layout=neato --dpi=300 graph.dot
        $> tpsort -v --extension=jpg --layout=neato --dpi=300 graph.dot
)";
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

std::ostream& out()
{
        return Option::log_file != "" ? Option::log_stream : std::cout;
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

        gviz::render(context, cgraph, "0", Option::extension);
        int frame_number = 1;

        std::vector<int> sorted;
        try {
                sorted = grlib::tpsort(alist, 0);
        } catch (std::exception& e) {
                out() << e.what() << "\n";
                return 0;
        }

        if (Option::should_output) {
                out() << "[";
                size_t end = sorted.size();

                for (size_t i = end - 1; i > 0; i--)
                        out() << "\"" << alist.vmap.names[sorted[i]] << "\", ";

                out() << "\"" << alist.vmap.names[sorted[0]] << "\"]\n";

        }

        for (auto p = sorted.rbegin(); p != sorted.rend(); p++) {
                gviz::Node node(cgraph.find_node(alist.vmap.names[*p]));

                if (!node.set_attr("fillcolor", "#ff4000")) {
                        continue;
                }

                try {
                        gviz::render(context, cgraph, std::to_string(frame_number++), Option::extension);
                } catch (std::exception& e) {
                        out() << e.what() << "\n";
                }
        }

        return 0;
}

