
#ifndef UNTITLED4_BOARDCELL_H
#define UNTITLED4_BOARDCELL_H

#include "CellType.h"
#include "Direction.h"

template<CellType t, Direction d, int l>
struct BoardCell{
    constexpr static CellType type = t;
    constexpr static Direction direction = d;
    constexpr static int length = l;
};

#endif //UNTITLED4_BOARDCELL_H
