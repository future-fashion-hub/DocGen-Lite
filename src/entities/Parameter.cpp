#include "entities/Parameter.h"

#include <utility>

Parameter::Parameter(std::string name, std::string type)
    : name_(std::move(name)), type_(std::move(type)) {}

const std::string& Parameter::getName() const {
    return name_;
}

const std::string& Parameter::getType() const {
    return type_;
}
