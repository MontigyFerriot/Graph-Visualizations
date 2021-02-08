/** @file */
#include "graphviz/cgraph.hpp"

#include <stdexcept>
#include <iostream>

gviz::cgraph::cgraph()
: Object(agopen((char*)"default-graph", Agdirected, nullptr))
{
}

gviz::cgraph::cgraph(const std::string& graph_name, graph_type type,
                Agdisc_t* disc)
: Object(agopen(const_cast<char*>(graph_name.c_str()), *types[static_cast<int>(type)], disc))
{
}

gviz::cgraph::cgraph(const std::string& file_name, Agdisc_t* disc)
{
        FILE *fp = fopen(const_cast<char*>(file_name.c_str()), "r");

        if (!fp) {
                obj = nullptr;
                return;
        }

        obj = &(agread(fp, disc)->base);

        fclose(fp);
}

gviz::cgraph::cgraph(cgraph&& other)
{
        obj = other.obj;
        other.obj = nullptr;
}

void gviz::cgraph::operator=(cgraph&& other)
{
        if (this == &other)
                return;

        obj = other.obj;
        other.obj = nullptr;
}

gviz::cgraph::~cgraph()
{
        Agraph_t* g = to_graph(obj);

        if (g)
                agclose(g);
}

gviz::Node_iterator gviz::cgraph::begin()
{
        Agraph_t* g = to_graph(obj);
        return Node_iterator(agfstnode(g), g);
}

gviz::Node_iterator gviz::cgraph::end()
{
        Agraph_t* g = to_graph(obj);
        return Node_iterator(nullptr, g);
}

bool gviz::cgraph::def_attr(int obj_type, const std::string& attr, const std::string& value)
{
        Agraph_t* g = to_graph(obj);

        auto ptr = agattr(g, obj_type, (char*)attr.c_str(), (char*)value.c_str());

        return ptr != nullptr;
}

bool gviz::cgraph::def_graph_attr(const std::string& attr, const std::string& value)
{
        return def_attr(AGRAPH, attr, value);
}

bool gviz::cgraph::def_node_attr(const std::string& attr, const std::string& value)
{
        return def_attr(AGNODE, attr, value);
}

bool gviz::cgraph::def_edge_attr(const std::string& attr, const std::string& value)
{
        return def_attr(AGEDGE, attr, value);
}

int gviz::cgraph::nodes_number() const
{
        Agraph_t* g = to_graph(obj);
        return agnnodes(g);
}

bool gviz::cgraph::is_directed() const
{
        Agraph_t* g = to_graph(obj);
        return agisdirected(g);
}

gviz::Node gviz::cgraph::find_node(const std::string& node_name)
{
        Agraph_t* g = to_graph(obj);
        return agnode(g, const_cast<char*>(node_name.c_str()), false);
}

std::string gviz::cgraph::find_attr(int obj_type, const std::string& attr)
{
        Agraph_t* g = to_graph(obj);
        Agsym_t *sym = agattr(g, obj_type, const_cast<char*>(attr.c_str()), nullptr);

        if (!sym)
                throw std::runtime_error("Graph attribute: \"" + attr + "\" not defined");

        return sym->defval;
}

std::string gviz::cgraph::find_graph_attr(const std::string& attr)
{
        return find_attr(AGRAPH, attr);
}

std::string gviz::cgraph::find_node_attr(const std::string& attr)
{
        return find_attr(AGNODE, attr);
}

std::string gviz::cgraph::find_edge_attr(const std::string& attr)
{
        return find_attr(AGEDGE, attr);
}

bool gviz::cgraph::add_node(const std::string& node_name)
{
        Agraph_t* g = to_graph(obj);
        Agnode_t* node = agnode(g, (char*)node_name.c_str(), true);
        return !!node;
}

gviz::Edge gviz::cgraph::find_edge(gviz::Node& u, gviz::Node& v, const std::string& name)
{
        Agraph_t* g = to_graph(obj);
        return agedge(g, u.data(), v.data(), const_cast<char*>(name.c_str()), false);
}

gviz::Edge gviz::cgraph::find_edge(gviz::Node& u, gviz::Node& v)
{
        Agraph_t* g = to_graph(obj);
        return agedge(g, u.data(), v.data(), nullptr, false);
}

bool gviz::cgraph::add_edge(gviz::Node& u, gviz::Node& v, const std::string& name)
{
        Agraph_t* g = to_graph(obj);
        Agedge_t* edge = agedge(g, u.data(), v.data(), const_cast<char*>(name.c_str()), true);
        return !!edge;
}

Agraph_t* gviz::cgraph::data()
{
        Agraph_t* g = to_graph(obj);
        return g;
}

