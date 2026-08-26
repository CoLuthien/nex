
#include "support.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace hw
{

void
say(char const* what)
{
    std::printf("  .. %s\n", what);
    std::fflush(stdout);
}

std::vector<char>
read_file(fs::path const& path)
{
    std::ifstream in{path, std::ios::binary};
    if (not in) throw std::runtime_error("cannot read " + path.string());

    std::vector<char> bytes{std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};

    std::printf("     %-40s %8zu B\n", path.filename().string().c_str(), bytes.size());
    return bytes;
}

float
from_bf16(std::uint16_t raw)
{
    std::uint32_t const bits = static_cast<std::uint32_t>(raw) << 16;
    float               out{};
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

std::uint16_t
to_bf16(float value)
{
    std::uint32_t bits{};
    std::memcpy(&bits, &value, sizeof(bits));

    // Round to nearest, ties to even -- the same rule the array rounds by, so a value that is
    // exactly between two bf16 lands in the same place here as there.
    std::uint32_t const rounding = 0x7FFFU + ((bits >> 16) & 1U);
    return static_cast<std::uint16_t>((bits + rounding) >> 16);
}

float
stream_of_numbers::next()
{
    m_state += 0x9E3779B97F4A7C15ULL;

    auto z = m_state;
    z      = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z      = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z      = z ^ (z >> 31);

    // 24 bits into [0, 1), then centred. Float division by a power of two is exact, so this is
    // the same number on every platform down to the last bit.
    auto const unit = static_cast<float>(z >> 40) / 16777216.0F;
    return 2.0F * unit - 1.0F;
}

void
stream_of_numbers::fill(std::span<std::uint16_t> into)
{
    for (auto& element : into)
    {
        element = to_bf16(next());
    }
}

deviation
measure(std::span<std::uint16_t const> got,
        std::span<std::uint16_t const> want,
        float                          rel_tol,
        float                          abs_tol)
{
    deviation found{};

    double error_squared = 0.0;
    double want_squared  = 0.0;

    for (std::size_t at = 0; at < want.size(); ++at)
    {
        auto const a   = from_bf16(got[at]);
        auto const b   = from_bf16(want[at]);
        auto const abs = std::fabs(a - b);
        auto const rel = std::fabs(b) > 0.0F ? abs / std::fabs(b) : abs;

        error_squared += static_cast<double>(abs) * abs;
        want_squared += static_cast<double>(b) * b;

        if (a == 0.0F) ++found.zeros;
        if (rel > found.worst_rel)
        {
            found.worst_rel    = rel;
            found.worst_rel_at = at;
        }
        if (abs > found.worst_abs)
        {
            found.worst_abs    = abs;
            found.worst_abs_at = at;
        }
        if (abs > abs_tol and rel > rel_tol) ++found.outside;
    }

    found.relative_l2 = want_squared > 0.0 ? std::sqrt(error_squared / want_squared) : 0.0;
    return found;
}

void
print(deviation const& off, std::size_t total)
{
    std::printf("     상대 L2 오차 %.4g\n", off.relative_l2);
    std::printf("     최대 절대오차 %.4g (원소 %zu)\n",
                static_cast<double>(off.worst_abs),
                off.worst_abs_at);
    std::printf("     최대 상대오차 %.4g (원소 %zu)\n",
                static_cast<double>(off.worst_rel),
                off.worst_rel_at);
    std::printf("     허용 밖 %zu / %zu, 0 인 원소 %zu개\n", off.outside, total, off.zeros);

    if (off.zeros == total)
    {
        std::printf("     (전부 0 -- 커널이 이 버퍼에 쓰지 않았다는 뜻이다)\n");
    }
}

void
show_head(std::span<std::uint16_t const> got, std::span<std::uint16_t const> want)
{
    std::printf("     나온 값");
    for (std::size_t at = 0; at < 6 and at < got.size(); ++at)
        std::printf(" %9.4f", static_cast<double>(from_bf16(got[at])));
    std::printf("\n     정답   ");
    for (std::size_t at = 0; at < 6 and at < want.size(); ++at)
        std::printf(" %9.4f", static_cast<double>(from_bf16(want[at])));
    std::printf("\n");
}

void
argument_storage::release::operator()(void* held) const noexcept
{
    std::free(held);
}

argument_storage::argument_storage(std::size_t bytes)
{
    // aligned_alloc takes a size that is a whole number of alignments, and XRT pins whole pages
    // anyway -- so the rounding is not waste, it is what actually gets registered.
    constexpr std::size_t kPage = 4096;

    m_bytes = (bytes + kPage - 1) / kPage * kPage;

    auto* const raw = std::aligned_alloc(kPage, m_bytes);
    if (nullptr == raw)
    {
        throw std::bad_alloc{};
    }

    // Cleared on purpose: a result buffer that the kernel never wrote has to read as zeros rather
    // than as whatever the allocator handed back, which is what makes an all-zero result a signal.
    std::memset(raw, 0, m_bytes);
    m_held.reset(static_cast<std::byte*>(raw));
}

void
describe_signature(xrt::xclbin const& binary)
{
    for (auto const& one : binary.get_kernels())
    {
        std::printf("     커널 %s, 인자 %zu개\n", one.get_name().c_str(), one.get_num_args());
    }
}

void
describe_header(std::span<command::word const> stream)
{
    if (stream.size() < 4)
    {
        std::printf("     스트림이 헤더보다 짧다 (%zu 워드)\n", stream.size());
        return;
    }

    std::printf("     헤더 major=%u minor=%u generation=%u rows=%u cols=%u memtile=%u\n",
                stream[0] & 0xFFU,
                (stream[0] >> 8) & 0xFFU,
                (stream[0] >> 16) & 0xFFU,
                (stream[0] >> 24) & 0xFFU,
                stream[1] & 0xFFU,
                (stream[1] >> 8) & 0xFFU);
    std::printf("     명령 %u개, %u B (스트림 %zu B)\n",
                stream[2],
                stream[3],
                stream.size() * sizeof(command::word));
}

bool
matches_reference(std::span<command::word const> ours, fs::path const& reference)
{
    auto const theirs = read_file(reference);
    bool const same   = theirs.size() == ours.size() * sizeof(command::word) and
                      std::memcmp(theirs.data(), ours.data(), theirs.size()) == 0;

    std::printf("     aiecc 스트림과 %s\n", same ? "바이트 일치" : "다름 (!)");
    return same;
}

} // namespace hw
