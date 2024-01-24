#pragma once

#include "side.hpp"
#include "types.hpp"

#include <cstdint>
#include <functional>
#include <format>
#include <list>
#include <map>
#include <optional>
#include <tuple>
#include <utility>

using OrderId = std::uint32_t;

template <typename PriceType>
class MatchingEngine {
public:
    struct Order
    {
        OrderId id;
        Quantity quantity;
    };

    using Level = std::list<Order>;

    template <typename P, template <typename> typename T>
    using Levels = std::map<P, Level, T<P>>;
    using AskLevels = Levels<Price, std::less>;
    using BidLevels = Levels<Price, std::greater>;
    using Orderbook = std::tuple<BidLevels, AskLevels>;

    template <Side S>
    std::optional<OrderId> limitOrder(Price price, Quantity quantity)
    {
        auto const remainingQuantity = crossBook<S>(price, quantity);
        if (!remainingQuantity)
            return std::nullopt;

        auto& level = getLevels<S>()[price];
        level.push_back(Order{remainingQuantity.value(), getNextId()});
        return level.back().id;
    }

    template <Side S> auto const& getLevels() const { return std::get<S>(mOrderbook); }
private:
    template <Side S>
    std::optional<Quantity> crossBook(Price price, Quantity quantity)
    {
        auto constexpr opposite = oppositeSide(S);
        auto& crossLevels = getLevels<opposite>();
                
        if (!crossLevels.empty())
        {
            auto const compFunc = crossLevels.key_comp();
            auto topLevel = crossLevels.begin();
            auto levelPrice = topLevel->first;

            while (!compFunc(levelPrice, price) and (topLevel != crossLevels.end()))
            {
                auto& level = topLevel->second;
                auto front = level.begin();

                while (front != level.end() )
                {
                    auto current = front;
                    front++;

                    if (front->quantity < quantity)
                    {
                        quantity -= front->quantity;
                        level.erase(current);
                    }
                    else
                    {
                        front->quantity -= quantity;
                        return std::nullopt;
                    }
                }
            }
        }
        return quantity;
    }

    /// Fetch the next valid order Id, and perform internal book keeping of IDs.
    auto getNextId() { return mNextId++; }

    template <Side S> auto& getLevels() { return std::get<std::to_underlying(S)>(mOrderbook); }

    OrderId mNextId = 0;
    Orderbook mOrderbook;
};
