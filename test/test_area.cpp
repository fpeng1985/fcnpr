#include <Area.hpp>

int main(){
	using namespace pandr::area;
	Area area;

	area.set(0,0);
	area.set(3,3);
	if(area.get() != 16) return EXIT_FAILURE;

	area.unset(0,0);
	area.unset(3,3);
	if(area.get() != 0) return EXIT_FAILURE;

	area.unset(3,1);
	area.unset(2,2);
	if(area.get() != 0) return EXIT_FAILURE;

	area.set(5,2);
	area.set(16,21);
	if(area.get() != 240) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
