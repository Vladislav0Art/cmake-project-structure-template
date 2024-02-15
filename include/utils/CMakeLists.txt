cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProjectUtils CXX)

set(UTILS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/PrintingUtility/printing-utility.cpp

	PARENT_SCOPE
)