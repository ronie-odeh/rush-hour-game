

#ifndef UNTITLED4_RUSHHOUR_H
#define UNTITLED4_RUSHHOUR_H

#include "MoveVehicle.h"

template<typename board, CellType wantedCT, CellType givenCT, int i, int j, int width, int widthMinus>
struct findCellCoordinates2{
    typedef findCellCoordinates2<board, wantedCT, Get<typename board::board, i, j + 1>::value::type, i, j + 1, width, widthMinus> coordinate;
    constexpr static int wantedI = coordinate::wantedI;
    constexpr static int wantedJ = coordinate::wantedJ;
};

template<typename board, CellType wantedCT, CellType givenCT, int i, int width, int widthMinus>
struct findCellCoordinates2<board, wantedCT, givenCT, i, widthMinus, width, widthMinus>{
    typedef findCellCoordinates2<board, wantedCT, Get<typename board::board, i + 1, 0>::value::type, i + 1, 0, width, widthMinus> coordinate;
    constexpr static int wantedI = coordinate::wantedI;
    constexpr static int wantedJ = coordinate::wantedJ;
};

template<typename board, CellType ct, int i, int j, int width, int widthMinus>
struct findCellCoordinates2<board, ct, ct, i, j, width, widthMinus>{
    constexpr static int wantedI = i;
    constexpr static int wantedJ = j;
};

template<typename board, CellType ct, int i, int width, int widthMinus>
struct findCellCoordinates2<board, ct, ct, i, widthMinus, width, widthMinus>{
    constexpr static int wantedI = i;
    constexpr static int wantedJ = width - 1;
};

template<typename board, CellType ct>
struct findCellCoordinates{
    typedef findCellCoordinates2<board, ct, Get<typename board::board, 0, 0>::value::type, 0, 0, board::width, board::width - 1> coordinate;
    constexpr static int wantedI = coordinate::wantedI;
    constexpr static int wantedJ = coordinate::wantedJ;
};

template<typename board, typename move>
struct GetBoardAfterSingleMove{
    typedef findCellCoordinates<board, move::type> coordinate;
    typedef typename MoveVehicle<board, coordinate::wantedI, coordinate::wantedJ, move::direction, move::amount>::board value;
};

template<typename board, typename moves>
struct GetBoardAfterMoves;

template<typename board, typename move, typename... remainingMoves>
struct GetBoardAfterMoves<board, List<move, remainingMoves...>>{
typedef typename GetBoardAfterSingleMove<board, move>::value board2;
typedef typename GetBoardAfterMoves<board2, List<remainingMoves...>>::value value;
};

template<typename board>
struct GetBoardAfterMoves<board, List<>>{
    typedef board value;
};

/*
template<typename board, typename move>
struct GetBoardAfterMoves<board, List<move>>{
    typedef typename GetBoardAfterSingleMove<board, move>::value value;
};
 */

template<typename board, CellType curCT, int i, int startingPoint, int Width, int WidthMinus>
struct CheckWin3{
    constexpr static bool value = false;
};

template<typename board, int i, int startingPoint, int Width, int WidthMinus>
struct CheckWin3<board, EMPTY, i, startingPoint, Width, WidthMinus>{
    constexpr static bool value = CheckWin3<board, Get<typename board::board, i, startingPoint + 1>::value::type, i, startingPoint + 1, Width, WidthMinus>::value;
};

template<typename board, int i, int Width, int WidthMinus>
struct CheckWin3<board, EMPTY, i, WidthMinus, Width, WidthMinus>{
    constexpr static bool value = true;
};


template<typename board>
struct CheckWin2{
    typedef findCellCoordinates<board, X> coordinate;
    constexpr static int startingPoint = GetLastHorizontalIndex<board ,coordinate::wantedI, coordinate::wantedJ>::value;
    //typedef Put<board, coordinate::wantedI, startingPoint, BoardCell<EMPTY, LEFT, 0>>::value manipulatedBoard;
    constexpr static bool value = CheckWin3<board, EMPTY,coordinate::wantedI, startingPoint, board::width, board::width - 1>::value;
};

template<typename board>
struct CheckWin{
    constexpr static bool result = CheckWin2<board>::value;
};

template<typename board, typename Moves>
struct CheckSolution2{
    typedef typename GetBoardAfterMoves<board, Moves>::value newBoard;

    constexpr static bool value = CheckWin<newBoard>::result;
};

template<typename board, typename Moves>
struct CheckSolution{
    constexpr static bool result = CheckSolution2<board, Moves>::value;
};


#endif //UNTITLED4_RUSHHOUR_H
