/** @file */
#include "graphviz/object.hpp"
#include <stdexcept>

template<class P, class M>
static inline size_t gviz_offsetof(const M P::*member)
{
        return (size_t)&(reinterpret_cast<P*>(0)->*member);
}

template<class P, class M>
static inline P* gviz_container_of(M* ptr, const M P::*member)
{
        return (P*)((char*)ptr - gviz_offsetof(member));
}

Agraph_t* gviz::to_graph(Agobj_t* obj)
{
        return gviz_container_of(obj, &Agraph_t::base);
}

Agnode_t* gviz::to_node(Agobj_t* obj)
{
        return gviz_container_of(obj, &Agnode_t::base);
}

Agedge_t* gviz::to_edge(Agobj_t* obj)
{
        return gviz_container_of(obj, &Agedge_t::base);
}

gviz::Object::Object()
: obj(nullptr) { }

gviz::Object::Object(Agraph_t* graph)
: obj(&graph->base) { }

gviz::Object::Object(Agnode_t* node)
: obj(&node->base) { }

gviz::Object::Object(Agedge_t* edge)
: obj(&edge->base) { }

bool gviz::Object::operator!() const
{
        return !operator bool();
}

gviz::Object::operator bool() const
{
        return !!obj;
}

gviz::Object::~Object()
{

}

std::string gviz::Object::name() const
{
        char *p = agnameof(obj);

        return p ? std::string(p) : std::string("no-name");
}

bool gviz::Object::set_attr(const std::string& attr, const std::string& value)
{
        int ret = agset(obj, (char*)attr.c_str(), (char*)value.c_str());

        return ret < 0 ? false : true;
}

std::string gviz::Object::get_attr(std::string attr)
{
        char* str = agget(obj, (char*)attr.c_str());

        if (!str)
                throw std::runtime_error("gviz::Object::get_attr(): Cannot set attribute: " + attr);

        return str;
}

bool gviz::Object::set_attr_safe(const std::string& attr, const std::string& value,
                const std::string& default_value)
{
        int ret = agsafeset(obj, (char*)attr.c_str(), (char*)value.c_str(),
                        (char*)default_value.c_str());

        return ret < 0 ? false : true;
}
