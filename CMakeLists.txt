CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(proxyme)

# The version number.
exec_program(
  "git"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ARGS "name-rev --tags --name-only $(git rev-parse HEAD)"
  OUTPUT_VARIABLE proxyme_VERSION )
set (proxyme_VERSION  ${proxyme_VERSION})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

FIND_PACKAGE(Boost COMPONENTS system filesystem program_options regex)

###################################################################################
# - Try to find ctemplate
# Once done this will define
#  LIBCTEMPLATE_FOUND - System has ctemplate
#  LIBCTEMPLATE_INCLUDE_DIRS - The ctemplate include directories
#  LIBCTEMPLATE_LIBRARIES - The libraries needed to use ctemplate
#  LIBCTEMPLATE_DEFINITIONS - Compiler switches required for using ctemplate

find_package(PkgConfig)
pkg_check_modules(PC_LIBCTEMPLATE QUIET libctemplate)
set(LIBCTEMPLATE_DEFINITIONS ${PC_LIBCTEMPLATE_CFLAGS_OTHER})

find_path(LIBCTEMPLATE_INCLUDE_DIR ctemplate/template.h
          HINTS ${PC_LIBCTEMPLATE_INCLUDEDIR} ${PC_LIBCTEMPLATE_INCLUDE_DIRS}
          PATH_SUFFIXES ctemplate )

find_library(LIBCTEMPLATE_LIBRARY NAMES ctemplate
             HINTS ${PC_LIBCTEMPLATE_LIBDIR} ${PC_LIBCTEMPLATE_LIBRARY_DIRS} )

set(LIBCTEMPLATE_LIBRARIES ${LIBCTEMPLATE_LIBRARY} )
set(LIBCTEMPLATE_INCLUDE_DIRS ${LIBCTEMPLATE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibCtemplate  DEFAULT_MSG
                                  LIBCTEMPLATE_LIBRARY LIBCTEMPLATE_INCLUDE_DIR)

mark_as_advanced(LIBCTEMPLATE_INCLUDE_DIR LIBCTEMPLATE_LIBRARY )
###################################################################################

find_program(CTEMPLATE_MAKE_TPL_VARNAMES_H ctemplate-make_tpl_varnames_h
  $ENV{HOME}/bin
  /usr/local/bin
  /usr/bin
  NO_DEFAULT_PATH
)

ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_BINARY_DIR}/proxyme.tpl.varnames.h
                   COMMAND ${CTEMPLATE_MAKE_TPL_VARNAMES_H} ${PROJECT_SOURCE_DIR}/templates/proxyme.tpl
                   COMMENT "Building header file with ctemplate definitions"
)

ADD_EXECUTABLE(proxyme src/main.cpp proxyme.tpl.varnames.h config.h)

target_link_libraries(
	proxyme
	${Boost_LIBRARIES}
	${LIBCTEMPLATE_LIBRARIES}
)

set(proxyme_templates
  templates/proxyme.ini
  templates/apt.tpl
  templates/curl.tpl
  templates/git.tpl
  templates/maven.tpl
  templates/npm.tpl
  templates/subversion.tpl
  templates/wget.tpl
)

install (TARGETS proxyme DESTINATION bin)
install (FILES ${proxyme_templates} DESTINATION share/proxyme)
