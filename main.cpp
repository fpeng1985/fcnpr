#include <alice/alice.hpp>
#include <random>
#include <set>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <Field.hpp>
#include <lorina/aiger.hpp>
#include <patterns/Use.hpp>
#include <Network.hpp>
#include <mockturtle/algorithms/reconv_cut.hpp>
#include <mockturtle/networks/mig.hpp>
#include <algorithms/BreadthFirstSearch.hpp>
#include <algorithms/I_Placement.hpp>
#include <algorithms/SimulatedAnnealing.hpp>
#include <PlacementAndRouting.hpp>
#include <Area.hpp>

using std::cout;
using std::endl;
using std::get;
using namespace pandr;
using namespace pandr::algorithm;
using namespace pandr::network;
using namespace pandr::patterns::use;
using namespace pandr::algorithm;
using namespace pandr::field;

namespace alice {
	ALICE_ADD_STORE(std::string, "Pandr", "p", "PlaceAndRoute", "PlaceAndRoute")
	ALICE_PRINT_STORE(std::string, os, element) {
		os << element << std::endl;
	}
	ALICE_COMMAND(say, "Generation", "Adds a string store"){
		auto& strings = store<std::string>();
		strings.extend() = "Hello world";
	}
}

//ALICE_MAIN();

auto main() -> int {
	Field<BreadthFirstSearch,30,generator> field;
	PlacementAndRouting<decltype(field), Network, SimulatedAnnealing> pandr;

	auto result = pandr.run();

	std::cout << result << std::endl;

	return 0;
}
