#include "parameter_interface.h"

void parameter_interface::set_value(const std::string& name, const std::string& value) {
	auto it = mParamHandlers.find(name);

	if (it == mParamHandlers.end())
		throw option_error(name);

	if (it->second->can_set())
		it->second->set(value);
	else
		throw option_operation_error(name, it->second->type_name(), "=");
}

void parameter_interface::add_value(const std::string& name, const std::string& value) {
	auto it = mParamHandlers.find(name);

	if (it == mParamHandlers.end())
		throw option_error(name);

	if (it->second->can_add())
		it->second->add(value);
	else
		throw option_operation_error(name, it->second->type_name(), "+=");
}

void parameter_interface::print_values(std::ostream& out) const {
	unsigned int typeColSize = 0;
	unsigned int nameColSize = 0;

	for (const auto& pair : mParamHandlers) {
		typeColSize = std::max<unsigned int>(typeColSize, std::strlen(pair.second->type_name()));
		nameColSize = std::max<unsigned int>(nameColSize, pair.first.size());
	}

	for (const auto& pair : mParamHandlers) {
		std::string typeName;
		typeName.reserve(typeColSize + 1);

		typeName.append(pair.second->type_name());
		typeName.append(1, ':');
		typeName.append(typeColSize - std::strlen(pair.second->type_name()), ' ');

		std::string name(pair.first);
		name.append(nameColSize - name.size(), ' ');

		out << "  " << typeName << ' ' << name << " = " << pair.second->get() << std::endl;
	}
}
