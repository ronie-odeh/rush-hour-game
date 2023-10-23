//
// Created by osama on 1/26/2023.
//

#ifndef UNTITLED4_CONDITIONAL_H
#define UNTITLED4_CONDITIONAL_H

template<bool expr, typename T1, typename T2>
struct Conditional{
    typedef T1 value;
};

template<typename T1, typename T2>
struct Conditional<false, T1, T2>{
    typedef T2 value;
};

template<bool expr, int n1, int n2>
struct ConditionalInteger{
    constexpr static int value = n1;
};

template<int n2, int n1>
struct ConditionalInteger<false, n1, n2>{
    constexpr static int value = n2;
};

#endif //UNTITLED4_CONDITIONAL_H
