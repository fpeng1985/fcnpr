#include <fstream>
#include <cxxopts.hpp>
#include <iomanip>
#include <patterns/Use.hpp>
#include <lorina/diagnostics.hpp>
#include <alice/alice.hpp>
#include <PlacementAndRouting.hpp>
#include <BreadthFirstSearch.hpp>
#include <SimulatedAnnealing.hpp>
#include <Field.hpp>
#include <Network.hpp>
#include <json.hpp>

using namespace pandr;
using namespace pandr::field;
using namespace pandr::algorithm;
using namespace pandr::patterns;
using namespace pandr::network;

class diagnostics : public lorina::diagnostic_engine
{
public:
  void emit( lorina::diagnostic_level level, const std::string& message ) const override {
	std::cerr << "\033[31;1m * \033[mError while parsing the file " << message << std::endl;
  }
};

auto main(int argc, char* argv[]) -> int {
	using json = nlohmann::json;

	std::string ifile;
	std::string ofile;
	try{
		cxxopts::Options options("Ropper", "A P&R tool for variadic clocking scheme designs and unbalanced graphs");

		options.add_options()
			("i,input", "Input filename", cxxopts::value<std::string>())
			("o,output", "Output filename", cxxopts::value<std::string>())
			;

		auto parsed {options.parse(argc,argv)};

		ifile = parsed["i"].as<std::string>();
		ofile = parsed["o"].as<std::string>();
	}catch(std::domain_error const& e){
		std::cerr << "\033[31;1m * \033[mYou must provide the names of the input and output files" << std::endl;
	}

	Network ntk(4);
	diagnostics diag;
	auto const result = lorina::read_verilog(ifile, mockturtle::verilog_reader(ntk), &diag);

	if(result == lorina::return_code::success){
		std::ofstream outFile(ofile, std::ios_base::trunc);
		if(!outFile.good()){
			std::cerr << "\033[31;1m * \033[mFailure to open the output file for writing" << std::endl;
			return EXIT_FAILURE;
		}
		Field<BreadthFirstSearch,30,use::generator> field;
		PlacementAndRouting<decltype(field), Network, SimulatedAnnealing> pandr(ntk);
		
		auto merge_json = [](const json &a, const json &b) -> json {
			json result = a;
			json tmp = b;
			for (json::iterator it = tmp.begin(); it != tmp.end(); ++it){
				std::string key {it.key()};
				result[key.c_str()] = it.value();
			}
			return result;
		};

		/* Json Header */
		auto jh = R"(
			{
				"Ropper": "A P&R tool for variadic clocking scheme designs and unbalanced graphs",
				"Structure": {
					"Level 0": {
						"Primary Input": "Position"
					},
					"Level 1..n": {
						"Fanout": {
							"Fanin": "Path"
						}
					}
				}
			}
		)"_json;
		/***************/

		auto jr = pandr.run();

		auto j {merge_json(jh,jr)};

		outFile << std::setw(2) << j << std::endl;
	}else{
		std::cerr << "\033[31;1m * \033[mFailure to load the graph from input file" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
