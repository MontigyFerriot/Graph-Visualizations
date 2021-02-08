/** @file */
#pragma once

#include "graphviz/node.hpp"
#include "graphviz/object.hpp"

namespace gviz {

struct Edge : public Object {
        /**
         * Initialize the edge to invalid state (check it with operator!())
         */
        Edge();
        /**
         * Initialize the edge. On fail, edge is in invalid state (check it with operator!())
         * @param edge: native edge pointer
         */
        Edge(Agedge_t* edge);

        gviz::Node tail();
        const gviz::Node tail() const;

        gviz::Node head();
        const gviz::Node head() const;

        /**
         * @return internal pointer to the edge
         */
        Agedge_t* data();

        /**
         * @return internal pointer to the edge
         */
        const Agedge_t* data() const;
        friend class Edge_iterator;
};

}; // namespace gviz

