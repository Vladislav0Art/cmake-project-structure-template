cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProjectUtils CXX)

set(SERVICES_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/AuthServiceImpl/auth-service-impl.cpp

	PARENT_SCOPE
)