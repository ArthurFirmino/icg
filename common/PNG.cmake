find_package(PNG)

if(NOT PNG_FOUND)

    message(WARNING "libpng not found")

else(NOT PNG_FOUND)

    add_definitions(-DUSE_PNG)

    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})
    list(APPEND COMMON_LIBS ${PNG_LIBRARIES})

endif(NOT PNG_FOUND)
