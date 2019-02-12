PROJECT(wrltest)
cmake_minimum_required(VERSION 2.8)

# list of source files
set(test_files test3.cpp)

# define the executable
# if(WIN32)
  add_executable(test3 WIN32 ${test_files})
# else()
#   add_executable(test3 ${test_files})
# endif()

# in Windows + Visual Studio we need this to make it a console application
if(WIN32)
  if(MSVC)
    set_target_properties(test3 PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
endif(WIN32)

# install application
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

target_link_libraries(test3 io util wrl)

install(TARGETS test3 DESTINATION ${BIN_DIR})
