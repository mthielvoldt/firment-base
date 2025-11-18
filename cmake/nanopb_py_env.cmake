
find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(PYTHON_PACKAGES
  "grpcio-tools"
)

# Placing venv alongside binary dir, I can avoid the delay of remaking it by 
# deleting the binary dir, but not the venv dir. The venv seldom changes.
# my build dir is 2-layers deep (bin/fw) so the source is still pristine.
set(VENV_DIR "${PROJECT_BINARY_DIR}/_venv")
if(WIN32)
  set(VENV_ACTIVATE "${VENV_DIR}/Scripts/activate")
else()
  set(VENV_ACTIVATE . ${VENV_DIR}/bin/activate)
endif()


add_custom_command(
  OUTPUT ${VENV_DIR}
  COMMAND python3 -m venv "${VENV_DIR}" &&
    ${VENV_ACTIVATE} &&
    python -m pip install ${PYTHON_PACKAGES}
)
