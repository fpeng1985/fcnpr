#include <fstream>
#include <cxxopts.hpp>
#include <iomanip>
#include <patterns/Use.hpp>
#include <lorina/diagnostics.hpp>
#include <PlacementAndRouting.hpp>
#include <BreadthFirstSearch.hpp>
#include <DynamicProgramming.hpp>
#include <SimulatedAnnealing.hpp>
#include <Field.hpp>
#include <Network.hpp>

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
		
		std::cout << "\033[34;1m * \033[mRunning Algorithm..." << std::endl;

		pandr.run();
		auto j {pandr.json()};

		std::cout << "\033[1;34m * \033[0mTime for P&R: " << pandr.duration() << "ms" << std::endl;

		outFile << std::setw(4) << j << std::endl;
	}else{
		std::cerr << "\033[31;1m * \033[mFailure to load the graph from input file" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
