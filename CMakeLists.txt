cmake_minimum_required(VERSION 3.16)
project(sqlEngine)

set(CMAKE_CXX_STANDARD 11)

set(Sources main.cpp SqlEngine/SqlEngine.cpp Table/Table.cpp Table/RowElement.cpp Table/Row.cpp SqlEngine/State.cpp Table/CellFactory.cpp SqlEngine/Date.cpp SqlEngine/Time.cpp)
set(Headers SqlEngine/SqlEngine.h Table/Table.h Table/RowElement.h Table/Row.h Table/Column.cpp Table/Column.h types.h SqlEngine/State.h Table/CellFactory.h SqlEngine/Date.h SqlEngine/Time.h)

add_executable(sqlEngine ${Sources} ${Headers})