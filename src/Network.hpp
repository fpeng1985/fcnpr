#pragma once
#include <mockturtle/mockturtle.hpp>
#include <iostream>
#include <vector>

namespace pandr::network {
	using level = uint32_t;
	using inode = uint64_t;

	class Network : public mockturtle::mig_network {
		public:
			uint32_t depth() const;
			std::vector<inode> nodes_at_level(level const l) const;
			std::vector<inode> node_fan_ins(node const n) const;
			uint32_t level_distance(node const n, node const m) const;
		/*operators*/
			friend std::ostream& operator<<(std::ostream& ostr, Network const& ntk);
	};

	uint32_t Network::depth() const {
		mockturtle::depth_view view{*this};
		return view.depth();
	}

	std::vector<inode> Network::nodes_at_level(level const l) const {
		using mockturtle::mig_network;
		using mockturtle::depth_view;
		using std::vector;

		depth_view view{*this};
		vector<inode> nodes;
		
		this->foreach_node([&](node const& n){
				auto i {this->node_to_index(n)};
				if(i != 0){
					if(view.level(n) == l){
						nodes.push_back(i);
					}
				}
		});

		if(nodes.empty()) throw std::range_error("Invalid level access");

		return nodes;
	}

	std::vector<inode> Network::node_fan_ins(node const n) const {
		using mockturtle::mig_network;
		using std::vector;
		vector<inode> fis;

		this->foreach_fanin(n, [&](signal const& s){
			auto fi {this->get_node(s)};
			auto fi_index {this->node_to_index(fi)};
			if(this->node_to_index(fi) != 0)
				fis.push_back(fi_index);
		});

		return fis;
	}

	uint32_t Network::level_distance(node const n, node const m) const {
		using mockturtle::depth_view;
		depth_view view{*this};

		return view.level(n) - view.level(m);
	}

	std::ostream& operator<<(std::ostream& ostr, Network const& ntk){
		ostr << "\033[32;1m * \033[0mDepth: " << ntk.depth() << std::endl;

		ostr << std::endl;

		ostr << "\033[32;1m * \033[0mLevels: " << std::endl;

		for(auto i{0}; i<=ntk.depth(); ++i){
			ostr << "\t" << i << ": ";
			auto level {ntk.nodes_at_level(i)};
			for(auto node : level){
				ostr << ntk.node_to_index(node) << " ";
			}
			ostr << std::endl;
		}

		ostr << std::endl;

		ostr << "\033[32;1m * \033[0mNode/Fanins: " << std::endl;
		ntk.foreach_node([&](auto const& n){
			if(ntk.node_to_index(n) != 0){
				ostr << "\tNode [" << ntk.node_to_index(n) << "] -> ";
				auto fis {ntk.node_fan_ins(n)};
				for(auto fi : fis){
					ostr << ntk.node_to_index(fi) << ", ";
				}
				ostr << std::endl;
			}
		});
		return ostr;
	}

} /* pandr::network namespace */
