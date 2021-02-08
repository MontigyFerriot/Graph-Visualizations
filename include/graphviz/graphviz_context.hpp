/** @file */
#pragma once

#include <gvc.h>

#include "graphviz/cgraph.hpp"

namespace gviz {

class graphviz_context {
    public:
        graphviz_context();
        graphviz_context(cgraph& graph, const std::string& layout);

        void set_layout(cgraph& graph, const std::string& layout);

        graphviz_context(const graphviz_context& other) = delete;
        graphviz_context(graphviz_context&& other) = delete;
        void operator=(const graphviz_context& other) = delete;
        void operator=(graphviz_context&& other) = delete;

        GVC_t* data();

        ~graphviz_context();

    private:
        GVC_t* gvc;
};

void render(graphviz_context& context, cgraph& graph, const std::string& file_name,
                const std::string& extension);

}; // namespace gviz

