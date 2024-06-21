add_executable(GrapheTesteur GrapheTesteur.cpp ${PROJECT_SOURCE_DIR}/Graphe.cpp)
add_executable(ReseauInterUrbainTesteur ReseauInterUrbainTesteur.cpp ${PROJECT_SOURCE_DIR}/ReseauInterurbain.cpp)

target_include_directories(GrapheTesteur PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(ReseauInterUrbainTesteur PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(GrapheTesteur
        gtest_main
        gtest
        pthread
)

target_link_libraries(ReseauInterUrbainTesteur
        gtest_main
        gtest
        pthread
)

include(googletest)
gtest_discover_tests(GrapheTesteur)
gtest_discover_tests(ReseauInterUrbainTesteur)