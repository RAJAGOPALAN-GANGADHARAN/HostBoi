# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ClientBoi_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ClientBoi_autogen.dir\\ParseCache.txt"
  "ClientBoi_autogen"
  )
endif()
