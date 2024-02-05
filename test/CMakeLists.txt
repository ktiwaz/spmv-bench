project(test)

add_executable(print print.cpp)
target_link_libraries(print spmm)

add_executable(print_csr print_csr.cpp)
target_link_libraries(print_csr spmm)

add_executable(print_ell print_ell.cpp)
target_link_libraries(print_ell spmm)

add_executable(print_bcsr print_bcsr.cpp)
target_link_libraries(print_bcsr spmm)

