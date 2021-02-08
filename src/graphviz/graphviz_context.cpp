/** @file */
#include "graphviz/graphviz_context.hpp"

#include <stdexcept>

gviz::graphviz_context::graphviz_context()
: gvc(gvContext())
{
}

gviz::graphviz_context::graphviz_context(cgraph& graph, const std::string& layout)
: gvc(gvContext())
{
        set_layout(graph, layout);
}

void gviz::graphviz_context::set_layout(cgraph& graph, const std::string& layout)
{
        gvLayout(gvc, graph.data(), const_cast<char*>(layout.c_str()));
}

GVC_t* gviz::graphviz_context::data()
{
        return gvc;
}

gviz::graphviz_context::~graphviz_context()
{
        // TODO: Check return
        gvFreeContext(gvc);
}

void gviz::render(gviz::graphviz_context& context, gviz::cgraph& graph, const std::string& file_name,
                const std::string& extension)
{
        std::string s = file_name;
        s += ".";
        s += extension;

        FILE *f = fopen(s.c_str(), "w");

        if (!f)
                throw std::runtime_error{"gviz::render(): cannot open: " + s + " file."};

        gvRender(context.data(), graph.data(), extension.c_str(), f);
        fclose(f);
}
