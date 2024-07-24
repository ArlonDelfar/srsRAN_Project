
project(entity_expired)

# Set the source files
set(SOURCES
  rlc_rx_um_entity_expired_wrapper.cpp
  rlc_rx_um_entity_expired.c
)

# Add the library
add_library(entity_expired STATIC ${SOURCES})