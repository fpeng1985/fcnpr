//
// Created by fpeng on 2020/6/14.
//

#ifndef ROPPER_CELL_H
#define ROPPER_CELL_H

#include <optional>
#include "Types.h"
//#define private public

namespace fcnpr {

    static const uint8_t MAX_CELL_WEIGHT{6};
    static const uint8_t WIRE_WEIGHT{2};
    static const uint8_t NODE_WEIGHT{4};

    class Cell {
    public:
        Cell() = default;

        bool put_node(Node node) noexcept;
        void unput_node() noexcept;
        bool put_wire() noexcept;
        void unput_wire() noexcept;

        inline bool empty() const noexcept {
            return (current_weight==0);
        }
        inline bool has_node() const noexcept {
            return node.has_value();
        }

    private:
        std::optional<Node> node;
        uint8_t current_weight{0};
        uint8_t wire_count{0};
    };

}


#endif //ROPPER_CELL_H
