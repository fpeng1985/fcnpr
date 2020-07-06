//
// Created by fpeng on 2020/6/25.
//

#include "Json.h"

namespace fcnpr {

    Json::Json() : nlohmann::json(
            /*
            R"(
				{
					"fcnpr": "A P&R tool for USE clocking scheme designs and mig graphs",
					"Structure": {
						"fcnpr": {
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
             */
    )
    {}

}
