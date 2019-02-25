#pragma once
#include <Exception.hpp>
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>
#include <Cell.hpp>
#include <memory>

namespace pandr::algorithm {
	class Placement {
		private:
			uint64_t identifier;
			std::unique_ptr<Slot> curr;
			std::unique_ptr<Slots> slots;
			mutable std::random_device rd;
			mutable std::mt19937 rgn;
			mutable std::uniform_int_distribution<uint64_t> dist;
		public:
		/* Constructors */
			Placement(Slot const& curr, Slots const& slots, uint64_t const id) noexcept;
			Placement(Placement const& src) noexcept;
			Placement(Placement&& src) noexcept;
		/* Methods */
			uint64_t id() const;
			Slot const& current() const noexcept;
			Slot const& random() const;
			void update(Slot&& slot);
			uint64_t available();
		/* operators */
			Placement& operator=(Placement const& rhs) noexcept;
			Placement& operator=(Placement&& rhs) noexcept;
			bool operator==(Placement const& rhs) const noexcept;
			bool operator!=(Placement const& rhs) const noexcept;
			friend std::ostream& operator<<(std::ostream& os, Placement const& placement);
	};

	/*
	 * Constructors
	 */
	Placement::Placement(Slot const& curr, Slots const& slots, uint64_t const id) noexcept
		: identifier(id)
		, curr(std::make_unique<Slot>(curr))
		, slots(std::make_unique<Slots>(slots))
		, rgn(rd())
		, dist(0,slots.size()-1)
	{
	}

	Placement::Placement(Placement const& src)  noexcept
		: Placement(*(src.curr), *(src.slots), src.identifier)
	{
		//std::cout << "Copy 1" << std::endl;
	}

	Placement::Placement(Placement&& src) noexcept
		: identifier(src.identifier)
		, curr(std::move(src.curr))
		, slots(std::move(src.slots))
		, rgn(rd())
		, dist(0,slots->size()-1)
	{
		//std::cout << "Move 1" << std::endl;
	}

	/*
	 * Methods
	 */
	uint64_t Placement::id() const {
		return this->identifier;
	}

	Slot const& Placement::current() const noexcept {
		return *(this->curr);
	}

	Slot const& Placement::random() const {
		auto random_index {dist(rgn)};
		return this->slots->at(random_index);
	}

	void Placement::update(Slot&& slot) {
		this->curr = std::make_unique<Slot>(slot);
	}

	uint64_t Placement::available() {
		return this->slots->size();
	}

	/*
	 * Operators
	 */
	Placement& Placement::operator=(Placement const& rhs) noexcept {
		//std::cout << "Copy 2" << std::endl;
		this->identifier = rhs.identifier;
		this->curr       = std::make_unique<Slot>(*(rhs.curr));
		this->slots      = std::make_unique<Slots>(*(rhs.slots));
		return (*this);
	}

	Placement& Placement::operator=(Placement&& rhs) noexcept {
		//std::cout << "Move 2" << std::endl;
		this->identifier = rhs.identifier;
		this->curr       = std::move(rhs.curr);
		this->slots      = std::move(rhs.slots);
		return (*this);
	}

	bool Placement::operator==(Placement const& rhs) const noexcept {
		if(this->identifier   != rhs.identifier  ||
		   this->curr->first  != rhs.curr->first ||
		   this->curr->second != rhs.curr->second) return false;

		return true;
	}

	bool Placement::operator!=(Placement const& rhs) const noexcept {
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, Placement const& placement) {
		os << "Id: " << placement.identifier << " | Pos: (" << placement.curr->first << "," << placement.curr->second << ")" << std::endl;
		return os;
	}
} /* pandr::algorithm namespace */
