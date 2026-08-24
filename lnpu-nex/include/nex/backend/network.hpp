
#pragma once

#include "cmn/encoded/encoded.hpp"

#include <string>
#include <string_view>
#include <memory>
#include <system_error>

namespace lnpu::nex
{
class io_contract
{
public:
    virtual ~io_contract() = default;

    virtual std::size_t input_count() const  = 0;
    virtual std::size_t output_count() const = 0;

    virtual encoded::format const* input_description(int index) const             = 0;
    virtual encoded::format const* input_description(std::string_view name) const = 0;

    virtual encoded::format const* output_description(int index) const             = 0;
    virtual encoded::format const* output_description(std::string_view name) const = 0;

    virtual bool has_input(std::string_view name) const  = 0;
    virtual bool has_output(std::string_view name) const = 0;
};

class network : public io_contract
{
public:
    using shared = std::shared_ptr<network>;
    class instance;

    ~network() override = default;

    virtual std::string_view          name() const                 = 0;
    virtual std::unique_ptr<instance> prepare(std::error_code& ec) = 0;
};

class network::instance : public io_contract
{
public:
    ~instance() override = default;

    virtual std::error_code execute() = 0;

    virtual std::error_code set_input(std::string_view key, encoded::shared target)  = 0;
    virtual std::error_code set_output(std::string_view key, encoded::shared target) = 0;

    virtual std::error_code reset_input(std::string_view key)  = 0;
    virtual std::error_code reset_output(std::string_view key) = 0;
};
} // namespace lnpu::nex
