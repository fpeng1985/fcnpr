//
// Created by fpeng on 2020/6/25.
//

#include <cassert>
#include <fstream>
#include "Network.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>


namespace fcnpr {

    void Network::parse(const std::string &fname){
        std::ifstream ifs(fname);
        auto result = lorina::read_verilog( ifs, mockturtle::verilog_reader( ntk ) );
        assert(result==lorina::return_code::success);
    }

    std::list<Node> Network::topological_order() const noexcept {
		using boost::vecS;
		using boost::directedS;
		boost::adjacency_list<vecS, vecS, directedS>  G(ntk.size());
		ntk.foreach_node([this, &G](const auto &node){
			this->ntk.foreach_fanin(node, [&node, &G](const auto &fanin){
                std::cout << fanin.index << "=>" << node << std::endl;
                boost::add_edge(fanin.index, node, G);
			});
		});
		
        std::list<Node> ret;
		topological_sort(G, std::front_inserter(ret));
		return ret;    
    }

     uint32_t Network::depth() const {
        depth_view view{ntk};
        return view.depth();
    }

     uint32_t Network::level_distance(Node const n, Node const m) const {
        depth_view view{ntk};

        auto l1 = view.level(n);
        auto l2 = view.level(m);
        return (l1>=l2)? (l1-l2) : (l2-l1);
    }

     std::vector<Node> Network::nodes_at_level(Level const l) const {
        depth_view view{ntk};

        std::vector<Node> nodes;

        ntk.foreach_node([&](Node const& n) {
            auto i {ntk.node_to_index(n)};
            if(i != 0) {
                if(view.level(n) == l){
                    nodes.push_back(i);
                }
            }
        });

        return nodes;
    }

     std::vector<Node> Network::fan_ins_of(Node const n) const {
        std::vector<Node> fis;

        ntk.foreach_fanin(n, [&](auto const& s){
            auto fi {ntk.get_node(s)};
            auto fi_index {ntk.node_to_index(fi)};
            if(ntk.node_to_index(fi) != 0)
                fis.push_back(fi_index);
        });

        return fis;
    }

     std::ostream& operator<<(std::ostream& ostr, const Network &src) {
        ostr << "\033[32;1m * \033[0mDepth: " << src.depth() << std::endl;

        ostr << std::endl;

        ostr << "\033[32;1m * \033[0mLevels: " << std::endl;

        for(auto i{0}; i<=src.depth(); ++i){
            ostr << "\t" << i << ": ";
            auto level_nodes {src.nodes_at_level(i)};
            for(auto node : level_nodes){
                ostr << src.ntk.node_to_index(node) << " ";
            }
            ostr << std::endl;
        }

        ostr << std::endl;

        ostr << "\033[32;1m * \033[0mNode/Fanins: " << std::endl;
        src.ntk.foreach_node([&](auto const& n){
            if(src.ntk.node_to_index(n) != 0){
                ostr << "\tNode [" << src.ntk.node_to_index(n) << "] -> ";
                auto fis {src.fan_ins_of(n)};
                for(auto fi : fis){
                    ostr << src.ntk.node_to_index(fi) << ", ";
                }
                ostr << std::endl;
            }
        });
        return ostr;
    }
    
    std::vector< std::pair<Node, std::vector<Node>> >
    Network::establish_traversal_data() noexcept {
        using boost::vecS;
		using boost::directedS;
		boost::adjacency_list<vecS, vecS, directedS>  G(ntk.size());
        
		ntk.foreach_node([this, &G](const auto &node){
			this->ntk.foreach_fanin(node, [&node, &G](const auto &fanin){
                boost::add_edge(fanin.index, node, G);
			});
		});
        
        std::list<Node> nodes_in_topo_order;
		topological_sort(G, std::front_inserter(nodes_in_topo_order));
        
        std::vector< std::pair<Node, std::vector<Node>> > travelsals;
        
        for(const auto&node: nodes_in_topo_order) {
            if(node !=0) {
                std::vector<Node> fanins;
                ntk.foreach_fanin(node, [&fanins, &node](const auto &fanin_sig){
                    auto index = fanin_sig.index;
                    if(index != 0) {
                        fanins.push_back(index);
                        //std::cout << index << "=>" << node << std::endl;
                    }
                });
                travelsals.emplace_back(std::make_pair(node, fanins));
            }
        }
        return travelsals;
    }
    
    
    
     Network &network() {
        static Network ntk;
        return ntk;
    }
}
