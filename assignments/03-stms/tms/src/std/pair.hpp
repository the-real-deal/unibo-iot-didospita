#pragma once

template <typename Left, typename Right>
struct Pair
{
    Left left;
    Right right;

    Pair(Left left, Right right) : left(left), right(right) {}
};
