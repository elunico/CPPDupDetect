cmake_minimum_required(VERSION 3.14)

project(cppdupdetect)

include_directories(.)

option(RELEASE_BUILD "Build in release mode" OFF)

file(GLOB SOURCES "./*.cpp")

if(WIN32)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DFAST_RANDOM /g /I \"C:/Program Files (x86)/FLTK/include\" /DWIN32 /D_WINDOWS") # Use faster but less random number generator


include_directories("C:\\Program Files\\OpenSSL-Win64-ARM\\include")

include_directories(C:\\Users\\thomaspovinelli\\Downloads\\fltk)

add_executable(main ${SOURCES})
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\bin\\lib\\Debug\\fltkd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_jpegd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_pngd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_zd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltkd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_formsd.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_gld.lib")
target_link_libraries(main "C:\\Users\\thomaspovinelli\\Downloads\\fltk\\lib\\Debug\\fltk_imagesd.lib")
target_link_libraries(main "C:\\Program Files\\OpenSSL-Win64-ARM\\lib\\VC\\arm64\\MT\\libssl.lib")
target_link_libraries(main "C:\\Program Files\\OpenSSL-Win64-ARM\\lib\\VC\\arm64\\MT\\libcrypto.lib")
else()

    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(VALGRIND_BUILD NO)
    set(SILENT_RUN NO)

    if (SILENT_RUN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSILENCE_OUTPUT")
    endif()
    

    if(RELEASE_BUILD)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    else()
        if(VALGRIND_BUILD)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer -D_GLIBCXX_DEBUG")
            if(ADDRESS_SANITIZER)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address")
            endif()
            if(THREAD_SANITIZER)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=thread")
            endif()
        endif()
    endif()

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DNO_TPS_LIMIT")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DFAST_RANDOM") # Use faster but less random number generator

    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})

    find_package(FLTK REQUIRED)
    include_directories(${FLTK_INCLUDE_DIR})
    add_executable(main ${SOURCES})
    target_link_libraries(main ${FLTK_LIBRARIES} ${FLTK_EXTRA_LIBRARIES} ${OPENSSL_LIBRARIES})
    # target_link_libraries(main ${FLTK_LIBRARIES} ${FLTK_EXTRA_LIBRARIES})

endif()
