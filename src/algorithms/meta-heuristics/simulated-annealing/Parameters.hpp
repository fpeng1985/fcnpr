#pragma once
namespace meta::sa {
	template<typename C, typename S>
	struct Parameters {
		struct Current {
			uint32_t transitions{};
			double temperature{};
			S solution;
		};
		Current current;
		std::function<double(void)> temperature;
		std::function<uint32_t(void)> transition;
		std::function<bool(void)> stop;
		std::function<void(S&,S&)> callback;
	};
}
