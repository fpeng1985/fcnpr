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

ALICE_ADD_STORE(Pandr_ptr, "pandr", "p", "PANDR", "PANDRs" );

ALICE_DESCRIBE_STORE( Pandr_ptr, pandr ){
  return "A P&R data structure";
}

ALICE_PRINT_STORE_STATISTICS( Pandr_ptr, os, pandr ){
  os << "Time for P&R: " << pandr->duration() << "ms" << std::endl;
}

ALICE_LOG_STORE_STATISTICS( Pandr_ptr, pandr ){
  return pandr->duration();
}

class pandr_command : public command {

	private:
		std::string p_alg;
		std::string r_alg;
	public:
		explicit pandr_command( const environment::ptr& env )
			: command( env, "Circuit Placement and Routing" )
		{
			//opts.add_option( "-p", p_alg, "Placement algorithm" )->required();
			//opts.add_option( "-r", r_alg, "Routing algorithm" )->required();
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
			pr->run();
			env->store<Pandr_ptr>().extend() = pr;
			env->set_default_option("pandr");
		}
};

ALICE_ADD_COMMAND( pandr, "Placement And Routing" )

ALICE_WRITE_FILE( Pandr_ptr, json, obj, filename, cmd ){
  std::ofstream outFile(filename, std::ios_base::trunc);
  outFile << std::setw(4) << obj->json() << std::endl;
}

} // namespace alice
