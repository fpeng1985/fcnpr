
#include "PandR_TopoOrder.h"
#include "Network.h"
#include "ChessBoard.h"

#include <memory>
#include <iostream>

namespace fcnpr {
    
    PandR::PandR() {
        traversals = network().establish_traversal_data();
        num_node = traversals.size();
        for(auto &trav:traversals) {
            prs[trav.first].initialized = false;
            prs[trav.first].placed      = false;
            prs[trav.first].routed      = false;
        }
    }
    
    bool PandR::run() noexcept{
        init_primary_input_positions();
        
        std::size_t i=0;
        while(i<num_node) {
            
            //std::cout << "=====================================" << std::endl;
            //std::cout << "current i is " << i << std::endl;
            auto &node = traversals[i].first;
            auto &fanins = traversals[i].second;
            
            if(!prs[node].initialized) {
                
                //std::cout << "init " << i << " " << node << std::endl;
                auto size_xy = chessboard().size();
                std::map<Position,std::size_t> position_occurrence;
                
                for( const auto&fanin : fanins ) {
                    Position &fanin_pos = prs[fanin].position();
                    
                    auto &x = fanin_pos.first;
                    auto &y = fanin_pos.second;
                    auto distance = network().level_distance(node, fanin) * LEVEL_MULTIPLIER;
                    
                    uint64_t floor_i = (x-distance >= 0)? x-distance : 0;
                    uint64_t floor_j = (y-distance >= 0)? y-distance : 0;
                    uint64_t top_i = (x+distance < size_xy-1)? x+distance : size_xy-1;
                    uint64_t top_j = (y+distance < size_xy-1)? y+distance : size_xy-1;
                    
                    for(auto i=floor_i; i<=top_i; i++){
                        for(auto j=floor_j; j<=top_j; j++){
                            Position cur_pos = {i,j};
                            auto route = chessboard().find_route_between(fanin_pos, cur_pos);
                            
                            if(route.has_value() && route.value().size() == (distance+1)) {
                                position_occurrence[cur_pos]++;
                            }
                        }
                    }
                }
                
                std::vector<Position> positions;
                for(auto &[pos, occurrences] : position_occurrence) {
                    if(occurrences == fanins.size()) {
                        positions.push_back(pos);
                    }
                }
                
                if(positions.empty()) {
                    //std::cout << i << " " << node << "is empty" << std::endl;
                    i--;
                    continue;
                } else {
                    prs[node].positions = positions;
                    prs[node].index     = -1;
                    prs[node].initialized = true;
                }
            }
            
            if(prs[node].routed) {
                for(auto &route: prs[node].routings) {
                    chessboard().unwire_route(route);
                }
                prs[node].routings.clear();
                prs[node].routed = false;
            }
            
            if(prs[node].placed) {
                chessboard().unplace_node(prs[node].position());
                prs[node].placed = false;
            }
            
            while(!prs[node].exhausted()) {
                assert(!prs[node].placed);
                
                ++prs[node].index;
                if(chessboard().place_node(prs[node].position(), node)) {
                    prs[node].placed = true;
                    
                    prs[node].routed = true;
                    for(auto &fanin : fanins) {
                        auto route = chessboard().find_route_between(prs[fanin].position(), prs[node].position()).value();
                        
                        if(chessboard().wire_route(route)) {
                            prs[node].routings.push_back(route);
                        } else {
                            for(auto &route : prs[node].routings) {
                                chessboard().unwire_route(route);
                            }
                            prs[node].routings.clear();
                            prs[node].routed = false;
                            prs[node].placed = false;
                            break;
                        }
                    }
                    
                    if(prs[node].placed && prs[node].routed) {
                        break;
                    } 
                }
            }
            
            if(prs[node].placed && prs[node].routed) {
                //std::cout << "pr success " << i << " " << node << std::endl;
                ++i;
            } else {
                if(i == 0) {
                    return false;
                }
                if(network().is_pi(node)) {
                    prs[node].index = -1;
                } else {
                    prs[node].positions.clear();
                    assert(prs[node].routings.empty());
                    prs[node].initialized = false;
                }
                //std::cout << "pr failure " << i << " " << node << std::endl;
                --i;
            }
        }
        return true;
    }
        
    void PandR::init_primary_input_positions() noexcept {
        auto primary_inputs = network().nodes_at_level(0);
        auto middle = chessboard().size() / 2;
        
        uint64_t initial_distance = 1;
        std::vector<Position> initial_positions;
        while(true) {
            initial_positions = std::move( chessboard().neighbours({middle, middle}, initial_distance) );
            if(primary_inputs.size() <= initial_positions.size()) break;
            else ++initial_distance;
        }
        
        for(auto &pi:primary_inputs) {
            prs[pi].positions = initial_positions;
            prs[pi].index   = -1;
            prs[pi].initialized = true;
        }
    }
    
    void PandR::pr_result() noexcept {
        std::cout << "Area: " << chessboard().compute_layout_area() << std::endl;
        for(auto &trav: traversals) {
            std::cout << "Node " << trav.first << " at " << prs[trav.first].position() << std::endl;
            if(!trav.second.empty()) {
                
                for(int i=0; i<trav.second.size(); ++i) {
                    std::cout << trav.second[i] << "=>" << trav.first << " : ";
                    for(auto &pos: prs[trav.first].routings[i]) {
                        std::cout << pos << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }
        
    }
}
