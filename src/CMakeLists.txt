ADD_EXECUTABLE(tuxtower
  main.c
  support.c
  support.h
  load.c
  load.h
  data_model.h
  constants.h)
TARGET_LINK_LIBRARIES(tuxtower
  ${SDL_LIBRARY}
  ${SDLTTF_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  ${SDLMIXER_LIBRARY}
)

INSTALL(TARGETS tuxtower
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
