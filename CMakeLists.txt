cmake_minimum_required(VERSION 3.26)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add a library target for your coding library
add_library(CCLib src/lib/CompetitiveCodingLibrary.cc)

# Function to add executables for each day
function(add_day_executable day_number)
    add_executable(day${day_number} src/${day_number}/main.cc)
    target_link_libraries(day${day_number} PRIVATE CCLib)
endfunction()

project(AOC)

# Add executables for each day
add_day_executable(1)
add_day_executable(2)
add_day_executable(3)
add_day_executable(4)
add_day_executable(5)
add_day_executable(6)
add_day_executable(7)
add_day_executable(8)
add_day_executable(9)
add_day_executable(10)
add_day_executable(11)
add_day_executable(12)