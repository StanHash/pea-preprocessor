#ifndef PARAMETER_INTERFACE_H
#define PARAMETER_INTERFACE_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

#include "lib/string_building.h"
#include "lib/filesystem.h"

// TODO: move this elsewhere
struct option_error : public std::runtime_error {
	option_error(const std::string& optionName)
		: std::runtime_error(string_builder("option \"", optionName, "\" doesn't exist")()) {}
};

struct option_operation_error : public std::runtime_error {
	option_operation_error(const std::string& optionName, const std::string& type, const std::string& operation)
		: std::runtime_error(string_builder("option \"", optionName, "\" (", type, ") doesn't support operation '", operation, "'")()) {}
};

class parameter_interface {
public:
	struct parameter_handler_base {
		virtual ~parameter_handler_base() {}

		virtual void set(const std::string& string) {}
		virtual void add(const std::string& string) {}

		virtual bool can_set() const { return false; }
		virtual bool can_add() const { return false; }

		virtual const char* type_name() const = 0;
		virtual std::string get() const = 0;
	};

	template<typename IntType>
	struct int_parameter_handler : public parameter_handler_base {
		int_parameter_handler(IntType& value)
			: mpValue(&value) {}

		void set(const std::string& string) { (*mpValue) =  (IntType) std::stoll(string); }
		void add(const std::string& string) { (*mpValue) += (IntType) std::stoll(string); }

		bool can_set() const { return true; }
		bool can_add() const { return true; }

		const char* type_name() const { return "integer"; }
		std::string get() const { return std::to_string(*mpValue); }

	private:
		IntType* const mpValue;
	};

	struct bool_parameter_handler : public parameter_handler_base {
		bool_parameter_handler(bool& ref)
			: mpValue(&ref) {}

		void set(const std::string& string) {
			if (string == "true")
				(*mpValue) = true;
			else if (string == "false")
				(*mpValue) = false;
			else
				throw std::runtime_error("tried to set bool argument to neither true nor false");
		}

		bool can_set() const { return true; }

		const char* type_name() const { return "boolean"; }
		std::string get() const { return (*mpValue) ? "true" : "false"; }

	private:
		bool* const mpValue;
	};

	template<typename CharType>
	struct string_parameter_handler : public parameter_handler_base {
		string_parameter_handler(std::basic_string<CharType>& ref)
			: mpValue(&ref) {}

		void set(const std::string& string) { (*mpValue) = std::basic_string<CharType>(string.begin(), string.end()); }
		bool can_set() const { return true; }

		const char* type_name() const { return "string"; }
		std::string get() const { return string_builder("\"", std::string(mpValue->begin(), mpValue->end()), "\"")(); }

	private:
		std::basic_string<CharType>* const mpValue;
	};

	template<typename CharType>
	struct string_list_parameter_handler : public parameter_handler_base {
		string_list_parameter_handler(std::vector<std::basic_string<CharType>>& ref)
			: mpValue(&ref) {}

		void set(const std::string& string) {
			(*mpValue) = { std::basic_string<CharType>(string.begin(), string.end()) };
		}

		void add(const std::string& string) {
			mpValue->emplace_back(string.begin(), string.end());
		}

		bool can_set() const { return true; }
		bool can_add() const { return true; }

		const char* type_name() const { return "string-list"; }

		std::string get() const {
			string_builder builder("[");

			for (auto it = mpValue->begin(); it != mpValue->end(); ++it) {
				builder.append("\"", std::string(it->begin(), it->end()), "\"");

				if (std::next(it) != mpValue->end())
					builder.append(", ");
			}

			builder.append("]");
			return builder();
		}

	private:
		std::vector<std::basic_string<CharType>>* const mpValue;
	};

	struct path_parameter_handler : public parameter_handler_base {
		path_parameter_handler(fs::path& ref)
			: mpValue(&ref) {}

		void set(const std::string& string) { (*mpValue) = string; }
		bool can_set() const { return true; }

		const char* type_name() const { return "path"; }
		std::string get() const { return mpValue->empty() ? "<no path>" : mpValue->string(); }

	private:
		fs::path* const mpValue;
	};

	struct path_list_parameter_handler : public parameter_handler_base {
		path_list_parameter_handler(std::vector<fs::path>& ref)
			: mpValue(&ref) {}

		void set(const std::string& string) { (*mpValue) = { string }; }
		bool can_set() const { return true; }

		void add(const std::string& string) { mpValue->emplace_back(string); }
		bool can_add() const { return true; }

		const char* type_name() const { return "path-list"; }

		std::string get() const {
			string_builder builder("[");

			for (auto it = mpValue->begin(); it != mpValue->end(); ++it) {
				builder.append("\"", it->string(), "\"");

				if (std::next(it) != mpValue->end())
					builder.append("; ");
			}

			builder.append("]");
			return builder();
		}

	private:
		std::vector<fs::path>* const mpValue;
	};

public:
	void register_parameter(std::string&& name, std::unique_ptr<parameter_handler_base>&& handler) {
		mParamHandlers.emplace(std::move(name), std::move(handler));
	}

	void set_value(const std::string& name, const std::string& value);
	void add_value(const std::string& name, const std::string& value);

	void print_values(std::ostream& out) const;

public:
	static std::unique_ptr<parameter_handler_base> make_bool_handler(bool& b) {
		return std::unique_ptr<parameter_handler_base>(new bool_parameter_handler(b));
	}

	template<typename IntType>
	static std::unique_ptr<parameter_handler_base> make_int_handler(IntType& i) {
		return std::unique_ptr<parameter_handler_base>(new int_parameter_handler<IntType>(i));
	}

	template<typename CharType>
	static std::unique_ptr<parameter_handler_base> make_string_handler(std::basic_string<CharType>& s) {
		return std::unique_ptr<parameter_handler_base>(new string_parameter_handler<CharType>(s));
	}

	template<typename CharType>
	static std::unique_ptr<parameter_handler_base> make_string_list_handler(std::vector<std::basic_string<CharType>>& v) {
		return std::unique_ptr<parameter_handler_base>(new string_list_parameter_handler<CharType>(v));
	}

	static std::unique_ptr<parameter_handler_base> make_path_handler(fs::path& p) {
		return std::unique_ptr<parameter_handler_base>(new path_parameter_handler(p));
	}

	static std::unique_ptr<parameter_handler_base> make_path_list_handler(std::vector<fs::path>& p) {
		return std::unique_ptr<parameter_handler_base>(new path_list_parameter_handler(p));
	}

private:
	std::map<std::string, std::unique_ptr<parameter_handler_base>> mParamHandlers;
};

#endif // OPTION_HANDLER_H
