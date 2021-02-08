/** @file */
#pragma once

#include <gvc.h>
#include "graphviz/node.hpp"

namespace gviz {
struct Node_iterator {
        Node_iterator(Agnode_t* node, Agraph_t* graph);

        Node_iterator& operator++();
        Node_iterator operator++(int);

        Node operator*();
        const Node operator*() const;

        Agnode_t* n;
        Agraph_t* g;
};


inline bool operator==(const Node_iterator& n1, const Node_iterator& n2)
{
        return n1.n == n2.n;
}

inline bool operator!=(const Node_iterator& n1, const Node_iterator& n2)
{
        return n1.n != n2.n;
}

}; // namespace gviz

