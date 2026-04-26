#pragma once

#include <string>

class Parameter {
public:
    Parameter() = default;
    Parameter(std::string name, std::string type);

    const std::string& getName() const;
    const std::string& getType() const;

private:
    std::string name_;
    std::string type_;
};
