cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProjectInclude CXX)

# adding subdirs
add_subdirectory(./components)
add_subdirectory(./utils)

# propagating variable from inner subdirs to root level project
set(COMPONENTS_SRCS ${COMPONENTS_SRCS} PARENT_SCOPE)
set(UTILS_SRCS ${UTILS_SRCS} PARENT_SCOPE)
