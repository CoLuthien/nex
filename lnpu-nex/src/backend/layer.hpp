
#pragma once

#include "cmn/encoded/encoded.hpp"
#include <memory>
#include <string_view>
#include <system_error>

namespace lnpu::nex
{

class layer
{
public:
    using unique     = std::unique_ptr<layer>;
    virtual ~layer() = default;

    virtual std::error_code set_input(std::string_view key, encoded::shared target)  = 0;
    virtual std::error_code set_output(std::string_view key, encoded::shared target) = 0;

    virtual std::error_code reset_input(std::string_view key)  = 0;
    virtual std::error_code reset_output(std::string_view key) = 0;

    virtual std::error_code execute() = 0;
};

} // namespace lnpu::nex
