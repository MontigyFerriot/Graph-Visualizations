/** @file */
#include "graphviz/node_iterator.hpp"

gviz::Node_iterator::Node_iterator(Agnode_t* node, Agraph_t* graph)
: n(node), g(graph) { }

gviz::Node_iterator& gviz::Node_iterator::operator++()
{
        n = agnxtnode(g, n);
        return *this;
}

gviz::Node_iterator gviz::Node_iterator::operator++(int)
{
        Node_iterator tmp(*this);
        operator++();
        return tmp;
}

gviz::Node gviz::Node_iterator::operator*()
{
        return Node(n, g);
}

const gviz::Node gviz::Node_iterator::operator*() const
{
        return Node(n, g);
}
