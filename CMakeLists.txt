CU_SET_PATH("CMAKE_LEVEL_ITEM_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/LevelItem.cpp")

AC_ADD_SCRIPT_LOADER("LevelItem"      "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_levelitem.conf.dist")

message("-- LevelItem --")
