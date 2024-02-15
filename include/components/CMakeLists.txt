cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProjectComponents CXX)

set(COMPONENTS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/HelloWorldComponent/hello-world-component.cpp

	PARENT_SCOPE
)