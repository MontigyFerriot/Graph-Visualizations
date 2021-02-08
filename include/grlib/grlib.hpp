/** @file */
#pragma once

#define CONFIG_ADJ_LIST_DEFAULT_CAP  2048
#define CONFIG_ADJ_MATRIX_DEFAULT_CAP 128

#define GRLIB_SYNC_WITH_GRAPHVIZ

#ifdef GRLIB_SYNC_WITH_GRAPHVIZ
#include "graphviz/wrapper.hpp"
#endif

#define grlib_log(fmt, ...) \
        printf("%s: " fmt, __PRETTY_FUNCTION__, ##__VA_ARGS__)

namespace grlib {

using vertex_id = int;

enum class Edge_type {
        tree,
        back,
        forward,
        cross
};

}; // namespace grlib

