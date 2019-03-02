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
			std::unique_ptr<Region> curr;
			std::unique_ptr<Regions> regions;
			mutable std::random_device rd;
			mutable std::mt19937 rgn;
			mutable std::uniform_int_distribution<uint64_t> dist;
		public:
		/* Constructors */
			Placement(Region const& curr, Regions const& regions, uint64_t const id) noexcept;
			Placement(Placement const& src) noexcept;
			Placement(Placement&& src) noexcept;
		/* Methods */
			uint64_t id() const;
			Region const& current() const noexcept;
			Region const& random() const;
			void update(Region&& region);
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
	Placement::Placement(Region const& curr, Regions const& regions, uint64_t const id) noexcept
		: identifier(id)
		, curr(std::make_unique<Region>(curr))
		, regions(std::make_unique<Regions>(regions))
		, rgn(rd())
		, dist(0,regions.size()-1)
	{
	}

	Placement::Placement(Placement const& src)  noexcept
		: Placement(*(src.curr), *(src.regions), src.identifier)
	{
		//std::cout << "Copy 1" << std::endl;
	}

	Placement::Placement(Placement&& src) noexcept
		: identifier(src.identifier)
		, curr(std::move(src.curr))
		, regions(std::move(src.regions))
		, rgn(rd())
		, dist(0,regions->size()-1)
	{
		//std::cout << "Move 1" << std::endl;
	}

	/*
	 * Methods
	 */
	uint64_t Placement::id() const {
		return this->identifier;
	}

	Region const& Placement::current() const noexcept {
		return *(this->curr);
	}

	Region const& Placement::random() const {
		auto random_index {dist(rgn)};
		return this->regions->at(random_index);
	}

	void Placement::update(Region&& region) {
		this->curr = std::make_unique<Region>(region);
	}

	uint64_t Placement::available() {
		return this->regions->size();
	}

	/*
	 * Operators
	 */
	Placement& Placement::operator=(Placement const& rhs) noexcept {
		//std::cout << "Copy 2" << std::endl;
		this->identifier = rhs.identifier;
		this->curr       = std::make_unique<Region>(*(rhs.curr));
		this->regions      = std::make_unique<Regions>(*(rhs.regions));
		return (*this);
	}

	Placement& Placement::operator=(Placement&& rhs) noexcept {
		//std::cout << "Move 2" << std::endl;
		this->identifier = rhs.identifier;
		this->curr       = std::move(rhs.curr);
		this->regions      = std::move(rhs.regions);
		return (*this);
	}

	bool Placement::operator==(Placement const& rhs) const noexcept {
		return (this->identifier   == rhs.identifier  &&
			    this->curr->first  == rhs.curr->first &&
			    this->curr->second == rhs.curr->second
			   );
	}

	bool Placement::operator!=(Placement const& rhs) const noexcept {
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, Placement const& placement) {
		os << "Id: " << placement.identifier << " | Pos: (" << placement.curr->first << "," << placement.curr->second << ")" << std::endl;
		return os;
	}
} /* pandr::algorithm namespace */
