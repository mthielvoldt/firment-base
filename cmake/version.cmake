# Read the version from an external file then strip any whitespace
file(READ "${CMAKE_CURRENT_LIST_DIR}/../version.txt" VERSION_CONTENT)
string(STRIP "${VERSION_CONTENT}" VERSION_SEM)