#pragma once
#include <stack>
#include <stdexcept>
#include <vector>
#include <memory>
#include <Cell.hpp>
#include <Types.hpp>
#include <algorithms/I_Routing.hpp>
#include <Area.hpp>

namespace pandr::field {
	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	using Matrix = std::vector<std::vector<std::shared_ptr<Cell<uint64_t>>>>;

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	class Field{
		private:
			Matrix<R, size_xy, scheme> field;
			R<Field> routing_algorithm;
			pandr::area::Area field_area;
		public:
			Field();
			bool place(uint64_t i, uint64_t j, uint64_t const& item);
			bool unplace(uint64_t i, uint64_t j);
			bool setWire(Path const& path);
			void unsetWire(Path const& path);
			Cell<uint64_t>& at(uint64_t i, uint64_t j);
			Cell<uint64_t> const& at(uint64_t i, uint64_t j) const;
			uint64_t size() const noexcept;
			bool isPlaced(uint64_t i, uint64_t j) const;
			uint64_t getRelativeDistance(uint64_t x1, uint64_t y1, uint64_t  x2, uint64_t  y2);
			Path getRelativeMinPath(uint64_t  x1, uint64_t  y1, uint64_t  x2, uint64_t  y2);
			Regions neighbors(uint64_t x, uint64_t y, uint64_t distance);
			uint64_t area();
		/*Operators*/
			template<template<typename> typename _R, uint64_t _size, uint8_t (*_scheme)(uint64_t,uint64_t)>
			friend std::ostream& operator<<(std::ostream &out, Field<_R, _size, _scheme> const& field);
	};

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	Field<R,size_xy,scheme>::Field()
		: routing_algorithm(*this)
	{
		for(uint64_t i{0}; i<size_xy; ++i){
			this->field.push_back(std::vector<std::shared_ptr<Cell<uint64_t>>>(size_xy));
			for(uint64_t j{0}; j<size_xy; ++j){
				this->field.at(i).at(j) = std::make_shared<Cell<uint64_t>>(0, scheme(i,j));
				this->field.at(i).at(j)->unplace();
			}
		}
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	bool Field<R,size_xy,scheme>::place(uint64_t i, uint64_t j, uint64_t const& item){
		if(this->at(i,j).place(item)){
			this->field_area.set(i,j);
			return true;
		}
		return false;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	bool Field<R,size_xy,scheme>::unplace(uint64_t i, uint64_t j){
		if(this->at(i,j).unplace()){
			this->field_area.unset(i,j);
			return true;
		}
		return false;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	bool Field<R,size_xy,scheme>::setWire(Path const& path) {
		Path wired_path;
		for(auto it{std::begin(path)}; it!=std::end(path); ++it){
			if(it == std::begin(path)) continue;
			if((it+1) == std::end(path)) break;

			auto [i,j] = std::make_pair(it->first, it->second);

			if(!this->at(i,j).setWire()){
				this->unsetWire(wired_path);
				return false;
			}else{
				wired_path.push_back({i,j});
				this->field_area.set(i,j);
			}
		}
		return true;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	void Field<R,size_xy,scheme>::unsetWire(Path const& path) {
		for(auto it{std::begin(path)}; it!=std::end(path); ++it){
			if(it == std::begin(path)) continue;
			if((it+1) == std::end(path)) break;

			auto [i,j] = std::make_pair(it->first, it->second);

			this->at(i,j).unsetWire();
			this->field_area.unset(i,j);
		}
	}
	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	Cell<uint64_t>& Field<R,size_xy,scheme>::at(uint64_t i, uint64_t j) {
		if(i >= size_xy || j >= size_xy) throw std::range_error("\033[1;33m*\033[0m \033[1;31mException\033[0m: Trying to access out of bounds position in field @ at");

		return *(this->field.at(i).at(j));
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	Cell<uint64_t> const& Field<R,size_xy,scheme>::at(uint64_t i, uint64_t j) const {
		if(i >= size_xy || j >= size_xy) throw std::range_error("\033[1;33m*\033[0m \033[1;31mException\033[0m: Trying to access out of bounds position in field @ const at");

		return *(this->field.at(i).at(j));
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	uint64_t Field<R,size_xy,scheme>::size() const noexcept {
		return size_xy;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	bool Field<R,size_xy,scheme>::isPlaced(uint64_t i, uint64_t j) const {
		return !this->at(i,j).empty();
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	uint64_t Field<R,size_xy,scheme>::getRelativeDistance(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {
		if(x1 >= size_xy || y1 >= size_xy) throw std::range_error("\033[1;33m*\033[0m \033[1;31mException\033[0m: Trying to access out of bounds position in field @ getRelativeDistance");

		return routing_algorithm.run({x1,y1},{x2,y2}).size()-1;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	Path Field<R,size_xy,scheme>::getRelativeMinPath(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {
		return routing_algorithm.run({x1,y1},{x2,y2});
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	Regions Field<R,size_xy,scheme>::neighbors(uint64_t x, uint64_t y, uint64_t distance) {
		 //All neighbors within a distance are numerically limited within that distance
		 //In other words, the maximum search area for neighbors within a x distance is x itself.
		Regions neighbors;

		uint64_t floor_i = (x-distance >= 0)? x-distance : 0;
		uint64_t floor_j = (y-distance >= 0)? y-distance : 0;
		uint64_t top_i = (x+distance < size_xy-1)? x+distance : size_xy-1;
		uint64_t top_j = (y+distance < size_xy-1)? y+distance : size_xy-1;

		for(auto i=floor_i; i<=top_i; i++){
			for(auto j=floor_j; j<=top_j; j++){
				if(this->getRelativeDistance(x, y, i, j) == distance){
					neighbors.push_back(std::make_pair(i,j));
				}
			}
		}
		return neighbors;
	}

	template<template<typename> typename R, uint64_t size_xy, uint8_t (*scheme)(uint64_t,uint64_t)>
	uint64_t Field<R,size_xy,scheme>::area() {
		return this->field_area.get();
	}

	/*
	 * Operators
	 */
	template<template<typename> typename _R, uint64_t _size, uint8_t (*_scheme)(uint64_t,uint64_t)>
	std::ostream& operator<<(std::ostream &out, Field<_R, _size, _scheme> const& field) {
		for(auto i{0}; i<_size; ++i){
			for(auto j{0}; j<_size; ++j){
				if(field.at(i,j).hasNode()){
					out << "|\033[32m" << field.at(i,j).get() << "\033[0m|";
				}else if(field.at(i,j).getWireCount() != 0){
					out << "|" << "\ufc62" << "|";
				}else{
					out << "\033[30;1m|" << (int)field.at(i,j).getZone() << "|\033[0m";
				}
			}
			out << std::endl;
		}
		return out;
	 }

} /* pandr namespace */
