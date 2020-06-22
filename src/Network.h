//
// Created by fpeng on 2020/6/22.
//

#ifndef ROPPER_NETWORK_H
#define ROPPER_NETWORK_H
#include <Exception.hpp>
#include <mockturtle/mockturtle.hpp>
#include <iostream>
#include <vector>

namespace fcnpr {
    using Level = uint32_t;
    using inode = uint64_t;
    using signal = int32_t;

    using namespace mockturtle;

    template<typename Ntk>
    class Network {
    protected:
        Ntk ntk;
        decltype(depth_view(ntk)) d_view;
        uint32_t level_multiplier;
    public:
        Network(uint32_t level_multiplier);
        Network(uint32_t level_multiplier, Ntk const& ntk);
        virtual ~Network() = default;
        uint32_t node_level(inode const n) const;
        uint32_t depth() const;
        std::vector<inode> nodes_at_level(Level const l) const;
        std::vector<inode> node_fan_ins(inode const n) const;
        uint32_t level_distance(inode const n, inode const m) const;
        uint32_t multiplier() const;
        Ntk& get();
        /* Operators */
        template<typename _Ntk> friend std::ostream& operator<<(std::ostream& ostr, Network<_Ntk> const& src);
        /* Exceptions */
        class invalid_level_access : public pandr::exception{using exception::exception;};
    };

    template<typename Ntk>
    Network<Ntk>::Network(uint32_t level_multiplier, Ntk const& ntk)
            : ntk(ntk)
            , d_view(this->ntk)
            , level_multiplier(level_multiplier)
    {
    }

    template<typename Ntk>
    Network<Ntk>::Network(uint32_t level_multiplier)
            : d_view(this->ntk)
            , level_multiplier(level_multiplier)
    {
    }

    template<typename Ntk>
    uint32_t Network<Ntk>::node_level(inode const n) const {
        //TODO Figure it out why this fails when using d_view
        depth_view view{this->ntk};
        return view.level(n);
    }

    template<typename Ntk>
    uint32_t Network<Ntk>::depth() const {
        //TODO Figure it out why this fails when using d_view
        depth_view view{this->ntk};
        return view.depth();
    }

    template<typename Ntk>
    std::vector<inode> Network<Ntk>::nodes_at_level(level const l) const {
        //TODO Figure it out why this fails when using d_view
        depth_view view{this->ntk};

        std::vector<inode> nodes;

        this->ntk.foreach_node([&](inode const& n){
            auto i {this->ntk.node_to_index(n)};
            if(i != 0){
                if(view.level(n) == l){
                    nodes.push_back(i);
                }
            }
        });

        if(nodes.empty()) throw invalid_level_access("Invalid level access, empty graph?");

        return nodes;
    }

    template<typename Ntk>
    std::vector<inode> Network<Ntk>::node_fan_ins(inode const n) const {
        std::vector<inode> fis;

        this->ntk.foreach_fanin(n, [&](auto const& s){
            auto fi {this->ntk.get_node(s)};
            auto fi_index {this->ntk.node_to_index(fi)};
            if(this->ntk.node_to_index(fi) != 0)
                fis.push_back(fi_index);
        });

        return fis;
    }

    template<typename Ntk>
    uint32_t Network<Ntk>::level_distance(inode const n, inode const m) const {
        depth_view view{this->ntk};

        return view.level(n) - view.level(m);
    }

    template<typename Ntk>
    uint32_t Network<Ntk>::multiplier() const {
        return this->level_multiplier;
    }

    template<typename Ntk>
    Ntk& Network<Ntk>::get() {
        return this->ntk;
    }

    template<typename _Ntk>
    std::ostream& operator<<(std::ostream& ostr, Network<_Ntk> const& src) {
        ostr << "\033[32;1m * \033[0mDepth: " << src.d_view.depth() << std::endl;

        ostr << std::endl;

        ostr << "\033[32;1m * \033[0mLevels: " << std::endl;

        for(auto i{0}; i<=src.d_view.depth(); ++i){
            ostr << "\t" << i << ": ";
            auto level {src.nodes_at_level(i)};
            for(auto node : level){
                ostr << src.d_view.node_to_index(node) << " ";
            }
            ostr << std::endl;
        }

        ostr << std::endl;

        ostr << "\033[32;1m * \033[0mNode/Fanins: " << std::endl;
        src.d_view.foreach_node([&](auto const& n){
            if(src.d_view.node_to_index(n) != 0){
                ostr << "\tNode [" << src.d_view.node_to_index(n) << "] -> ";
                auto fis {src.node_fan_ins(n)};
                for(auto fi : fis){
                    ostr << src.d_view.node_to_index(fi) << ", ";
                }
                ostr << std::endl;
            }
        });
        return ostr;
    }

} /* pandr::network namespace */

#endif //ROPPER_NETWORK_H
