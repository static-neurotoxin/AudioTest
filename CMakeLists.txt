CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
SET(PROJECTNAME audiotest)
PROJECT(${PROJECTNAME})

FIND_PACKAGE(Boost 1.40 COMPONENTS program_options filesystem REQUIRED)

INCLUDE(CMakeToolsHelpers OPTIONAL)

SET(SOURCES
        AudioWriter.cpp
        main.cpp
        )

SET(HEADERS
        AudioWriter.h
        AudioGenerator.h)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(${PROJECTNAME} ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(${PROJECTNAME} Boost::program_options Boost::filesystem)

SET_PROPERTY(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 14)
TARGET_COMPILE_FEATURES(${PROJECTNAME} PRIVATE
        cxx_auto_type
        cxx_deleted_functions
        cxx_lambdas
        cxx_nullptr
        cxx_range_for
        cxx_return_type_deduction
        cxx_right_angle_brackets
        cxx_variadic_templates
        )
