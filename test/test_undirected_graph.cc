#include "testsuite.hh"
#include <boost/graph/adjacency_list.hpp>
#include <utilmm/iterator_sequence.hh>
#include <utilmm/undirected_graph.hh>
#include <vector>
#include <boost/concept_check.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace utilmm;
using namespace std;

class TC_UndirectedGraph
{
public:
    void test_iterator_sequence()
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
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> BaseGraph;
    typedef undirected_graph< BaseGraph > UndirectedGraph;
    BOOST_CLASS_REQUIRE(UndirectedGraph, boost, AdjacencyGraphConcept);
    BOOST_CLASS_REQUIRE(UndirectedGraph, boost, IncidenceGraphConcept);


    /* Check that some algorithms work on undirected graph */
    void test_undirected_algorithms()
    {
	BaseGraph g;
	UndirectedGraph undirected(g);
	using namespace boost;

	associative_property_map< std::map<UndirectedGraph::vertex_descriptor, int> > component_map;
	associative_property_map< std::map<UndirectedGraph::vertex_descriptor, default_color_type> > color_map;
	connected_components(undirected, component_map, boost::color_map(color_map));

	default_dfs_visitor dfs_vis;
	depth_first_search(undirected, visitor(dfs_vis));
	default_bfs_visitor bfs_vis;
	breadth_first_search(undirected, *vertices(g).first, visitor(bfs_vis));
    }
};

void test_undirected_graph(test_suite* ts)
{
    boost::shared_ptr<TC_UndirectedGraph> instance( new TC_UndirectedGraph );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_UndirectedGraph::test_iterator_sequence, instance ) );
}

