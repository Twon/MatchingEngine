#pragma once

#include <cstdint>
#include <utility>

enum class Side : std::uint32_t
{
    Bid,
    Ask
};

constexpr auto oppositeSide(Side side)
{
    return static_cast<Side>(std::to_underlying(Side::Ask) - std::to_underlying(side));
}