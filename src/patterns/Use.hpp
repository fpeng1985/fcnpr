#pragma once

namespace pandr::patterns {
	namespace use {
		uint8_t generator(uint64_t x, uint64_t y){
			return (y % 2 != 0)?
						(
						 ((y+1)%4 != 0)? (1+(x+1)%4) : (1+(x+3)%4)
						)
					:
						(
						 (y%4 == 0)? (4-(x+3)%4) : (4-((x+1)%4))
						);
		}
		bool seq(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2){
			
		}
	}
}
