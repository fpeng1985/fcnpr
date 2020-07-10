//
// Created by fpeng on 2020/6/22.
//

#ifndef FCNPR_TYPES_H
#define FCNPR_TYPES_H

#include <utility>
#include <vector>
#include <set>
#include <cstdint>
#include <iostream>
#include <boost/container_hash/hash.hpp>

namespace fcnpr {
    using Node       = uint64_t;
    using Coordinate = uint64_t;
    using Position   = std::pair<Coordinate, Coordinate>;
    using Placement  = std::pair<Node, Position>;
    using Route      = std::vector<Position>;
    using Region     = std::set<Position>;
    using TimeZone   = uint8_t;
    using Level      = uint32_t;

    template<typename OStream>
    OStream& operator<<(OStream &out, const Position &pos) {
        out << "(" << pos.first << "," << pos.second << ")";
        return out;
    }
}

namespace std {
    template<>
    struct hash< std::pair<fcnpr::Node, fcnpr::Node> > {
        using argument_type = std::pair<fcnpr::Node, fcnpr::Node>;
        using result_type   = std::size_t;

        result_type operator()(argument_type const &src_tgt) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, src_tgt.first);
            boost::hash_combine(seed, src_tgt.second);
            return seed;
        }
    };
}

//#endif
#endif //ROPPER_TYPES_H
