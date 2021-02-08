/** @file */
#pragma once

#include "graphviz/edge.hpp"

namespace gviz {

class Edge_iterator {
    public:
        Edge_iterator(Agedge_t* edge, Agraph_t* graph);

        Edge_iterator& operator++();
        Edge_iterator operator++(int);

        Edge operator*();
        const Edge operator*() const;
        Agedge_t* e;
        Agraph_t* g;
};

} // namespace gviz

inline bool operator==(const gviz::Edge_iterator& e1, const gviz::Edge_iterator& e2)
{
        return e1.e == e2.e;
}

inline bool operator!=(const gviz::Edge_iterator& e1, const gviz::Edge_iterator& e2)
{
        return e1.e != e2.e;
}

