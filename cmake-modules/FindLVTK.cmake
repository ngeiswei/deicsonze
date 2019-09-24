# Try to find LVTK
# This will define the following variables:
#
#  LVTK_FOUND - Whether LVTK was found.
#  LVTK_INCLUDE_DIRS - LVTK include directories.
#  LVTK_LIBRARIES - LVTK libraries.

include(FindPackageHandleStandardArgs)

find_library(LVTK_LIBRARY
  NAMES
    liblvtk_plugin2.a
  PATHS
    ${_LVTK_LIBDIR}
)

find_path(LVTK_INCLUDE_DIR
  NAMES
    lvtk-2/lvtk/lvtk.hpp
  PATHS
    ${_LVTK_INCLUDEDIR}
 )

set(LVTK_INCLUDE_DIRS
  ${LVTK_INCLUDE_DIR}
)

set(LVTK_LIBRARIES
  ${LVTK_LIBRARY}
)

find_package_handle_standard_args(LVTK DEFAULT_MSG LVTK_LIBRARIES LVTK_INCLUDE_DIRS)
