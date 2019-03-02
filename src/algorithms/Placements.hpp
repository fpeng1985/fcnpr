#pragma once
#include <Exception.hpp>
#include <algorithms/Placement.hpp>
#include <memory>

namespace pandr::algorithm {
	typedef std::vector<Placement>::iterator placement_iterator;
	typedef std::vector<Placement>::const_iterator placement_const_iterator;
	class Placements {
		private:
			std::unique_ptr<std::vector<Placement>> placements;
			static uint32_t copies;
			static uint32_t moves;
		public:
		/* Constructors */
			Placements() noexcept;
			Placements(Placements const& src) noexcept;
			Placements(Placements&& src) noexcept;
		/* Iterators */
			placement_iterator begin();
			placement_iterator end();
			placement_const_iterator cbegin() const;
			placement_const_iterator cend() const;
		/* Methods */
			placement_const_iterator find(uint64_t identifier) const noexcept;
			void add(Placement&& placement) noexcept;
			uint64_t size() const noexcept;
			std::pair<uint32_t,uint32_t> stats() const noexcept;
		/* Operators */
			Placements& operator=(Placements&& rhs) noexcept;
			Placements& operator=(Placements const& rhs) noexcept;
			bool operator==(Placements const& rhs) const noexcept;
			bool operator!=(Placements const& rhs) const noexcept;
			friend std::ostream& operator<<(std::ostream& os, Placements const& placements);
		/* exceptions */
			class invalid_identifier : public pandr::exception{using exception::exception;};
			class remove_failure : public pandr::exception{using exception::exception;};
	};

	uint32_t Placements::copies{0};
	uint32_t Placements::moves{0};

	/*
	 * Constructors
	 */
	Placements::Placements() noexcept
		: placements(std::make_unique<std::vector<Placement>>())
	{
	}

	Placements::Placements(Placements const& src) noexcept
		: placements(std::make_unique<std::vector<Placement>>())
	{
		std::copy(std::begin(*(src.placements)), std::end(*(src.placements)), std::back_inserter(*(this->placements)));
		++this->copies;
	}

	Placements::Placements(Placements&& src) noexcept {
		this->placements = std::move(src.placements);
		++this->moves;
	}

	/*
	 * Iterators
	 */
	placement_iterator Placements::begin() {
		return std::begin(*this->placements);
	}

	placement_iterator Placements::end() {
		return std::end(*this->placements);
	}

	placement_const_iterator Placements::cbegin() const {
		return std::cbegin(*this->placements);
	}

	placement_const_iterator Placements::cend() const {
		return std::cend(*this->placements);
	}

	/*
	 * Methods
	 */
	placement_const_iterator Placements::find(uint64_t identifier) const noexcept {
		return std::find_if(this->cbegin(), this->cend(), [&](auto const& placement){
			return placement.id() == identifier;
		});
	}

	void Placements::add(Placement&& placement) noexcept {
		this->placements->emplace_back(std::move(placement));
	}

	uint64_t Placements::size() const noexcept {
		return this->placements->size();
	}

	std::pair<uint32_t,uint32_t> Placements::stats() const noexcept{
		return std::make_pair(this->copies, this->moves);
	}

	/*
	 * Operators
	 */
	Placements& Placements::operator=(Placements&& rhs) noexcept {
		std::swap(this->placements, rhs.placements);
		rhs.placements->clear();
		++this->moves;
		return (*this);
	}

	Placements& Placements::operator=(Placements const& rhs) noexcept {
		this->placements->clear();
		std::copy(std::begin(*(rhs.placements)), std::end(*(rhs.placements)), std::back_inserter(*(this->placements)));
		++this->copies;
		return (*this);
	}

	bool Placements::operator==(Placements const& rhs) const noexcept {
		if(rhs.size() != this->size()) return false;

		for(auto it{rhs.cbegin()}; it!=rhs.cend(); ++it){
			auto const& search {this->find(it->id())};
			if(search == this->cend() || *search != *it) return false;
		}

		return true;
	}

	bool Placements::operator!=(Placements const& rhs) const noexcept {
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, Placements const& placements) {
		char const* prefix {"\033[1;32m * \033[0m"};
		std::cout << "Placements:" << std::endl;
		for(auto const& placement : *(placements.placements)){
			os << prefix << placement << std::endl;
		}
		return os;
	}

} /* pandr::algorithm namespace */
