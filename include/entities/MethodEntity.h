#pragma once

#include <string>
#include <vector>

#include "entities/CodeEntity.h"
#include "entities/Parameter.h"

/**
 * @brief Model of function/method declaration.
 */
class MethodEntity : public CodeEntity {
public:
    /// @brief Creates method entity.
    /// @param name Method name.
    /// @param line Declaration line number.
    /// @param returnType Return type text.
    /// @param isStatic Static qualifier flag.
    MethodEntity(std::string name, std::size_t line, std::string returnType, bool isStatic);

    /// @brief Adds parameter to signature.
    /// @param param Parameter model.
    void addParam(const Parameter& param);
    /// @brief Builds normalized method signature string.
    /// @return Signature text.
    std::string getSignature() const;

    /// @brief Returns return type.
    /// @return Return type text.
    const std::string& getReturnType() const;
    /// @brief Returns parameter list.
    /// @return Parameters.
    const std::vector<Parameter>& getParams() const;
    /// @brief Indicates whether method is static.
    /// @return true for static methods.
    bool isStaticMethod() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string returnType_;
    std::vector<Parameter> params_;
    bool isStatic_;
};
