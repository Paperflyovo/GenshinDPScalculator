# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GenshinDPS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GenshinDPS_autogen.dir\\ParseCache.txt"
  "GenshinDPS_autogen"
  )
endif()
