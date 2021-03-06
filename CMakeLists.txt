cmake_minimum_required(VERSION 3.7)

project(OpenSDG)

include(GNUInstallDirs)

include_directories(include)
set(PUBLIC_INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/opensdg.h)

if (MSVC)
  # Microsoft has their own ideas on C standards, shut them up
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

  option(STATIC_BUILD "STATIC_BUILD" ON)

  if (STATIC_BUILD)
  	add_compile_definitions(OPENSDG_STATIC)
	# In case we are building static libraries, link also the runtime library statically
	# so that MSVCR*.DLL is not required at runtime.
	# https://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx
	# This is achieved by replacing msvc option /MD with /MT and /MDd with /MTd
	# https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#how-can-i-build-my-msvc-application-with-a-static-runtime
	foreach(flag_var
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
		CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE 
		CMAKE_C_FLAGS_MINSIZEREL CMAKE_FLAGS_RELWITHDEBINFO)
	  if(${flag_var} MATCHES "/MD")
		string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	  endif(${flag_var} MATCHES "/MD")
	endforeach(flag_var)
  endif (STATIC_BUILD)
else (MSVC)
  option(STATIC_BUILD "STATIC_BUILD" OFF)
endif (MSVC)

add_subdirectory(library)
add_subdirectory(jni)
add_subdirectory(testapp)

install(FILES ${PUBLIC_INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

