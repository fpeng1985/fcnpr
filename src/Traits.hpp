#pragma once

namespace pandr::traits{

#define HAS_TRAIT(trait) \
template <typename Type>  \
class trait \
{ \
private: \
    typedef char YesType[1]; \
    typedef char NoType[2]; \
    template <typename C> static YesType& test( decltype(&C::trait) ) ; \
    template <typename C> static NoType& test(...); \
public: \
    enum { value = sizeof(test<Type>(0)) == sizeof(YesType) }; \
}; \

HAS_TRAIT(size)
HAS_TRAIT(get)
HAS_TRAIT(place)
HAS_TRAIT(unplace)
HAS_TRAIT(neighbors)
HAS_TRAIT(depth)
HAS_TRAIT(nodes_at_level)
HAS_TRAIT(node_fan_ins)

} /* pandr::traits namespace */
