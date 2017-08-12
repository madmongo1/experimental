if (NOT included_report)
    set(included_report 1)
else ()
    return()
endif ()

option(report_enabled "Enable reporting of variables" OFF)

function(report var)
    message(STATUS "reporting ${var}=${${var}}")
endfunction()
