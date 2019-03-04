#pragma once
namespace meta::sa {
	template<typename C, typename S>
	struct Parameters {
		struct Current {
			uint32_t transitions{};
			double temperature{};
		};
		Current current;
		std::function<double(void)> temperature;
		std::function<uint32_t(void)> transition;
		std::function<bool(S const&)> stop;
	};
}
