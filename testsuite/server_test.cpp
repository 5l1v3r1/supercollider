#include <boost/test/unit_test.hpp>

#include "../source/server/server.hpp"
#include "../source/server/server_args.hpp"

using namespace nova;
using namespace boost;


namespace
{
struct test_synth_prototype:
    public synth_prototype
{
    test_synth_prototype(void):
        synth_prototype("foo")
    {}

    abstract_synth * create_instance(int node_id)
    {
        return new synth(node_id, this);
    }
};
}


BOOST_AUTO_TEST_CASE( server_test_1 )
{
    rt_pool.init(1024 * 1024);

    nova_server server(server_arguments::initialize(0, 0));
    rt_pool.init(1024*1024);
    sc_factory.initialize();


    server.synth_factory::register_prototype(new test_synth_prototype());

    node_position_constraint to_root = std::make_pair(server.root_group(), insert);

    abstract_synth * s = server.add_synth("foo", 1, to_root);
    server.free_node(s);
}

BOOST_AUTO_TEST_CASE( server_test_2 )
{
    nova_server server(server_arguments::initialize(0, 0));
    server.synth_factory::register_prototype(new test_synth_prototype());

    node_position_constraint to_root = std::make_pair(server.root_group(), insert);

    abstract_synth * s = server.add_synth("foo", 1, to_root);
    server();
    server();
    server();
    server.free_node(s);
}

BOOST_AUTO_TEST_CASE( server_test_3 )
{
    nova_server server(server_arguments::initialize(0, 0));
    server.synth_factory::register_prototype(new test_synth_prototype());

    parallel_group * g = new parallel_group(1);

    server.add_node(g);

    node_position_constraint to_group = std::make_pair(g, insert);

    abstract_synth * s1 = server.add_synth("foo", 2, to_group);
    abstract_synth * s2 = server.add_synth("foo", 3, to_group);
    abstract_synth * s3 = server.add_synth("foo", 4, to_group);
    server();
    server();
    server();
    server.free_node(s1);
    server.free_node(s2);
    server.free_node(s3);
}
