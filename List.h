//
// Created by osama on 1/26/2023.
//

#ifndef UNTITLED4_LIST_H
#define UNTITLED4_LIST_H

template<typename... TT>
struct List;

template<typename T, typename... TT>
struct List<T, TT...>{
    typedef T head;
    typedef List<TT...> next;
    constexpr static int size = sizeof...(TT) + 1;
};

template<>
struct List<>{
    constexpr static int size = 0;
};

template<typename T, typename L>
struct PrependList;

template<typename T, typename... TT>
struct PrependList<T, List<TT...>>{
    typedef List<T,TT...> list;
};

template<int n, typename list>
struct GetAtIndex;

template<int n, typename T, typename... TT>
struct GetAtIndex<n, List<T, TT...>>{
    typedef typename GetAtIndex<n - 1, List<TT...>>::value value;
};

template<typename T, typename... TT>
struct GetAtIndex<0, List<T, TT...>>{
    typedef T value;
};

template<int n, typename toAdd, typename l>
struct SetAtIndex;

template<int n, typename toAdd, typename T, typename... TT>
struct SetAtIndex<n, toAdd, List<T, TT...>>{
    typedef typename PrependList<T, typename SetAtIndex<n - 1, toAdd, List<TT...>>::list>::list list;
};

template<typename toAdd, typename T, typename... TT>
struct SetAtIndex<0, toAdd, List<T,TT...>>{
    typedef List<toAdd, TT...> list;
};


#endif //UNTITLED4_LIST_H
