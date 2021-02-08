/** @file */
#include "graphviz/node.hpp"
#include "graphviz/edge_iterator.hpp"

#include <iostream>

gviz::Node::Node()
: Object() { }

gviz::Node::Node(Agnode_t* node, Agraph_t* graph)
: Object(node), g(graph) { }

gviz::Edge_iterator gviz::Node::begin()
{
        Agnode_t* n = to_node(obj);
        return Edge_iterator(agfstout(g, n), g);
}

gviz::Edge_iterator gviz::Node::end()
{
        return Edge_iterator(nullptr, g);
}

const gviz::Edge_iterator gviz::Node::begin() const
{
        Agnode_t* n = to_node(obj);
        return Edge_iterator(agfstout(g, n), g);
}

const gviz::Edge_iterator gviz::Node::end() const
{
        return Edge_iterator(nullptr, g);
}

Agnode_t* gviz::Node::data()
{
        Agnode_t* n = to_node(obj);
        return n;
}

const Agnode_t* gviz::Node::data() const
{
        Agnode_t* n = to_node(obj);
        return n;
}
