#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace {
using namespace gain_ground;

void require(bool condition, const char *message)
{
    if (!condition) throw std::runtime_error(message);
}

class CarryReturningHost final : public ExecutionHost {
public:
    CarryReturningHost()
    {
        shared_[0x0003fffaU] = 0x2600U;
        shared_[0x0003fffcU] = 0x0000U;
        shared_[0x0003fffeU] = 0x0696U;
    }

    std::uint16_t read_memory_word(std::uint16_t region,
        std::uint32_t byte_offset, std::uint16_t) override
    {
        if (region != 3U) return 0U;
        const auto found = shared_.find(byte_offset);
        return found == shared_.end() ? 0U : found->second;
    }

    void write_memory_word(std::uint16_t region, std::uint32_t byte_offset,
        std::uint16_t data, std::uint16_t memory_mask) override
    {
        require(region == 3U, "Function 34 wrote outside shared stack memory");
        auto &value = shared_[byte_offset];
        value = static_cast<std::uint16_t>(
            (value & ~memory_mask) | (data & memory_mask));
        if (byte_offset == 0x0003fffaU && memory_mask == 0x00ffU
            && (data & 0x00ffU) == 1U) {
            observed_carry_result_write_ = true;
        }
    }

    std::uint16_t read_hardware(std::uint8_t kind, std::uint8_t cpu,
        std::uint8_t state, std::uint32_t, std::uint32_t address,
        std::uint16_t memory_mask) override
    {
        require(kind == 1U && cpu == 0U && state == 0xffU,
            "Unexpected Function 34 hardware-read identity");
        require(address == 0x00b00004U && memory_mask == 0x00ffU,
            "Unexpected Function 34 hardware-read lane");
        return hardware_read_count_++ == 0U ? 0U : 0x00ffU;
    }

    void write_hardware(std::uint8_t kind, std::uint8_t cpu,
        std::uint8_t state, std::uint32_t, std::uint32_t address,
        std::uint16_t data, std::uint16_t memory_mask) override
    {
        require(kind == 2U && cpu == 0U && state == 0xffU,
            "Unexpected Function 34 hardware-write identity");
        require(address == 0x00b00004U && data == 0xffffU
                && memory_mask == 0x00ffU,
            "Unexpected Function 34 hardware-write payload");
    }

    PendingInterrupt consume_pending_interrupt(
        std::uint8_t, std::uint8_t, std::uint32_t) override
    {
        return {};
    }

    FunctionResult call_function(std::uint32_t function_id,
        std::uint8_t target_cpu, std::uint8_t target_state,
        std::uint8_t kind, std::uint32_t callsite, std::uint32_t target,
        FunctionContext &context) override
    {
        require(function_id == 23U && target_cpu == 0U
                && target_state == 0xffU && kind == 2U,
            "Unexpected Function 34 child identity");
        require(callsite == 0x000021e6U && target == 0x000018a4U,
            "Unexpected Function 34 child edge");
        context.registers.address[7] += 4U;
        context.registers.status = static_cast<std::uint16_t>(
            context.registers.status | 0x0001U);
        context.registers.program_counter = 0x000021eaU;
        return FunctionResult::complete(1U, 0x000021eaU);
    }

    [[nodiscard]] bool observed_carry_result_write() const noexcept
    {
        return observed_carry_result_write_;
    }

private:
    std::unordered_map<std::uint32_t, std::uint16_t> shared_;
    unsigned hardware_read_count_{};
    bool observed_carry_result_write_{};
};

class TransferCarryHost final : public ExecutionHost {
public:
    TransferCarryHost()
    {
        shared_[0x0003fff6U] = 0x2700U;
        shared_[0x0003fff8U] = 0x0008U;
        shared_[0x0003fffaU] = 0x0258U;
    }

    std::uint16_t read_memory_word(std::uint16_t region,
        std::uint32_t byte_offset, std::uint16_t memory_mask) override
    {
        require(memory_mask == 0xffffU,
            "Function 35 used an unexpected read mask");
        if (region == 1U) {
            static constexpr std::uint32_t expected[] = {
                0x1018U, 0x101aU, 0x2210U, 0x2212U, 0x2218U,
                0x221aU, 0x221cU, 0x221eU, 0x2220U,
            };
            require(program_read_index_ < std::size(expected)
                    && byte_offset == expected[program_read_index_++],
                "Function 35 carry path fetched the wrong program word");
            return 0U;
        }
        require(region == 3U,
            "Function 35 read outside program or shared memory");
        shared_reads_.push_back(byte_offset);
        const auto found = shared_.find(byte_offset);
        return found == shared_.end() ? 0U : found->second;
    }

    void write_memory_word(std::uint16_t region, std::uint32_t byte_offset,
        std::uint16_t data, std::uint16_t memory_mask) override
    {
        require(region == 3U,
            "Function 35 wrote outside shared stack memory");
        writes_.push_back({byte_offset, data, memory_mask});
        auto &value = shared_[byte_offset];
        value = static_cast<std::uint16_t>(
            (value & ~memory_mask) | (data & memory_mask));
    }

    std::uint16_t read_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t) override
    {
        throw std::runtime_error("Function 35 unexpectedly read hardware");
    }

    void write_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t, std::uint16_t) override
    {
        throw std::runtime_error("Function 35 unexpectedly wrote hardware");
    }

    PendingInterrupt consume_pending_interrupt(
        std::uint8_t, std::uint8_t, std::uint32_t) override
    {
        return {};
    }

    FunctionResult call_function(std::uint32_t function_id,
        std::uint8_t target_cpu, std::uint8_t target_state,
        std::uint8_t kind, std::uint32_t callsite, std::uint32_t target,
        FunctionContext &context) override
    {
        require(function_id == 12U && target_cpu == 0U
                && target_state == 0xffU && kind == 2U,
            "Unexpected Function 35 child identity");
        require(callsite == 0x0000220cU && target == 0x00001018U,
            "Unexpected Function 35 child edge");
        context.registers.address[7] += 4U;
        context.registers.status = static_cast<std::uint16_t>(
            context.registers.status | 0x0001U);
        (void)read_memory_word(1U, 0x00002210U, 0xffffU);
        (void)read_memory_word(1U, 0x00002212U, 0xffffU);
        context.registers.program_counter = 0x00002210U;
        return FunctionResult::complete(1U, 0x00002210U);
    }

    void verify() const
    {
        require(program_read_index_ == 9U,
            "Function 35 carry path omitted a program fetch");
        require(writes_.size() == 3U
                && writes_[0].offset == 0x0003fff2U
                && writes_[0].data == 0U
                && writes_[0].mask == 0xffffU
                && writes_[1].offset == 0x0003fff4U
                && writes_[1].data == 0x2210U
                && writes_[1].mask == 0xffffU
                && writes_[2].offset == 0x0003fff6U
                && writes_[2].data == 0x0101U
                && writes_[2].mask == 0x00ffU,
            "Function 35 carry path wrote the wrong exception-frame data");
        const std::vector<std::uint32_t> expected_reads = {
            0x0003fff6U, 0x0003fff6U, 0x0003fff8U,
            0x0003fffaU, 0x00000258U, 0x0000025aU,
        };
        require(shared_reads_ == expected_reads,
            "Function 35 carry path used the wrong RTE read order");
    }

private:
    struct Write {
        std::uint32_t offset;
        std::uint16_t data;
        std::uint16_t mask;
    };
    std::unordered_map<std::uint32_t, std::uint16_t> shared_;
    std::vector<std::uint32_t> shared_reads_;
    std::vector<Write> writes_;
    std::size_t program_read_index_{};
};

class RuntimeModeHost final : public ExecutionHost {
public:
    explicit RuntimeModeHost(std::uint16_t mode, bool transfer_carry)
        : mode_(mode), transfer_carry_(transfer_carry)
    {
        shared_[0x0003fffcU] = 0x0008U;
        shared_[0x0003fffeU] = 0x00d4U;
    }

    std::uint16_t read_memory_word(std::uint16_t region,
        std::uint32_t byte_offset, std::uint16_t) override
    {
        // Supply the original TRAP vectors. The translated caller now follows
        // their actual values instead of discarding reads and hardcoding targets.
        if (region == 1U) {
            if (byte_offset == 0x98U || byte_offset == 0xa0U) return 0x0008U;
            if (byte_offset == 0x9aU) return 0x007eU;
            if (byte_offset == 0xa2U) return 0x008aU;
        }
        if (region == 3U) {
            const auto found = shared_.find(byte_offset);
            return found == shared_.end() ? 0U : found->second;
        }
        if (region == 2U) return main_[byte_offset];
        return 0U;
    }

    void write_memory_word(std::uint16_t region, std::uint32_t byte_offset,
        std::uint16_t data, std::uint16_t memory_mask) override
    {
        auto &memory = region == 2U ? main_ : shared_;
        require(region == 2U || region == 3U,
            "Function 58 wrote outside CPU-A RAM");
        auto &value = memory[byte_offset];
        value = static_cast<std::uint16_t>(
            (value & ~memory_mask) | (data & memory_mask));
        if (region == 3U && byte_offset == 0x00038000U
            && memory_mask == 0xff00U && (data & 0xff00U) == 0xff00U)
            observed_shared_st_ = true;
    }

    std::uint16_t read_hardware(std::uint8_t kind, std::uint8_t cpu,
        std::uint8_t state, std::uint32_t pc, std::uint32_t address,
        std::uint16_t memory_mask) override
    {
        require(kind == 1U && cpu == 0U && state == 0xffU
                && pc == 0x00080236U && address == 0x00800008U
                && memory_mask == 0xffffU,
            "Function 58 used the wrong runtime-mode hardware read");
        return mode_;
    }

    void write_hardware(std::uint8_t, std::uint8_t, std::uint8_t,
        std::uint32_t, std::uint32_t, std::uint16_t, std::uint16_t) override
    {
        throw std::runtime_error("Function 58 unexpectedly wrote hardware");
    }

    PendingInterrupt consume_pending_interrupt(
        std::uint8_t, std::uint8_t, std::uint32_t) override { return {}; }

    FunctionResult call_function(std::uint32_t function_id,
        std::uint8_t target_cpu, std::uint8_t target_state,
        std::uint8_t kind, std::uint32_t callsite, std::uint32_t target,
        FunctionContext &context) override
    {
        require(target_cpu == 0U && target_state == 0xffU && kind == 4U,
            "Function 58 used the wrong trap-call identity");
        calls_.push_back(function_id);
        require((function_id == 52U && target == 0x0008008aU)
                || (function_id == 50U && target == 0x0008007eU),
            "Function 58 called the wrong trap owner");
        context.registers.address[7] += 6U;
        if (function_id == 50U) {
            context.registers.status = static_cast<std::uint16_t>(
                transfer_carry_ ? context.registers.status | 1U
                                : context.registers.status & ~1U);
        }
        context.registers.program_counter = callsite + 2U;
        return FunctionResult::complete(2U, callsite + 2U);
    }

    void verify_initialization(bool expect_traps) const
    {
        require((calls_.empty()) == !expect_traps,
            "Function 58 took the wrong runtime-mode branch");
        if (expect_traps)
            require(calls_ == std::vector<std::uint32_t>({52U, 50U, 52U}),
                "Function 58 used the wrong carry-path trap sequence");
        require(observed_shared_st_ == expect_traps,
            "Function 58 shared-mode flag write did not match the path");
        for (std::uint32_t offset = 0x7b20U; offset < 0x7b70U; offset += 2U) {
            auto expected = std::uint16_t{0U};
            if (offset == 0x7b32U || offset == 0x7b38U || offset == 0x7b3eU)
                expected = 0x003cU;
            if (offset == 0x7b46U || offset == 0x7b48U || offset == 0x7b4aU)
                expected = 0xffffU;
            const auto found = main_.find(offset);
            require(found != main_.end() && found->second == expected,
                "Function 58 initialized the runtime workspace incorrectly");
        }
        const auto control = main_.find(0x410U);
        require(control != main_.end() && control->second == 0xff00U,
            "Function 58 did not set the runtime control byte");
    }

private:
    std::uint16_t mode_;
    bool transfer_carry_;
    bool observed_shared_st_{};
    std::unordered_map<std::uint32_t, std::uint16_t> shared_;
    std::unordered_map<std::uint32_t, std::uint16_t> main_;
    std::vector<std::uint32_t> calls_;
};
} // namespace

int main()
{
    CarryReturningHost host;
    gain_ground::FunctionContext context{};
    context.host = &host;
    context.cpu = 0U;
    context.state = 0xffU;
    context.registers.program_counter = 0x000021ccU;
    context.registers.address[7] = 0xfffffffaU;
    context.registers.status = 0x2600U;

    const auto result = gain_ground::translated::cpu_a_bios_fdc_drive_exception(context);
    require(result.status == gain_ground::TranslationStatus::complete
            && result.control == 2U && result.exit_program_counter == 0x00000696U,
        "Function 34 carry path returned the wrong control result");
    require(context.registers.program_counter == 0x00000696U
            && context.registers.address[7] == 0U
            && context.registers.status == 0x2601U,
        "Function 34 carry path restored the wrong exception frame");
    require((context.registers.data[1] & 0xffU) == 0xffU,
        "Function 34 did not preserve the inverted FDC sample in D1");
    require(host.observed_carry_result_write(),
        "Function 34 did not write carry result byte one");

    TransferCarryHost transfer_host;
    gain_ground::FunctionContext transfer_context{};
    transfer_context.host = &transfer_host;
    transfer_context.cpu = 0U;
    transfer_context.state = 0xffU;
    transfer_context.registers.program_counter = 0x0000220cU;
    transfer_context.registers.address[7] = 0xfffffff6U;
    transfer_context.registers.status = 0x2700U;
    const auto transfer_result =
        gain_ground::translated::cpu_a_bios_fdc_transfer_exception(
            transfer_context);
    require(transfer_result.status == gain_ground::TranslationStatus::complete
            && transfer_result.control == 2U
            && transfer_result.exit_program_counter == 0x00080258U,
        "Function 35 carry path returned the wrong control result");
    require(transfer_context.registers.program_counter == 0x00080258U
            && transfer_context.registers.address[7] == 0xfffffffcU
            && transfer_context.registers.status == 0x2701U,
        "Function 35 carry path restored the wrong exception frame");
    transfer_host.verify();

    for (const bool carry_path : {false, true}) {
        RuntimeModeHost runtime_host(carry_path ? 0x20U : 0U, carry_path);
        gain_ground::FunctionContext runtime_context{};
        runtime_context.host = &runtime_host;
        runtime_context.cpu = 0U;
        runtime_context.state = 0xffU;
        runtime_context.registers.program_counter = 0x00080236U;
        runtime_context.registers.address[7] = 0xfffffffcU;
        runtime_context.registers.status = 0x2700U;
        const auto runtime_result =
            gain_ground::translated::cpu_a_init_runtime_mode_state(
                runtime_context);
        require(runtime_result.status == gain_ground::TranslationStatus::complete
                && runtime_result.control == 1U
                && runtime_result.exit_program_counter == 0x000800d4U,
            "Function 58 returned to the wrong continuation");
        require(runtime_context.registers.program_counter == 0x000800d4U
                && runtime_context.registers.address[7] == 0U,
            "Function 58 restored the wrong caller frame");
        runtime_host.verify_initialization(carry_path);
    }
    return 0;
}
