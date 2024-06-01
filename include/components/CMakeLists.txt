cmake_minimum_required(VERSION 3.15)

project(SampleCmakeProjectComponents CXX)

set(COMPONENTS_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ServerInterceptor/server-interceptor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ClientInterceptor/client-interceptor.cpp


	PARENT_SCOPE
)