/** @file */
#pragma once

#include "graphviz/object.hpp"

namespace gviz {

class Edge_iterator;
class Node : public Object {
    public:
        /**
         * Initialize the edge to invalid state (check it with operator!())
         */
        Node();

        /**
         * Initialize the node. On fail, edge is in invalid state (check it with operator!())
         * @param node: native node pointer
         * @param graph: graph from which the node has been retrieved
         */
        Node(Agnode_t* node, Agraph_t* graph = nullptr);

        /**
         * @return iterator to the beginning of edge sequence
         */
        Edge_iterator begin();

        /**
         * @return iterator to the end of edge sequence
         */
        Edge_iterator end();

        /**
         * @return iterator to the beginning of edge sequence
         */
        const Edge_iterator begin() const;

        /**
         * @return iterator to the end of edge sequence
         */
        const Edge_iterator end() const;

        /**
         * @return internal pointer to the node
         */
        Agnode_t* data();

        /**
         * @return internal pointer to the node
         */
        const Agnode_t* data() const;

    private:
        Agraph_t* g;
};

}; // namespace gviz

