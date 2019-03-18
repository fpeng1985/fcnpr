/* File taken from https://github.com/msoeken/cirkit */
#pragma once
#include <alice/alice.hpp>

namespace alice
{
ALICE_ADD_FILE_TYPE_READ_ONLY( aiger, "Aiger" );
ALICE_ADD_FILE_TYPE( bench, "BENCH" );
ALICE_ADD_FILE_TYPE( verilog, "Verilog" );
ALICE_ADD_FILE_TYPE( json, "json" );
}
