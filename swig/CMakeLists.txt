find_package(SWIG)

if (${SWIG_FOUND})
    include(${SWIG_USE_FILE})

    # C-Sharp
    add_subdirectory(csharp)
endif ()