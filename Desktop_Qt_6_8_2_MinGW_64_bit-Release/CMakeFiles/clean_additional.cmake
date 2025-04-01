# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\cybersec_toolkit_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\cybersec_toolkit_autogen.dir\\ParseCache.txt"
  "cybersec_toolkit_autogen"
  )
endif()
