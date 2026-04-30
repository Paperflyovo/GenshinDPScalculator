cmake_minimum_required(VERSION 3.16)
project(GenshinDPS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(GenshinDPS
    main.cpp
    MainWindow.hpp MainWindow.cpp
    DataModel.hpp DataModel.cpp
    CharacterPreset.hpp CharacterPreset.cpp
    TeamConfig.hpp TeamConfig.cpp
    Buff.hpp Buff.cpp
    ReactionRule.hpp ReactionRule.cpp
    DPSEngine.hpp DPSEngine.cpp
    Page_DPS.hpp Page_DPS.cpp
    Page_Character.hpp Page_Character.cpp
    Page_Team.hpp Page_Team.cpp
    Page_Reaction.hpp Page_Reaction.cpp
    resources.qrc
)

target_link_libraries(GenshinDPS PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
