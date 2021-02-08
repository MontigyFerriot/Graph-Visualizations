/** @file */
#include "graphviz/edge.hpp"

gviz::Edge::Edge()
: Object() { }

gviz::Edge::Edge(Agedge_t* edge)
: Object(edge) { }

gviz::Node gviz::Edge::tail()
{
        Agedge_t* edge = to_edge(obj);
        return agtail(edge);
}

const gviz::Node gviz::Edge::tail() const
{
        Agedge_t* edge = to_edge(obj);
        return agtail(edge);
}

gviz::Node gviz::Edge::head()
{
        Agedge_t* edge = to_edge(obj);
        return aghead(edge);
}

const gviz::Node gviz::Edge::head() const
{
        Agedge_t* edge = to_edge(obj);
        return aghead(edge);
}

Agedge_t* gviz::Edge::data()
{
        Agedge_t* edge = to_edge(obj);
        return edge;
}

const Agedge_t* gviz::Edge::data() const
{
        Agedge_t* edge = to_edge(obj);
        return edge;
}

