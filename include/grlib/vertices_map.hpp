/** @file */
#pragma once

#include <map>
#include <vector>

#include "grlib/grlib.hpp"

namespace grlib {

struct Vertices_map {
        Vertices_map() = default;
        Vertices_map(int size)
        : indexes(), names(size), max_index(0UL) { }

        /**
         * add unique name for vertex with given index.
         * @param name: name of vertex to be added
         */
        void push(const std::string& name)
        {
                if (max_index == names.size())
                       names.resize(2 * names.size());

                names[max_index] = name;
                indexes[name] = max_index;
                max_index++;
        }

        /**
         * get id of the vertex with given index.
         * @param name: name of vertex to be added
         */
        vertex_id index(const std::string& name)
        {
                return indexes[name];
        }

        /**
         * get name of the vertex with given id.
         * @param index: index of the vertex
         */
        std::string name(vertex_id index) const
        {
                return names[index];
        }

        std::map<std::string, vertex_id> indexes;
        std::vector<std::string> names;
        size_t max_index;
};

/**
 * Print contents of vmap.
 * @param vmap: Vertices map to be printed.
 */
void print_vmap(const grlib::Vertices_map& vmap)
{
        std::cout << "-------------------\n";

        auto size = vmap.names.size();

        std::cout << "[";
        for (size_t i = 0; i < size - 1; i++)
                printf("\"%s\", ", vmap.names[i].c_str());

        printf("\"%s\"]\n\n", vmap.names[size - 1].c_str());

        for (const auto& p : vmap.indexes)
                printf("[%s: %d]\n", p.first.c_str(), p.second);

        std::cout << "------------------\n\n";
}

};

