#include <iomanip>
#include <fstream>
#include <PandrJson.hpp>
#include <Field.hpp>
#include <BreadthFirstSearch.hpp>
#include <DynamicProgramming.hpp>
#include <SimulatedAnnealing.hpp>
#include <PlacementAndRouting.hpp>
#include <patterns/Use.hpp>
#include <Network.hpp>
#include <mockturtle/mockturtle.hpp>

namespace alice{

using Network = pandr::network::Network<mockturtle::mig_network>;
using Field = pandr::field::Field<pandr::algorithm::DynamicProgramming,30,pandr::patterns::use::generator>;
using Pandr = pandr::PlacementAndRouting<Field, Network, pandr::algorithm::SimulatedAnnealing>;
using Pandr_ptr = std::shared_ptr<Pandr>;

struct Benchmark {
	double average_area{};
	double average_time{};
	uint32_t iterations{};
};

ALICE_ADD_STORE(Benchmark, "benchmark", "b", "benchmark", "benchmarks" );

ALICE_DESCRIBE_STORE( Benchmark, benchmark ){
  return "A benchmark of P&R";
}

ALICE_PRINT_STORE_STATISTICS( Benchmark, os, benchmark ){
  os << "Iterations: " << benchmark.iterations << std::endl;
  os << "Average area:" << benchmark.average_area << std::endl;
  os << "Average time:" << benchmark.average_time << std::endl;
}

ALICE_LOG_STORE_STATISTICS( Benchmark, benchmark ){
  return {
    {"Iterations: ", benchmark.iterations},
    {"Average area: ", benchmark.average_area},
    {"Average time: ", benchmark.average_time}
  };
}

class benchmark_command : public command {
	private:
		Benchmark benchmark;
	public:
		explicit benchmark_command( const environment::ptr& env )
			: command( env, "Circuit Placement and Routing" )
		{
			opts.add_option( "-i", this->benchmark.iterations, "Number of iterations" )->required();
		}

	protected:
		void execute() {
			auto env_mig {env->store<mig_t>()};
			if(env_mig.empty()) {
				env->err() << "\033[1;31m * \033[0mYou must load a mig or aig to execute the P&R." << std::endl;
				return;
			}

			auto mig {env_mig.current()};

			auto ntk = Network(4, *mig);
			auto pr = std::make_shared<Pandr>(ntk);

			for(auto i{0}; i<this->benchmark.iterations; ++i){
				pr->run();
				this->benchmark.average_area += pr->area();
				this->benchmark.average_time += pr->duration();
			}
			this->benchmark.average_area /= this->benchmark.iterations;
			this->benchmark.average_time /= this->benchmark.iterations;

			env->store<Benchmark>().extend() = benchmark;
			env->set_default_option("benchmark");
		}
};

ALICE_ADD_COMMAND( benchmark, "Execute a P&R benchmark" )

ALICE_WRITE_FILE( Benchmark, json, obj, filename, cmd ){
	std::ofstream outFile(filename, std::ios_base::trunc);

	pandr::Json j;
	j["iterations"] = obj.iterations;
	j["average_area"] = obj.average_area;
	j["average_time"] = obj.average_time;

	outFile << std::setw(4) << j << std::endl;
}

} // namespace alice
