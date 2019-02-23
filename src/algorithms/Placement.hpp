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
			Placement(Slots const& slots, uint64_t const id) noexcept;
			Placement(Slots&& slots, uint64_t const id) noexcept;
			Placement(Placement const& src) noexcept;
			Placement(Placement&& src) noexcept;
			uint64_t id() const;
			Slot const& current() const noexcept;
			Slot const& random() const;
			void update(Slot&& slot);
			uint64_t available();
		/* operators */
			Placement& operator=(Placement const& lhs) noexcept;
			Placement& operator=(Placement&& lhs) noexcept;
	};

	Placement::Placement(Slots const& slots, uint64_t const id) noexcept
		: slots(std::make_unique<Slots>(slots))
		, identifier(id)
		, rgn(rd())
		, dist(0,slots.size()-1)
	{
	}

	Placement::Placement(Slots&& slots, uint64_t const id) noexcept
		: slots(std::make_unique<Slots>(std::move(slots)))
		, identifier(id)
		, rgn(rd())
		, dist(0,slots.size()-1)
	{
	}

	Placement::Placement(Placement const& src)  noexcept
		: identifier(src.identifier)
		, curr(std::make_unique<Slot>(*(src.curr)))
		, slots(std::make_unique<Slots>(*(src.slots)))
		, rgn(rd())
		, dist(std::uniform_int_distribution<uint64_t>(0,this->available()-1))
	{
		//std::cout << "Copy 1" << std::endl;
	}

	Placement::Placement(Placement&& src) noexcept
		: identifier(src.identifier)
		, curr(std::move(src.curr))
		, slots(std::move(src.slots))
		, rgn(rd())
		, dist(std::uniform_int_distribution<uint64_t>(0, this->available()-1))
	{
		//std::cout << "Move 1" << std::endl;
	}

	Placement& Placement::operator=(Placement const& lhs) noexcept {
		//std::cout << "Copy 2" << std::endl;
		this->identifier = lhs.identifier;
		this->curr = std::make_unique<Slot>(*(lhs.curr));
		this->slots = std::make_unique<Slots>(*(lhs.slots));
		return (*this);
	}

	Placement& Placement::operator=(Placement&& lhs) noexcept {
		//std::cout << "Move 2" << std::endl;
		this->identifier = lhs.identifier;
		this->curr = std::move(lhs.curr);
		this->slots = std::move(lhs.slots);
		return (*this);
	}

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
} /* pandr::algorithm namespace */
