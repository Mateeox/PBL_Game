// PBL_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Game.hpp"
#include "Window.hpp"

#include "PathFinding/PathFindingUtils.hpp"

using namespace PBLGame;


int main()
{
    Window okienko(1280, 720, "37");
        Game gierka(okienko);
    gierka.Granko();

}

