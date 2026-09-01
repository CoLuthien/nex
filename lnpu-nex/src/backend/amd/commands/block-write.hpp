
#pragma once

#include "amd/command.hpp"

namespace lnpu::nex::amd::cmds
{

/** AI Asissted code
 */
class block_write : public amd::command
{
public:
    struct dimension
    {
        word size{};
        word stride{1};
    };

    struct parameters
    {
        word      length{}; // words to transfer
        word      offset{}; // starting word offset inside the argument
        dimension d0{};
        dimension d1{};
        dimension d2{}; // size is implied by length; only stride is emitted
        word      iteration_size{1};
        word      iteration_stride{1};

        // Packet-switched routing. Only meaningful when the design uses packet
        // switching to pick between kernels; leave disabled for circuit routes.
        bool packet_enable{false};
        word packet_id{};
        word packet_type{};
        word out_of_order_id{};

        word cache_flag{}; // AxCache QoS bits

        // BD chaining. valid_bd must be set for the BD to be usable.
        bool valid_bd{true};
        word next_bd_id{};
    };

    explicit block_write(parameters param);

    void emit(npu::placement const& where, std::vector<word>& out) const override;

private:
    parameters m_param;
};
} // namespace lnpu::nex::amd::cmds
