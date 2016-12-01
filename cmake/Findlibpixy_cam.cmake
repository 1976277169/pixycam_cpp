if (LIBPIXY_CAM_0_LIBRARY AND LIBPIXY_CAM_0_INCLUDE_DIR)
  # In cmake cache already
  set (LIBPIXY_CAM_0_FOUND TRUE)
else (LIBPIXY_CAM_0_LIBRARY AND LIBPIXY_CAM_0_INCLUDE_DIR)
  find_path(INCLUDE_DIR
    NAMES
      pixy.h
      pixy_defs.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(LIBRARY_DIR
    NAMES
      pixy_cam
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  message ("lib ${LIBRARY_DIR}")
  message ("inc ${INCLUDE_DIR}")

  set (LIBPIXY_CAM_0_INCLUDE_DIR ${INCLUDE_DIR})
  set (LIBPIXY_CAM_0_LIBRARY ${LIBRARY_DIR})

  if (LIBPIXY_CAM_0_INCLUDE_DIR AND LIBPIXY_CAM_0_LIBRARY)
    set (LIBPIXY_CAM_0_FOUND TRUE)
  endif (LIBPIXY_CAM_0_INCLUDE_DIR AND LIBPIXY_CAM_0_LIBRARY)

  if (LIBPIXY_CAM_0_FOUND)
    message (STATUS "Found libpixy_cam")
  else (LIBPIXY_CAM_0_FOUND)
    message (FATAL_ERROR "Could not find libpixy_cam")
  endif (LIBPIXY_CAM_0_FOUND)

  mark_as_advanced (LIBPIXY_CAM_0_INCLUDE_DIR LIBPIXY_CAM_0_LIBRARY)
      
endif (LIBPIXY_CAM_0_LIBRARY AND LIBPIXY_CAM_0_INCLUDE_DIR)
