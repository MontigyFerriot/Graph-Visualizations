/** @file */
#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include "graphviz/wrapper.hpp"

using std::tuple;
using std::make_tuple;
using std::get;

// Tests whether gviz::cgraph constructor works with valid file.
TEST(GraphvizWrapperTest, CgraphCtorFileOpen)
{
        // constructing with *valid* file
        gviz::cgraph cgraph("graphs/complex_graph.dot");

        // if opening success, cgraph internal pointer
        // should be non-NULL.

        // bool operator() should return true if opening success.
        EXPECT_TRUE(cgraph);

        // operator!() should return false if opening success.
        EXPECT_FALSE(!cgraph);
}

// Tests whether gviz::cgraph constructor behaves correctly with invalid files.
TEST(GraphvizWrapperTest, CgraphCtorInvalidFile)
{
        // constructing with *INvalid* file
        gviz::cgraph cgraph("undefined_file_9123649.dot");

        // if opening success, cgraph internal pointer
        // should be non-NULL.

        // bool operator() should return false if opening fails.
        EXPECT_FALSE(cgraph);

        // operator!() should return false if opening fails.
        EXPECT_TRUE(!cgraph);
}

// Tests whether gviz::cgraph default constructor
TEST(GraphvizWrapperTest, CgraphDefaultCtor)
{
        gviz::cgraph cgraph;
        EXPECT_TRUE(cgraph);
        EXPECT_FALSE(!cgraph);
}

// Tests whether gviz::cgraph moving works
TEST(GraphvizWrapperTest, CgraphMoving)
{
        gviz::cgraph g1("test graph", gviz::graph_type::directed);

        // g1 cannot be empty
        ASSERT_FALSE(!g1);

        gviz::cgraph g2 = std::move(g1);
        // g2 takes g1 data - g1 should be empty, g2 should not
        EXPECT_TRUE(!g1);
        EXPECT_FALSE(!g2);
}

class CgraphEmptyCtorTest : public ::testing::TestWithParam<tuple<const char *, gviz::graph_type>> {
};

TEST_P(CgraphEmptyCtorTest, CgraphEmptyCtor)
{
        char* name_for_graph = const_cast<char*>(get<0>(GetParam()));

        gviz::graph_type gtype = get<1>(GetParam());

        bool initially_directed = gtype == gviz::graph_type::directed
                                        or gtype == gviz::graph_type::strict_directed;

        [[maybe_unused]] bool initially_undirected = gtype == gviz::graph_type::undirected
                                        or gtype == gviz::graph_type::strict_undirected;

        gviz::cgraph cgraph(name_for_graph, gtype);

        // bool operator() should return true if opening success.
        ASSERT_TRUE(cgraph);

        // operator!() should return false if opening success.
        ASSERT_FALSE(!cgraph);

        const char* graph_name = cgraph.name().c_str();

        // check if name of the graph is the same as the provided one
        EXPECT_STREQ(graph_name, name_for_graph);

        // check whether graph parameter is retrived correctly
        EXPECT_EQ(cgraph.is_directed(), initially_directed);
        // EXPECT_EQ(cgraph.is_undirected(), initially_undirected);
}

// Tests whether constuction of empty graph success
INSTANTIATE_TEST_CASE_P(
        TestsOfCgraphEmptyCtor,
        CgraphEmptyCtorTest,
        ::testing::Values(
                make_tuple("test_graph1", gviz::graph_type::directed),
                make_tuple("test_graph2", gviz::graph_type::strict_directed),
                make_tuple("test_graph3", gviz::graph_type::undirected),
                make_tuple("test_graph4", gviz::graph_type::strict_undirected)
        ));

// Test adding/finding nodes in graph
TEST(GraphvizWrapperTest, CgraphNodeIteration)
{
        gviz::cgraph g("test graph", gviz::graph_type::directed);

        ASSERT_FALSE(!g);

        for (int i = 0; i < 10; i++)
                EXPECT_TRUE(g.add_node(std::to_string(i)));

        std::string node_name;
        int i = 0;
        for (gviz::Node node : g) {
                // test if node has been found
                EXPECT_TRUE(node);
                EXPECT_FALSE(!node);

                node_name = std::to_string(i++);

                const char* expected_name = node_name.c_str();
                const char* provided_name = node.name().c_str();

                EXPECT_STREQ(expected_name, provided_name);
        }
}

// Test adding/finding nodes in graph
TEST(GraphvizWrapperTest, CgraphAddFindNode)
{
        gviz::cgraph g("test graph", gviz::graph_type::directed);

        ASSERT_FALSE(!g);

        int size = 0;
        std::string node_name;

        for (int i = 0; i < 10; i++) {
                EXPECT_EQ(g.nodes_number(), size);
                node_name = std::to_string(i);
                EXPECT_TRUE(g.add_node(node_name));

                gviz::Node node = g.find_node(node_name);
                // test if node has been found
                EXPECT_TRUE(node);
                EXPECT_FALSE(!node);

                EXPECT_EQ(g.nodes_number(), ++size);
        }
}

// Test adding/finding edges in the graph
TEST(GraphvizWrapperTest, CgraphAddFindEdge)
{
        gviz::cgraph g("test graph", gviz::graph_type::directed);

        ASSERT_FALSE(!g);

        EXPECT_TRUE(g.add_node("u"));
        EXPECT_TRUE(g.add_node("v"));

        gviz::Node n1 = g.find_node("u");
        gviz::Node n2 = g.find_node("v");

        ASSERT_FALSE(!n1);
        ASSERT_FALSE(!n2);

        EXPECT_TRUE(g.add_edge(n1, n2, "new_edge"));

        // test if edge has been found
        gviz::Edge edge = g.find_edge(n1, n2, "new_edge");

        ASSERT_TRUE(edge);
        ASSERT_FALSE(!edge);

        EXPECT_STREQ("new_edge", edge.name().c_str());
}

//--------------------------------------------------------------

class CgraphObjGetSetAttr : public ::testing::TestWithParam<tuple<const char *, const char *>> {
};

/**
 * tests whether add_attr()/get_attr() Object base class member functions
 * work with attributes.
 */
TEST_P(CgraphObjGetSetAttr, CgraphObjGetSetAttrTest)
{
        const char* attr = get<0>(GetParam());
        const char* provided_value = get<1>(GetParam());

        gviz::cgraph g("test graph", gviz::graph_type::directed);
        ASSERT_FALSE(!g);

        // if attr is not defined in the graph structure,
        // setting it for object will fail.
        EXPECT_TRUE(g.def_node_attr(attr, ""));

        EXPECT_TRUE(g.add_node("u"));
        gviz::Node node = g.find_node("u");

        EXPECT_TRUE(node.set_attr(attr, provided_value));
        const char* value = node.get_attr(attr).c_str();
        EXPECT_STREQ(provided_value, value);
}

INSTANTIATE_TEST_CASE_P(
        TestsOfCgraphObjGetSetAttr,
        CgraphObjGetSetAttr,
        ::testing::Values(
                make_tuple("fillcolor", "#ffffff"),
                make_tuple("fillcolor", "black"),
                make_tuple("color", "blue"),
                make_tuple("shape", "doublecircle"),
                make_tuple("style", "filed")
        ));

int main(int argc, char *argv[])
{
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}

