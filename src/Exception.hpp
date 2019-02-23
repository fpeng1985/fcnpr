#pragma once
#include <string>
#include <exception>

namespace pandr {
	class exception : public std::exception {
		private:
			std::string const prefix;
			std::string const message;
		public:
			exception(char const* message)
				: prefix("\033[1;32m*\033[0m \033[1;31mException\033[0m: ")
				, message(std::string(prefix + message))
			{}
			virtual char const* what() const noexcept override {
				return this->message.c_str();
			}
	};
} /* pandr::exception namespace */
