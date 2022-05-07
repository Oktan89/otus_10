cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk_tcp VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Whether to build Google tets" OFF)
option(WITH_BOOST "Add boost libraries" ON)

find_package(Threads)

add_executable(bulk_server main.cpp)


set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


if(WITH_BOOST)
    find_package(Boost COMPONENTS thread system REQUIRED)
    if(Boost_FOUND)
    
        include_directories(${Boost_INCLUDE_DIR})
        link_directories(${Boost_LYBRARY_DIR})
        target_link_libraries(bulk_server PRIVATE
            ${Boost_LIBRARIES}
        )
    endif()
endif()

target_link_libraries(bulk_server PRIVATE
    ${CMAKE_SOURCE_DIR}/bulk_lib/libasync.so
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WITH_GOOGLE_TEST)
    add_subdirectory(./googletest EXCLUDE_FROM_ALL)
    add_executable(google_test_version test_version_g.cpp)

    set_target_properties(google_test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(google_test_version 
        gtest_main 
        gmock
        async_ver
    )
endif()

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )

    if(WITH_GOOGLE_TEST)
        target_compile_options(google_test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    if(WITH_GOOGLE_TEST)
        target_compile_options(google_test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT r.zhigalenko@mail.ru)
include(CPack)

if(WITH_GOOGLE_TEST)
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(google_test_version)
endif()