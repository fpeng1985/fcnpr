#pragma once
#include <Exception.hpp>
#include <algorithms/Placement.hpp>
#include <memory>

namespace pandr::algorithm {
	typedef std::vector<Placement>::iterator placement_iterator;
	typedef std::vector<Placement>::const_iterator placement_const_iterator;
	template<typename Matrix>
	class Placements {
		private:
			Matrix* matrix;
			std::unique_ptr<std::vector<Placement>> placements;
			static uint32_t copies;
			static uint32_t moves;
		public:
		/* Constructors */
			Placements(Matrix& matrix) noexcept;
			Placements(Placements<Matrix> const& src) noexcept;
			Placements(Placements<Matrix>&& src) noexcept;
		/* Iterators */
			placement_iterator begin();
			placement_iterator end();
			placement_const_iterator cbegin() const;
			placement_const_iterator cend() const;
		/* Methods */
		private:
			void placeMatrix(Placements const& src);
			void unplaceMatrix(Placements const& src);
			bool isCurrentInMatrix(Placements const& src);
		public:
			placement_const_iterator find(uint64_t identifier) const noexcept;
			void add(Placement&& placement) noexcept;
			uint64_t size() const noexcept;
			std::pair<uint32_t,uint32_t> stats() const noexcept;
		/* Operators */
			Placements<Matrix>& operator=(Placements<Matrix>&& rhs) noexcept;
			Placements<Matrix>& operator=(Placements<Matrix> const& rhs) noexcept;
			bool operator==(Placements<Matrix> const& rhs) const noexcept;
			bool operator!=(Placements<Matrix> const& rhs) const noexcept;
			template<typename _Matrix> friend std::ostream& operator<<(std::ostream& os, Placements<_Matrix> const& placements);
		/* exceptions */
			class invalid_identifier : public pandr::exception{using exception::exception;};
			class remove_failure : public pandr::exception{using exception::exception;};
	};

	template<typename Matrix>
	uint32_t Placements<Matrix>::copies{0};
	template<typename Matrix>
	uint32_t Placements<Matrix>::moves{0};

	/*
	 * Constructors
	 */
	template<typename Matrix>
	Placements<Matrix>::Placements(Matrix& matrix) noexcept
		: matrix(&matrix)
		, placements(std::make_unique<std::vector<Placement>>())
	{
	}

	template<typename Matrix>
	Placements<Matrix>::Placements(Placements const& src) noexcept
		: matrix(src.matrix)
		, placements(std::make_unique<std::vector<Placement>>())
	{
		std::copy(std::begin(*(src.placements)), std::end(*(src.placements)), std::back_inserter(*(this->placements)));
		++this->copies;
	}

	template<typename Matrix>
	Placements<Matrix>::Placements(Placements&& src) noexcept
		: matrix(src.matrix)
		, placements(std::move(src.placements))
	{
		++this->moves;
	}

	/*
	 * Iterators
	 */
	template<typename Matrix>
	placement_iterator Placements<Matrix>::begin() {
		return std::begin(*this->placements);
	}

	template<typename Matrix>
	placement_iterator Placements<Matrix>::end() {
		return std::end(*this->placements);
	}

	template<typename Matrix>
	placement_const_iterator Placements<Matrix>::cbegin() const {
		return std::cbegin(*this->placements);
	}

	template<typename Matrix>
	placement_const_iterator Placements<Matrix>::cend() const {
		return std::cend(*this->placements);
	}

	/*
	 * Methods
	 */

	template<typename Matrix>
	void Placements<Matrix>::placeMatrix(Placements const& src) {
		std::for_each(src.cbegin(), src.cend(), [&](auto const& p){
			this->matrix->place(p.current().first, p.current().second, p.id());
		});
	}

	template<typename Matrix>
	void Placements<Matrix>::unplaceMatrix(Placements const& src) {
		std::for_each(src.cbegin(), src.cend(), [&](auto const& p){
			this->matrix->unplace(p.current().first, p.current().second);
		});
	}

	template<typename Matrix>
	bool Placements<Matrix>::isCurrentInMatrix(Placements const& src) {
		if(src.cbegin() == src.cend()) return false;

		bool check{true};
		std::for_each(src.cbegin(), src.cend(), [&](auto const& p){
			if(!this->matrix->isPlaced(p.current().first, p.current().second)){
				check = false;
			}
		});
		return check;
	}

	template<typename Matrix>
	placement_const_iterator Placements<Matrix>::find(uint64_t identifier) const noexcept {
		return std::find_if(this->cbegin(), this->cend(), [&](auto const& placement){
			return placement.id() == identifier;
		});
	}

	template<typename Matrix>
	void Placements<Matrix>::add(Placement&& placement) noexcept {
		this->matrix->place(placement.current().first, placement.current().second, placement.id());
		this->placements->emplace_back(std::move(placement));
	}

	template<typename Matrix>
	uint64_t Placements<Matrix>::size() const noexcept {
		return this->placements->size();
	}

	template<typename Matrix>
	std::pair<uint32_t,uint32_t> Placements<Matrix>::stats() const noexcept{
		return std::make_pair(this->copies, this->moves);
	}

	/*
	 * Operators
	 */
	template<typename Matrix>
	Placements<Matrix>& Placements<Matrix>::operator=(Placements<Matrix>&& rhs) noexcept {
		if(this->isCurrentInMatrix(rhs)) {
			this->matrix = rhs.matrix; rhs.matrix=nullptr;
			this->placements = std::move(rhs.placements);
		}else{
			this->unplaceMatrix(*this);
			this->matrix = rhs.matrix; rhs.matrix=nullptr;
			this->placements = std::move(rhs.placements);
			this->placeMatrix(*this);
		}
		++this->moves;
		return (*this);
	}

	template<typename Matrix>
	Placements<Matrix>& Placements<Matrix>::operator=(Placements<Matrix> const& rhs) noexcept {
		if(this->isCurrentInMatrix(rhs)){
			this->matrix = rhs.matrix;
			this->placements->clear();
			std::copy(std::begin(*(rhs.placements)), std::end(*(rhs.placements)), std::back_inserter(*(this->placements)));
		}else{
			this->unplaceMatrix(*this);
			this->matrix = rhs.matrix;
			this->placements->clear();
			std::copy(std::begin(*(rhs.placements)), std::end(*(rhs.placements)), std::back_inserter(*(this->placements)));
			this->placeMatrix(*this);
		}
		++this->copies;
		return (*this);
	}

	template<typename Matrix>
	bool Placements<Matrix>::operator==(Placements<Matrix> const& rhs) const noexcept {
		if(rhs.size() != this->size()) return false;

		for(auto it{rhs.cbegin()}; it!=rhs.cend(); ++it){
			auto const& search {this->find(it->id())};
			if(search == this->cend() || *search != *it) return false;
		}

		return true;
	}

	template<typename Matrix>
	bool Placements<Matrix>::operator!=(Placements<Matrix> const& rhs) const noexcept {
		return !(*this == rhs);
	}

	template<typename _Matrix>
	std::ostream& operator<<(std::ostream& os, Placements<_Matrix> const& placements) {
		char const* prefix {"\033[1;32m * \033[0m"};
		std::cout << "Placements:" << std::endl;
		for(auto const& placement : *(placements.placements)){
			os << prefix << placement << std::endl;
		}
		return os;
	}

} /* pandr::algorithm namespace */
