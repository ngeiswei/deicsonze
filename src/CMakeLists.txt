# Deicsonze
add_subdirectory(deicsonze)

# DSSI plugin
if (HAVE_DSSI)
  add_library(deicsonze-dssi SHARED
    dssideicsonze)

  target_link_libraries(deicsonze-dssi
    deicsonze
    dssiplugin)

  add_subdirectory(dssi)

  add_custom_command(TARGET deicsonze-dssi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:deicsonze-dssi>
    ../deicsonze-dssi.so)

  install(FILES ${CMAKE_BINARY_DIR}/deicsonze-dssi.so DESTINATION lib/dssi)
endif (HAVE_DSSI)

# VST plugin
if (HAVE_VST)
  include_directories("${PROJECT_SOURCE_DIR}/VST3 SDK")

  add_library(deicsonze-vst SHARED
    vstdeicsonze
    "${PROJECT_SOURCE_DIR}/VST3 SDK/public.sdk/source/vst2.x/vstplugmain"
    "${PROJECT_SOURCE_DIR}/VST3 SDK/public.sdk/source/vst2.x/audioeffect"
    "${PROJECT_SOURCE_DIR}/VST3 SDK/public.sdk/source/vst2.x/audioeffectx")

  target_link_libraries(deicsonze-vst
    deicsonze)

endif (HAVE_VST)
