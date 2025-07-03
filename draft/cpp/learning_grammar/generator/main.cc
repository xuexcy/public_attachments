/*
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/07/03 13:16:40
# Desc   :
########################################################################
*/

#include <generator>
#include <iostream>

template<typename T>
struct Tree
{
    T value;
    Tree *left{}, *right{};

    std::generator<const T&> traverse_inorder() const
    {
        if (left)
            co_yield std::ranges::elements_of(left->traverse_inorder());

        co_yield value;

        if (right)
            co_yield std::ranges::elements_of(right->traverse_inorder());
    }
};

std::generator<char> letters(char first) {
    for (;; co_yield first++);
}
int main()
{
    Tree<char> tree[]
    {
                                    {'D', tree + 1, tree + 2},
        //                            │
        //            ┌───────────────┴────────────────┐
        //            │                                │
                    {'B', tree + 3, tree + 4},       {'F', tree + 5, tree + 6},
        //            │                                │
        //  ┌─────────┴─────────────┐      ┌───────────┴─────────────┐
        //  │                       │      │                         │
          {'A'},                  {'C'}, {'E'},                    {'G'}
    };

    for (char x : tree->traverse_inorder())
        std::cout << x << ' ';
    std::cout << '\n';

    for (auto ch : letters('a') | std::views::take(26)) {
        std::cout << ch << ' ';
    }
    std::cout << '\n';
}
