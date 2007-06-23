#include <boost/test/auto_unit_test.hpp>

#include "testsuite.hh"
#include <boost/graph/adjacency_list.hpp>
#include <utilmm/iterator_sequence.hh>
#include <utilmm/undirected_graph.hh>
#include <utilmm/undirected_dfs.hh>
#include <vector>
#include <boost/concept_check.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace utilmm;
using namespace std;

BOOST_AUTO_TEST_CASE( test_iterator_sequence )
{
    int seq1[] = { 0, 1, 2 };
    int seq2[] = { 3, 4, 5 };

    std::vector<int> v1(seq1, seq1 + 3);
    std::vector<int> v2(seq2, seq2 + 3);

    typedef iterator_sequence<vector<int>::iterator, vector<int>::iterator>
	it_seq_t;

    it_seq_t it_seq_begin(v1.begin(), v1.end(), v2.begin(), v2.begin());
    it_seq_t it_seq = it_seq_begin;
    it_seq_t it_seq_end(v1.end(), v1.end(), v2.begin(), v2.end());


    // forward traversal
    for (int i = 0; i < 6; ++i, ++it_seq)
	BOOST_REQUIRE_EQUAL(*it_seq, i);
    BOOST_REQUIRE(it_seq == it_seq_end);

    // backward traversal
    for (int i = 0; i < 6; ++i)
    {
	--it_seq;
	BOOST_REQUIRE_EQUAL(*it_seq, 5 - i);
    }
    BOOST_REQUIRE(it_seq == it_seq_begin);

    // advance
    it_seq += 2;
    BOOST_REQUIRE_EQUAL(*it_seq, 2);
    BOOST_REQUIRE_EQUAL(2, it_seq - it_seq_begin);
    it_seq += 2;
    BOOST_REQUIRE_EQUAL(*it_seq, 4);
    BOOST_REQUIRE_EQUAL(4, it_seq - it_seq_begin);
    ++it_seq;
    BOOST_REQUIRE_EQUAL(*it_seq, 5);
    BOOST_REQUIRE_EQUAL(5, it_seq - it_seq_begin);
    it_seq -= 2;
    BOOST_REQUIRE_EQUAL(*it_seq, 3);
    BOOST_REQUIRE_EQUAL(3, it_seq - it_seq_begin);
    it_seq -= 2;
    BOOST_REQUIRE_EQUAL(*it_seq, 1);
    BOOST_REQUIRE_EQUAL(1, it_seq - it_seq_begin);
}

// Concept checking
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
	boost::no_property, boost::property<boost::edge_color_t, boost::default_color_type> > BaseGraph;
typedef undirected_graph< BaseGraph, const BaseGraph& > UndirectedGraph;
BOOST_CLASS_REQUIRE(UndirectedGraph, boost, AdjacencyGraphConcept);
BOOST_CLASS_REQUIRE(UndirectedGraph, boost, IncidenceGraphConcept);

BOOST_AUTO_TEST_CASE( test_undirected_property_maps_adaptor )
{
    using namespace boost;
    BaseGraph g;
    UndirectedGraph undirected(g);
    BaseGraph::vertex_descriptor v1, v2;
    v1 = add_vertex(g);
    v2 = add_vertex(g);
    BaseGraph::edge_descriptor e = add_edge(v1, v2, g).first;
    UndirectedGraph::edge_descriptor u_e = *edges(undirected).first;

    put(boost::edge_color_t(), g, e, boost::white_color);
    
    typedef property_map<UndirectedGraph, edge_color_t>::const_type PMap;
    undirected_property_map<PMap> u_pmap(get(boost::edge_color_t(), undirected));
    int value = get(u_pmap, u_e);
    BOOST_REQUIRE_EQUAL(boost::white_color, value);
}

/* Test that the edges have the proper source() and target() */
BOOST_AUTO_TEST_CASE( test_undirected_incidence )
{
    BaseGraph g;
    UndirectedGraph undirected(g);
    using namespace boost;

    BaseGraph::vertex_descriptor v1, v2, v3;
    UndirectedGraph::out_edge_iterator it, end;

    v1 = add_vertex(g);
    v2 = add_vertex(g);
    v3 = add_vertex(g);
    add_edge(v1, v2, g);
    add_edge(v2, v3, g);
    add_edge(v3, v1, g);

    tie(it, end) = out_edges(v1, undirected);
    BOOST_REQUIRE(it->second);
    BOOST_REQUIRE(source(*it, undirected) == v1);
    BOOST_REQUIRE(target(*it, undirected) == v2 || target(*it, undirected) == v3);
    BOOST_REQUIRE(!(++it)->second);
    BOOST_REQUIRE(source(*it, undirected) == v1);
    BOOST_REQUIRE(target(*it, undirected) == v2 || target(*it, undirected) == v3);
    
    tie(it, end) = out_edges(v2, undirected);
    BOOST_REQUIRE(source(*it, undirected) == v2);
    BOOST_REQUIRE(target(*it, undirected) == v1 || target(*it, undirected) == v3);
    BOOST_REQUIRE(source(*++it, undirected) == v2);
    BOOST_REQUIRE(target(*it, undirected) == v1 || target(*it, undirected) == v3);
}


/* Check that some algorithms compile on undirected graph */
BOOST_AUTO_TEST_CASE( test_undirected_algorithms )
{
    BaseGraph g;
    UndirectedGraph undirected(g);
    using namespace boost;

    typedef associative_property_map< std::map<BaseGraph::vertex_descriptor, default_color_type> > 
	VertexColorMap;
    associative_property_map< std::map<UndirectedGraph::vertex_descriptor, int> > component_map;
    VertexColorMap color_map;
    connected_components(undirected, component_map, boost::color_map(color_map));

    default_dfs_visitor dfs_vis;
    depth_first_search(undirected, visitor(dfs_vis));
    default_bfs_visitor bfs_vis;
    breadth_first_search(undirected, *vertices(g).first, visitor(bfs_vis));

    UndirectedGraph::vertex_descriptor v1 = add_vertex(g);
    // typedef associative_property_map< std::map<BaseGraph::edge_descriptor, default_color_type> > 
    //     EdgeColorMap;
    // EdgeColorMap edge_color_map;
    utilmm::undirected_dfs(undirected, dfs_vis,
	    color_map, make_undirected_edge_map(get(boost::edge_color_t(), g)), v1);
}
