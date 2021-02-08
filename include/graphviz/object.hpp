/** @file */
#pragma once

#include <string>
#include <gvc.h>

namespace gviz {

Agraph_t* to_graph(Agobj_t* obj);
Agnode_t* to_node(Agobj_t* obj);
Agedge_t* to_edge(Agobj_t* obj);

class Object {
    public:
        /**
         * Initialize the edge to invalid state (check it with operator!())
         */
        Object();

        /**
         * Initialize base object to the graph. On fail, edge is in invalid state (check it with operator!())
         * @param node: native graph pointer
         */
        Object(Agraph_t* graph);

        /**
         * Initialize base object to the node. On fail, edge is in invalid state (check it with operator!())
         * @param node: native node pointer
         */
        Object(Agnode_t* node);

        /**
         * Initialize base object to the graph. On fail, edge is in invalid state (check it with operator!())
         * @param edge: native edge pointer
         */
        Object(Agedge_t* edge);
        virtual ~Object();

        /**
         * @return bool indicating whether Object is in valid state (it is non-NULL)
         */
        bool operator!() const;

        /**
         * @return bool indicating whether Object is in invalid state (it is NULL)
         */
        operator bool() const;

        /**
         * Get object name
         * @return string with name value or throws the exception
         * if name is not defined
         */
        std::string name() const;

        /**
         * Set object's attribute
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return bool indicating whether attribute is applied
         */
        bool set_attr(const std::string& attr, const std::string& value);

        /**
         * Get object's attribute
         * @param attr: name of the attribute
         * @param value: value to be set
         * @return string with attribute value or throws the exception
         * if attribute is not defined
         */
        std::string get_attr(std::string attr);

        /**
         * Set object's attribute safely - if it is not defined for objects,
         * it creates it and set default value.
         * @param attr: name of the attribute
         * @param value: value to be set
         * @param default_value: default value the attribute will have for object class (nodes, edges)
         * @return bool indicating whether attribute is applied
         */
        bool set_attr_safe(const std::string& attr, const std::string& value,
                        const std::string& default_value);
    protected:
        Agobj_t* obj; /// pointer to the base object
};

}; // namespace gviz

