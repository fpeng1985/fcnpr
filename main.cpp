#include <iomanip>
#include <patterns/Use.hpp>
#include <alice/alice.hpp>
#include <PlacementAndRouting.hpp>
#include <BreadthFirstSearch.hpp>
#include <SimulatedAnnealing.hpp>
#include <Field.hpp>
#include <Network.hpp>


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

using namespace pandr;
using namespace pandr::field;
using namespace pandr::algorithm;
using namespace pandr::patterns;
using namespace pandr::network;

auto main() -> int {
	Field<BreadthFirstSearch,30,use::generator> field;
	PlacementAndRouting<decltype(field), Network, SimulatedAnnealing> pandr;

	auto result = pandr.run();

	std::cout << std::setw(4) << result << std::endl;

	return 0;
}
