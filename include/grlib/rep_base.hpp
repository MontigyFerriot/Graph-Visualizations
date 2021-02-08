/** @file */
#pragma once

#include "grlib/grlib.hpp"
#include "grlib/vertices_map.hpp"

namespace grlib {

struct Representation_base {
        /**
         * Initialize represention base
         * @param size: number/range of preallocated nodes
         * @param directed: whether graph is directed
         * @param enumber: number of edges
         */
        Representation_base(size_t size, bool directed, size_t enumber)
        : vmap(size), directed(directed), enumber(enumber) { }

        virtual ~Representation_base() = default;

        /**
         * @return number of edges
         */
        size_t edges_number() const
        {
                return enumber;
        }

        grlib::Vertices_map vmap;
        bool directed;
        size_t enumber;
};

};

