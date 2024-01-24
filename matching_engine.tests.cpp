#include "matching_engine.hpp"

#include <catch2/catch_test_macros.hpp>

auto generatePopulated()
{
    MatchingEngine<int> matchingEngine;
    matchingEngine.limitOrder<Side::Ask>(103, 40);
    matchingEngine.limitOrder<Side::Ask>(103, 35);
    matchingEngine.limitOrder<Side::Ask>(103, 50);
    matchingEngine.limitOrder<Side::Ask>(102, 20);
    matchingEngine.limitOrder<Side::Ask>(102, 30);
    matchingEngine.limitOrder<Side::Ask>(102, 50);
    matchingEngine.limitOrder<Side::Ask>(101, 10);
    matchingEngine.limitOrder<Side::Ask>(101, 20);
    matchingEngine.limitOrder<Side::Ask>(101, 30);

    matchingEngine.limitOrder<Side::Bid>(99, 10);
    matchingEngine.limitOrder<Side::Bid>(99, 20);
    matchingEngine.limitOrder<Side::Bid>(99, 30);
    matchingEngine.limitOrder<Side::Bid>(98, 20);
    matchingEngine.limitOrder<Side::Bid>(98, 30);
    matchingEngine.limitOrder<Side::Bid>(98, 50);
    matchingEngine.limitOrder<Side::Bid>(97, 40);
    matchingEngine.limitOrder<Side::Bid>(97, 35);
    matchingEngine.limitOrder<Side::Bid>(97, 50);
    return matchingEngine;
}

TEST_CASE("Test construction of a matching engine", "[matching_engine.construct]")
{
    GIVEN("An orderbook with multiple levels populated on each side")
    {
        auto matchingEngine = generatePopulated();

        WHEN("inspecting the order book levels")
        {
            MatchingEngine<int> const& cmatchingEngine = matchingEngine;
            MatchingEngine<int>::AskLevels const expectedAskLevels =
            {
                {101, {{7, 10}, {8, 20}, {9, 30}}},
                {102, {{4, 20}, {5, 30}, {6, 50}}},
                {103, {{1, 40}, {2, 35}, {3, 50}}}
            };
            MatchingEngine<int>::BidLevels const expectedBidLevels =
            {
                {99, {{10, 10}, {11, 20}, {12, 30}}},
                {98, {{13, 20}, {14, 30}, {15, 50}}},
                {97, {{16, 40}, {17, 35}, {18, 50}}}
            };

            THEN("expect an orderbook populated with the input limit orders")
            {
                REQUIRE(cmatchingEngine.getLevels<Side::Ask>() == expectedAskLevels);
                REQUIRE(cmatchingEngine.getLevels<Side::Bid>() == expectedBidLevels);
            }
        }
    }
}

TEST_CASE("Test crossing the book of the orderbook so the order is partially executed", "[matching_engine.cross_book.partially_excuted_bid_inserts_remaining]")
{
    GIVEN("An orderbook with multiple levels populated on each side")
    {
        auto matchingEngine = generatePopulated();

        WHEN("The placing a bid order which crosses the book and consumes multiple ask levels without producing partial orders")
        {
            auto const insertedOrder = matchingEngine.limitOrder<Side::Bid>(102, 200);

            THEN("Expect the bid to consume 2 levels of the book below the input price, and insert the remaining on the bid side")
            {
                REQUIRE(insertedOrder);

                MatchingEngine<int>::AskLevels const expectedAskLevels =
                {
                    {103, {{1, 40}, {2, 35}, {3, 50}}}
                };
                MatchingEngine<int> const& cmatchingEngine = matchingEngine;
                REQUIRE(cmatchingEngine.getLevels<Side::Ask>() == expectedAskLevels);

                MatchingEngine<int>::BidLevels const expectedBidLevels =
                {
                    {102, {{19, 40}}},
                    {99, {{10, 10}, {11, 20}, {12, 30}}},
                    {98, {{13, 20}, {14, 30}, {15, 50}}},
                    {97, {{16, 40}, {17, 35}, {18, 50}}}
                };
                REQUIRE(cmatchingEngine.getLevels<Side::Bid>() == expectedBidLevels);
            }
        }
    }
}

TEST_CASE("Test crossing the book of the orderbook so the order is fully execute but leaves partially executed orders in the book", "[matching_engine.cross_book.partial_excuted_order_remains]")
{
    GIVEN("An orderbook with multiple levels populated on each side")
    {
        auto matchingEngine = generatePopulated();

        WHEN("The placing a bid order which crosses the book and consumes multiple bid levels and producing a final partial order")
        {
            auto const insertedOrder = matchingEngine.limitOrder<Side::Ask>(97, 180);

            THEN("Expect the ask levels to consume 2 levels of the book below the input price, and partially execute order 16 before running out of quantity")
            {
                REQUIRE(!insertedOrder);

                MatchingEngine<int>::BidLevels const expectedBidLevels =
                {
                    {97, {{16, 20}, {17, 35}, {18, 50}}}
                };
                MatchingEngine<int> const& cmatchingEngine = matchingEngine;
                REQUIRE(cmatchingEngine.getLevels<Side::Bid>() == expectedBidLevels);
            }
        }
    }
}