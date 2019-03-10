#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <Area.hpp>

using namespace pandr::area;
using namespace Catch;


TEST_CASE("Area.hpp testing" "[area]"){
	Area area;
	REQUIRE(area.get() == 0);

	SECTION("Area 4x4 testing"){
		area.set(0,0);
		area.set(3,3);
		REQUIRE(area.get() == 16);
	}

	SECTION("Area unset testing"){
		area.set(0,0);
		area.set(3,3);
		area.unset(0,0);
		area.unset(3,3);
		REQUIRE(area.get() == 0);
		
	}

	SECTION("Area unset testing"){
		area.set(0,0);
		area.set(3,3);
		area.unset(0,0);
		area.unset(3,3);
		REQUIRE(area.get() == 0);
		
	}

	SECTION("Area 12x20 testing"){
		area.set(5,2);
		area.set(16,21);
		REQUIRE(area.get() == 240);
	}

	SECTION("Negative values testing"){
		/* The signs should be ignored */
		area.set(-1,2);
		area.set(16,-1);
		REQUIRE(area.get() == 32);

		area.unset(-1,2);
		area.unset(16,-1);
		REQUIRE(area.get() == 0);

		area.set(1,-2);
		area.set(-16,1);
		REQUIRE(area.get() == 32);

		area.unset(1,-2);
		area.unset(-16,1);
		REQUIRE(area.get() == 0);
	}
}
