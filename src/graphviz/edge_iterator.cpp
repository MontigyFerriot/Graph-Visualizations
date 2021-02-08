/** @file */
#include "graphviz/edge_iterator.hpp"

gviz::Edge_iterator::Edge_iterator(Agedge_t* edge, Agraph_t* graph)
: e(edge), g(graph) { }

gviz::Edge_iterator& gviz::Edge_iterator::operator++()
{
        e = agnxtout(g, e);
        return *this;
}

gviz::Edge_iterator gviz::Edge_iterator::operator++(int)
{
        Edge_iterator tmp(*this);
        operator++();
        return tmp;
}

gviz::Edge gviz::Edge_iterator::operator*()
{
        return Edge(e);
}

const gviz::Edge gviz::Edge_iterator::operator*() const
{
        return Edge(e);
}

