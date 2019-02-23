#pragma once
#include <exception>
#include <algorithms/Placement.hpp>
#include <memory>

namespace pandr::algorithm {
	class Placements {
		private:
			std::unique_ptr<std::vector<Placement>> placements;
		public:
			Placements() noexcept;
			Placements(Placements const& src) noexcept;
			Placements(Placements&& src) noexcept;
			virtual void add(Placement const& placement) noexcept;
			void add(Placement&& placement) noexcept;
			Placement& at(uint64_t i);
			Placement const& at(uint64_t i) const;
			Placement& search(uint64_t identifier);
			Placement const& search(uint64_t identifier) const;
			void update(Placements& placements);
			uint64_t size() const noexcept;
			Placements& operator=(Placements&& rhs) noexcept;
			Placements& operator=(Placements const& rhs) noexcept;
			bool operator==(Placements const& rhs) noexcept;
			bool operator!=(Placements const& rhs) noexcept;
		/* exceptions */
			class invalid_identifier : public pandr::exception{using exception::exception;};
	};

	Placements::Placements() noexcept
		: placements(std::make_unique<std::vector<Placement>>())
	{
	}

	Placements::Placements(Placements const& src) noexcept
		: placements(std::make_unique<std::vector<Placement>>())
	{
		//std::cout << "Copy @ " << __func__ << std::endl;
		std::copy(std::begin(*(src.placements)), std::end(*(src.placements)), std::back_inserter(*(this->placements)));
	}

	Placements::Placements(Placements&& src) noexcept {
		//std::cout << "Move @ " << __func__ << std::endl;
		std::swap(this->placements, src.placements);
		src.placements->clear();
	}

	void Placements::add(Placement const& placement) noexcept {
		this->placements->push_back(placement);
	}

	void Placements::add(Placement&& placement) noexcept {
		this->placements->push_back(placement);
	}

	Placement& Placements::at(uint64_t i) {
		return this->placements->at(i);
	}

	Placement const& Placements::at(uint64_t i) const {
		return this->at(i);
	}

	Placement& Placements::search(uint64_t identifier) {
		auto& placements {*this->placements};
		auto search = std::find_if(std::begin(placements), std::end(placements), [&](auto const& placement){
				return placement.id() == identifier;
		});

		if(search == std::end(placements)) throw invalid_identifier("Node not found with the specified identifier");

		return *search;
	}

	Placement const& Placements::search(uint64_t identifier) const {
		return search(identifier);
	}

	void Placements::update(Placements& placements) {
		auto& curr_placements {*this->placements};
		for(auto i{0}; i<placements.size(); ++i){
			auto& new_placement {placements.at(i)};
			auto& old_placement {this->search(new_placement.id())};
			old_placement = new_placement;
		}
	}

	uint64_t Placements::size() const noexcept {
		return this->placements->size();
	}
	
	Placements& Placements::operator=(Placements&& rhs) noexcept {
		//std::cout << "Move @ " << __func__ << std::endl;
		std::swap(this->placements, rhs.placements);
		rhs.placements->clear();
		return (*this);
	}

	Placements& Placements::operator=(Placements const& rhs) noexcept {
		//std::cout << "Copy @ " << __func__ << std::endl;
		this->placements->clear();
		std::copy(std::begin(*(rhs.placements)), std::end(*(rhs.placements)), std::back_inserter(*(this->placements)));
		return (*this);
	}

	bool Placements::operator==(Placements const& rhs) noexcept {
		auto sz {this->size()};

		if(rhs.size() != sz) return false;

		auto const& l_placements {*(this->placements)};
		auto const& r_placements {*(rhs.placements)};

		for(auto i{0}; i<sz; ++i){
			auto const& r_placement {rhs.at(i)};
			try{
				auto const& l_placement {this->search(r_placement.id())};
				if(r_placement != l_placement) return false;
			}catch(invalid_identifier const& e){
				return false;
			}
		}
		return true;
	}

	bool Placements::operator!=(Placements const& rhs) noexcept {
		return *this == rhs;
	}
} /* pandr::algorithm namespace */
