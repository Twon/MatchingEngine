#include "matching_engine.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Test construction of a matching engine", "[matching_engine.construct]")
{
    MatchingEngine<int> matchingEngine;
    matchingEngine.limitOrder<Side::Ask>(103, 40);
    matchingEngine.limitOrder<Side::Ask>(103, 35);
    matchingEngine.limitOrder<Side::Ask>(103, 50);
    matchingEngine.limitOrder<Side::Ask>(102, 20);
    matchingEngine.limitOrder<Side::Ask>(102, 30);
    matchingEngine.limitOrder<Side::Ask>(103, 50);
    matchingEngine.limitOrder<Side::Ask>(101, 10);
    matchingEngine.limitOrder<Side::Ask>(101, 20);
    matchingEngine.limitOrder<Side::Ask>(101, 30);

    matchingEngine.limitOrder<Side::Ask>(99, 10);
    matchingEngine.limitOrder<Side::Ask>(99, 20);
    matchingEngine.limitOrder<Side::Ask>(99, 30);
    matchingEngine.limitOrder<Side::Ask>(98, 20);
    matchingEngine.limitOrder<Side::Ask>(98, 30);
    matchingEngine.limitOrder<Side::Ask>(98, 50);
    matchingEngine.limitOrder<Side::Ask>(97, 40);
    matchingEngine.limitOrder<Side::Ask>(97, 35);
    matchingEngine.limitOrder<Side::Ask>(97, 50);
}