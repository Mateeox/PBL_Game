// PBL_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Game.hpp"
#include "Window.hpp"
#include "ConfigUtils.hpp"

using namespace PBLGame;


int main()
{
    ConfigUtils::LoadConfig();
  unsigned  WINDOW_WIDTH = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_WIDTH", ConfigUtils::GlobalConfigMap);
  unsigned WINDOW_HEIGHT = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_HEIGHT", ConfigUtils::GlobalConfigMap);
  std::string GameName = ConfigUtils::GetValueFromMap<std::string>("Game_Name", ConfigUtils::GlobalConfigMap);
    Window okienko(WINDOW_WIDTH, WINDOW_HEIGHT, GameName.c_str());
    Game gierka(okienko);
    gierka.Granko();

}

