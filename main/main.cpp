#include <fstream>
#include <iomanip>
#include <cxxopts.hpp>
#include <lorina/diagnostics.hpp>

#include "Network.h"
#include "ChessBoard.h"
#include "PandR_LevelPR.h"
#include "PandR_TopoOrder.h"

using namespace fcnpr;

/*
class diagnostics : public lorina::diagnostic_engine
{
public:
  void emit( lorina::diagnostic_level level, const std::string& message ) const override {
	std::cerr << "\033[31;1m * \033[mError while parsing the file " << message << std::endl;
  }
};
*/

int main(int argc, char* argv[]) {
	std::string ifile;
	std::string ofile;
	try{
		cxxopts::Options options("fcnpr", "A P&R tool for FCN circuit");

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

	if(network().parse(ifile)){
		std::ofstream outFile(ofile, std::ios_base::trunc);
		if(!outFile.good()){
			std::cerr << "\033[31;1m * \033[mFailure to open the output file for writing" << std::endl;
			return EXIT_FAILURE;
		}

		PandRLevelPR pandr;
		std::cout << "\033[34;1m * \033[mRunning Algorithm..." << std::endl;

		if(pandr.run()) {
            pandr.pr_result();
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
