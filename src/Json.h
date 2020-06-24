#pragma once
#include <json.hpp>

namespace fcnpr {

	class Json : public nlohmann::json {
		public:
			Json();
	};

	Json::Json() : nlohmann::json(
			R"(
				{
					"Ropper": "A P&R tool for variadic clocking scheme designs and unbalanced graphs",
					"Structure": {
						"pandr": {
							"Level 0": {
								"Primary Input": "Position"
							},
							"Level 1..n": {
								"Fanout": {
									"Fanin": "Path including fanin to fanout positions"
								}
							}
						}
					}
				}
			)"_json
		)
	{}

}
