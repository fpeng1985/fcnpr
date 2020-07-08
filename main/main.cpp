#include <fstream>
#include <iomanip>
#include <cxxopts.hpp>
#include <lorina/diagnostics.hpp>

#include "Network.h"
#include "ChessBoardTest.h"
#include "PlacementAndRouting.h"

using namespace fcnpr;

class diagnostics : public lorina::diagnostic_engine
{
public:
  void emit( lorina::diagnostic_level level, const std::string& message ) const override {
	std::cerr << "\033[31;1m * \033[mError while parsing the file " << message << std::endl;
  }
};

int main(int argc, char* argv[]) {
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

	diagnostics diag;
	auto const result = lorina::read_verilog(ifile, mockturtle::verilog_reader(network().get()), &diag);

	if(result == lorina::return_code::success){
		std::ofstream outFile(ofile, std::ios_base::trunc);
		if(!outFile.good()){
			std::cerr << "\033[31;1m * \033[mFailure to open the output file for writing" << std::endl;
			return EXIT_FAILURE;
		}

		PlacementAndRouting pandr;
		std::cout << "\033[34;1m * \033[mRunning Algorithm..." << std::endl;

		if(pandr.run()) {
            std::cout << "\033[1;34m * \033[0mTime for P&R: " << pandr.duration() << "ms" << std::endl;
            outFile << std::setw(4) << pandr.json() << std::endl;
		} else {
            std::cerr << "\033[31;1m * \033[mFailure to find a proper solution" << std::endl;
            return EXIT_FAILURE;
		}

	}else{
		std::cerr << "\033[31;1m * \033[mFailure to load the graph from input file" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
