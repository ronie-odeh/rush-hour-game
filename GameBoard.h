

#ifndef UNTITLED4_GAMEBOARD_H
#define UNTITLED4_GAMEBOARD_H

template<typename b>
struct GameBoard{
    typedef b board;
    constexpr static int width = b::head::size;
    constexpr static int length = b::size;
};

#endif //UNTITLED4_GAMEBOARD_H
