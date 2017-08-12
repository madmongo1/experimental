sugar_include(experimental)

file(RELATIVE_PATH path_part ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR})
set(current_source_dir ${CMAKE_CURRENT_LIST_DIR})
set(current_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/${path_part}")

include(ConfigureLibraryHeader)

configure_library_header(${current_binary_dir}/experimental.hpp
        TEMPLATE ${current_source_dir}/experimental.hpp.in
        SOURCE_DIR ${current_source_dir}
        BINARY_DIR ${current_binary_dir}
        INCLUDE_PREFIX ""
        HEADERS ${PUBLIC_HEADERS})

sugar_files(PUBLIC_HEADERS ${current_binary_dir}/experimental.hpp)
