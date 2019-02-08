#pragma once
#include <limits>
#include <string>

namespace pandr {
	template<typename T>
	class Cell{
		private:
			T node;
			bool has_node;
			uint8_t clock_zone;
			uint8_t const weight_max;
			uint8_t const weight_wire;
			uint8_t const weight_node;
		public:
			Cell(T node) noexcept;
			Cell(T node, uint8_t clock_zone);
			bool place(const T& node) noexcept;
			bool unplace() noexcept;
			bool empty()  const noexcept;
			T const& get() const noexcept;
			uint8_t getZone() const noexcept;
			uint8_t getFactor() const noexcept;
			void setZone(uint8_t clock_zone);
		/*Operators*/
			template<typename U> friend std::ostream& operator<<(std::ostream &out, Cell<U>& cell);
		/*Exception Classes*/
			class InvalidNode : public std::exception {
				private:
					std::string message;
				public:
					InvalidNode(char const* message) noexcept;
					virtual char const *what() const noexcept override;
			};
	};

	template<typename T>
	Cell<T>::Cell(T node) noexcept
		: Cell(node, 0)
	{
	}

	template<typename T>
	Cell<T>::Cell(T node, uint8_t clock_zone)
		: node(node)
		, has_node(true)
		, clock_zone(clock_zone)
		, weight_max(5)
		, weight_wire(1)
		, weight_node(4)
	{
	}

	template<typename T>
	bool Cell<T>::place(const T& node) noexcept {
		if(!this->empty()) return false;
		this->node = node;
		this->has_node = true;
		return true;
	}

	template<typename T>
	bool Cell<T>::unplace() noexcept {
		if(this->empty()) return false;
		this->has_node = false;
		return true;
	}

	template<typename T>
	bool Cell<T>::empty() const noexcept {
		return has_node;
	}

	template<typename T>
	T const& Cell<T>::get() const noexcept {
		if(has_node){
			return this->node;
		}else{
			throw InvalidNode("\033[1;33m*\033[0m \033[1;31mException\033[0m: Trying to access cell without a node");
		}
	}

	template<typename T>
	uint8_t Cell<T>::getZone() const noexcept {
		return this->clock_zone;
	}

	template<typename T>
	uint8_t Cell<T>::getFactor() const noexcept {
		return this->occupation_factor;
	}

	template<typename T>
	void Cell<T>::setZone(uint8_t clock_zone){
		this->clock_zone = clock_zone;
	}

	/*
	 * Operators
	 */
	template<typename U>
	std::ostream& operator<<(std::ostream &out, Cell<U>& cell) {
		out << cell.node;
		return out;
	}
	
	 /*
	  * Exception Classes
	  */
	template<typename T>
	Cell<T>::InvalidNode::InvalidNode(char const* message) noexcept {
		this->message = message;
	}

	template<typename T>
	char const *Cell<T>::InvalidNode::what() const noexcept {
		return this->message.c_str();
	}

} /* pandr namespace */
