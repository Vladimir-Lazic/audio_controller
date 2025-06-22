find_path(RTAUDIO_INCLUDE_DIR RtAudio.h
  PATH_SUFFIXES include/rtaudio include
  PATHS /usr/local /usr
)

find_library(RTAUDIO_LIBRARY rtaudio
  PATH_SUFFIXES lib
  PATHS /usr/local /usr
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RtAudio DEFAULT_MSG RTAUDIO_INCLUDE_DIR RTAUDIO_LIBRARY)

mark_as_advanced(RTAUDIO_INCLUDE_DIR RTAUDIO_LIBRARY)
