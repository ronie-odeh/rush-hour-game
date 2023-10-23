//
// Created by osama on 1/26/2023.
//

#ifndef UNTITLED4_MOVEVEHICLE_H
#define UNTITLED4_MOVEVEHICLE_H

#include "BoardCell.h"
#include "CellType.h"
#include "Conditional.h"
#include "Direction.h"
#include "GameBoard.h"
#include "List.h"
#include "TransposeList.h"

template<CellType t, Direction d, int i>
struct Move{
    static_assert(t != EMPTY);
    constexpr static CellType type = t;
    constexpr static Direction direction = d;
    constexpr static int amount = i;
};

template<typename lists, int i, int j>
struct Get{
    typedef typename GetAtIndex<j, typename GetAtIndex<i, lists>::value>::value value;
};

template<typename lists, int i, int j, typename toPut>
struct Put{
    typedef typename SetAtIndex<i, typename SetAtIndex<j, toPut, typename GetAtIndex<i, lists>::value>::list, lists>::list value;
};

template<typename lists, int i, int j1, int j2>
struct GetFromToHorizontal{
    typedef typename PrependList<typename Get<lists, i, j1>::value, typename GetFromToHorizontal<lists, i, j1 + 1, j2>::value>::list value;
};

template<typename lists, int i, int j>
struct GetFromToHorizontal<lists, i, j, j>{
    typedef List<typename Get<lists, i, j>::value> value;
};

template<typename list, typename tail>
struct  AddTail;

template<typename T, typename... TT, typename tail>
struct AddTail<List<T, TT...>, tail>{
typedef typename PrependList<T, typename AddTail<List<TT...>, tail>::value>::list value;
};

template<typename tail>
struct AddTail<List<>, tail>{
    typedef List<tail> value;
};

template<typename list>
struct Invert;

template<typename T, typename... TT>
struct Invert<List<T, TT...>>{
typedef typename AddTail<typename Invert<List<TT...>>::value, T>::value value;
};

template<>
struct Invert<List<>>{
    typedef List<> value;
};

template<typename list, template<typename U> class op>
struct ForEach;

template<typename T, typename... TT, template<typename U> class op>
struct ForEach<List<T, TT...>, op>{
typedef typename PrependList<typename op<T>::value, typename ForEach<List<TT...>, op>::value>::list value;
};

template<template<typename U> class op>
struct ForEach<List<>, op>{
    typedef List<> value;
};

template<typename lists>
struct InvertY{
    typedef typename ForEach<lists, Invert>::value value;
};

template<typename lists>
struct InvertX{
    typedef typename Transpose<typename InvertY<typename Transpose<lists>::matrix>::value>::matrix value;
};

template<typename board, CellType prevT, CellType curT, int i, int j>
struct GetFirstHorizontalIndex2{
    constexpr static int value = j + 1;
};

template<typename board, CellType ct, int i, int j>
struct GetFirstHorizontalIndex2<board, ct, ct, i, j>{
    constexpr static int value = GetFirstHorizontalIndex2<board, Get<typename board::board, i, j - 1>::value::type, ct, i, j - 1>::value;
};

template<typename board, CellType ct, int i>
struct GetFirstHorizontalIndex2<board, ct, ct, i, 0>{
    constexpr static int value = 0;
};

template<typename board, int R, int C>
struct GetFirstHorizontalIndex{
    constexpr static CellType curType = Get<typename board::board, R, C>::value::type;
    constexpr static CellType prevType = Get<typename board::board, R, C - 1>::value::type;
    constexpr static int value = GetFirstHorizontalIndex2<board, prevType, curType, R , C - 1>::value;
};

template<typename board, int R>
struct GetFirstHorizontalIndex<board, R, 0>{
    constexpr static int value = 0;
};

template<typename board, int R, int C>
struct GetLastHorizontalIndex{
    constexpr static int value = board::width - 1 - GetFirstHorizontalIndex<GameBoard<typename InvertY<typename board::board>::value>, R, board::width - 1 - C>::value;
};

template<typename board, int R, int C>
struct MoveCellOneStepRight{
    static_assert(Get<typename board::board, R, C + 1>::value::type == EMPTY);
    typedef typename Get<typename board::board, R, C>::value toMove;
    typedef BoardCell<EMPTY, RIGHT, 0> EmptyCell;
    typedef typename Put<typename board::board, R, C, EmptyCell>::value board2;
    typedef GameBoard<typename Put<board2, R, C + 1, toMove>::value> value;

};

template<typename board, int R, int C1, int C2>
struct MoveVehicleOneStepRight2{
    typedef typename MoveCellOneStepRight<board, R, C2>::value board2;
    typedef typename MoveVehicleOneStepRight2<board2, R, C1, C2 - 1>::value value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStepRight2<board, R, C, C>{
    typedef typename MoveCellOneStepRight<board, R, C>::value value;
};


template<typename board, int R, int C>
struct MoveVehicleOneStepRight{
    constexpr static int firstHorizontalIndex = GetFirstHorizontalIndex<board, R, C>::value;
    constexpr static int lastHorizontalIndex = GetLastHorizontalIndex<board, R, C>::value;
    typedef typename MoveVehicleOneStepRight2<board, R, firstHorizontalIndex, lastHorizontalIndex>::value value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStepLeft{
    constexpr static int newC = board::width - 1 - C;
    typedef GameBoard<typename InvertY<
            typename MoveVehicleOneStepRight<
                    GameBoard<typename InvertY<typename board::board>::value>, R, newC>::value::board>::value
    > value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStepDown{
    typedef GameBoard<typename Transpose<typename MoveVehicleOneStepRight<GameBoard<typename Transpose<typename board::board>::matrix>, C, R>::value::board>::matrix> value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStepUp{
    constexpr static int newR = board::length - 1 - R;
    typedef GameBoard<typename InvertX<typename MoveVehicleOneStepDown<GameBoard<typename InvertX<typename board::board>::value>, newR, C>::value::board>::value> value;
};

template<Direction d1, Direction d2>
struct DirectionCompatible{
    constexpr static bool value = (d1 <= 1 && d2 <= 1) || (d1 >= 2 && d2 >= 2);
};

template<typename board, int R, int C, int D>
struct MoveVehicleOneStep;

template<typename board, int R, int C>
struct MoveVehicleOneStep<board, R, C, LEFT>{
    typedef typename MoveVehicleOneStepLeft<board, R, C>::value value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStep<board, R, C, RIGHT>{
    typedef typename MoveVehicleOneStepRight<board, R, C>::value value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStep<board, R, C, UP>{
    typedef typename MoveVehicleOneStepUp<board, R, C>::value value;
};

template<typename board, int R, int C>
struct MoveVehicleOneStep<board, R, C, DOWN>{
    typedef typename MoveVehicleOneStepDown<board, R, C>::value value;
};

template<int R, int C, int D>
struct GetNewCoordinates;

template<int R, int C>
struct GetNewCoordinates<R, C, UP>{
    constexpr static int newR = R - 1;
    constexpr static int newC = C;
};

template<int R, int C>
struct GetNewCoordinates<R, C, DOWN>{
    constexpr static int newR = R + 1;
    constexpr static int newC = C;
};

template<int R, int C>
struct GetNewCoordinates<R, C, LEFT>{
    constexpr static int newR = R;
    constexpr static int newC = C - 1;
};

template<int R, int C>
struct GetNewCoordinates<R, C, RIGHT>{
    constexpr static int newR = R;
    constexpr static int newC = C + 1;
};

template<typename board, int R, int C, Direction D, int A>
struct MoveVehicle2{
    typedef GetNewCoordinates<R,C,D> coordinate;
    typedef typename MoveVehicleOneStep<board, R, C, D>::value board2;
    typedef typename MoveVehicle2<board2, coordinate::newR, coordinate::newC, D, A - 1>::value value;
};

template<typename board, int R, int C, Direction D>
struct MoveVehicle2<board, R, C, D, 0>{
    typedef board value;
};

template<typename oldBoard, int R, int C, Direction D, int A>
struct MoveVehicle{
    static_assert(R >= 0);
    static_assert(C >= 0);
    static_assert(A >= 1);
    static_assert(R <= oldBoard::length);
    static_assert(C <= oldBoard::width);
    static_assert(DirectionCompatible<Get<typename oldBoard::board, R, C>::value::direction, D>::value);
    typedef typename MoveVehicle2<oldBoard, R, C, D, A>::value board;
};

#endif //UNTITLED4_MOVEVEHICLE_H
