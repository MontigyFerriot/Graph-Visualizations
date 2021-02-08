/** @file */
#pragma once

#include "graphviz/node_iterator.hpp"
#include "graphviz/edge.hpp"
#include "graphviz/object.hpp"

namespace gviz {

enum class graph_type {
        directed,
        strict_directed,
        undirected,
        strict_undirected
};

struct cgraph : public Object {
        /**
         * Initialize the graph. On fail, graph is in invalid state (check it with operator!())
         */
        cgraph();

        /**
         * Initialize the graph. On fail, graph is in invalid state (check it with operator!())
         * @param graph_name: Name of the graph
         * @param graph_type type: type of graph
         * @param disc: optional disc callbacks structure.
         */
        cgraph(const std::string& graph_name, graph_type type, Agdisc_t* disc=nullptr);

        /**
         * Initialize the graph. On fail, graph is in invalid state (check it with operator!())
         * @param file_name: Name of the file (.dot preferably) from which the graph will be initialized
         * @param disc: optional disc callbacks structure.
         */
        cgraph(const std::string& file_name, Agdisc_t* disc=nullptr);

        // copying prevented, moving is okay

        cgraph(const cgraph& other);
        void operator=(const cgraph& other) = delete;
        cgraph(cgraph&& other);
        void operator=(cgraph&& other);

        ~cgraph();

        /**
         * @return iterator to the beginning of node sequence
         */
        gviz::Node_iterator begin();

        /**
         * @return iterator to the end of node sequence
         */
        gviz::Node_iterator end();

        /**
         * Define graph attribute
         * @param obj_type: type of Graphviz object
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return bool indicating whether attribute is applied
         */
        bool def_attr(int obj_type, const std::string& attr, const std::string& value);

        /**
         * Define graph attribute
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return bool indicating whether attribute is applied
         */
        bool def_graph_attr(const std::string& attr, const std::string& value);

        /**
         * Define node attribute
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return bool indicating whether attribute is applied
         */
        bool def_node_attr(const std::string& attr, const std::string& value);

        /**
         * Define edge attribute
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return bool indicating whether attribute is applied
         */
        bool def_edge_attr(const std::string& attr, const std::string& value);

        /**
         * Find object attribute
         * @param obj_type: type of Graphviz object
         * @param attr: attribute to be found
         * @return string with attribute value or throws the exception
         * if attribute is not defined
         */
        std::string find_attr(int obj_type, const std::string& attr);

        /**
         * Find graph attribute
         * @param obj_type: type of Graphviz object
         * @param attr: attribute to be found
         * @return string with attribute value or throws the exception
         * if attribute is not defined
         */
        std::string find_graph_attr(const std::string& attr);

        /**
         * Find edge attribute
         * @param obj_type: type of Graphviz object
         * @param attr: attribute to be found
         * @return string with attribute value or throws the exception
         * if attribute is not defined
         */
        std::string find_node_attr(const std::string& attr);

        /**
         * Find edge attribute
         * @param obj_type: type of Graphviz object
         * @param attr: attribute to be found
         * @return string with attribute value or throws the exception
         * if attribute is not defined
         */
        std::string find_edge_attr(const std::string& attr);

        int nodes_number() const;
        bool is_directed() const;

        /**
         * Find graph node.
         * @param node_name: name of the node
         * @return valid or invalid node, depending whether it is defined in graph.
         */
        gviz::Node find_node(const std::string& node_name);

        /**
         * Add a node to graph.
         * @param node_name: name of the node to be added
         * @return bool indicating, whether node was added or not.
         */
        bool add_node(const std::string& node_name);

        /**
         * Find graph edge u --> v.
         * @param u: tail node
         * @param v: head node
         * @param name: name of the node to be added
         * @return valid or invalid edge, depending whether it is defined in graph.
         */
        gviz::Edge find_edge(gviz::Node& u, gviz::Node& v, const std::string& name);

        /**
         * Find *ANY* of the graph's edge u --> v.
         * @param u: tail node
         * @param v: head node
         * @return valid or invalid edge, depending whether it is defined in graph.
         */
        gviz::Edge find_edge(gviz::Node& u, gviz::Node& v);

        /**
         * Add u --> v edge to the graph.
         * @param u: tail node
         * @param v: head node
         * @return bool indicating, whether edge was added or not.
         */
        bool add_edge(gviz::Node& u, gviz::Node& v, const std::string& name);

        Agraph_t* data();

        static constexpr Agdesc_t* const types[4] = {&Agdirected, &Agstrictdirected,
                &Agundirected, &Agstrictundirected};
};

}; // namespace gviz

