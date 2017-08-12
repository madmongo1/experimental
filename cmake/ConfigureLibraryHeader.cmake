if (NOT included_configure_library_header)
    set(included_configure_library_header 1)
else ()
    return()
endif ()

include(CMakeParseArguments)

option(configure_library_header_debug "Debug configure_library_header operations" OFF)

function(_clh_debug_message)
    if (configure_library_header_debug)
        foreach (msg ${ARGN})
            message("configure_library_header: ${msg}")
        endforeach ()
    endif ()
endfunction()

function(_clh_arg_or_error PREFIX ARGNAME)
    _clh_debug_message("argument ${ARGNAME} = ${${PREFIX}_${ARGNAME}}")
    if (NOT ${PREFIX}_${ARGNAME})
        message(FATAL_ERROR "configure_library_header: missing required parameter: ${ARGNAME}")
    endif ()
endfunction()

function(_clh_add_include filename)

    if (arg_INCLUDE_PREFIX)
        set(filename "${arg_INCLUDE_PREFIX}/${filename}")
    endif ()
    set(include_lines
            "${include_lines}${sep}#include <${filename}>" PARENT_SCOPE)
    set(sep "\n" PARENT_SCOPE)
endfunction()

function(configure_library_header outfile)
    set(options)
    set(oneValueArgs TEMPLATE SOURCE_DIR BINARY_DIR INCLUDE_PREFIX)
    set(multiValueArgs HEADERS HEADER_LISTS)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    _clh_arg_or_error(arg TEMPLATE)
    _clh_arg_or_error(arg SOURCE_DIR)
    _clh_arg_or_error(arg BINARY_DIR)
    _clh_debug_message("INCLUDE_PREFIX=${arg_INCLUDE_PREFIX}")

    set(include_lines)
    set(sep)

    _clh_debug_message("HEADER_LISTS=${arg_HEADER_LISTS}")
    foreach (include_file IN LISTS ${arg_HEADER_LISTS})
        string(REGEX MATCH "${arg_SOURCE_DIR}/(.*)" matched "${include_file}")
        if (NOT matched)
            string(REGEX MATCH "${arg_BINARY_DIR}/(.*)" matched "${include_file}")
        endif ()

        if (matched)
            _clh_add_include(${CMAKE_MATCH_1})
        else ()
            message(FATAL_ERROR "failed to match include file: ${include_file}")
        endif ()

    endforeach ()

    _clh_debug_message("HEADERS=${arg_HEADERS}")
    foreach (include_file ${arg_HEADERS})
        string(REGEX MATCH "${arg_SOURCE_DIR}/(.*)" matched "${include_file}")
        if (NOT matched)
            string(REGEX MATCH "${arg_BINARY_DIR}/(.*)" matched "${include_file}")
        endif ()

        if (matched)
            _clh_add_include(${CMAKE_MATCH_1})
        else ()
            message(FATAL_ERROR "failed to match include file: ${include_file}")
        endif ()

    endforeach ()

    _clh_debug_message("include lines=${include_lines}")

    configure_file(${arg_TEMPLATE} ${outfile} @ONLY)

endfunction()
