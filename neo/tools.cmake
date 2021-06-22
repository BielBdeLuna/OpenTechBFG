#idtools########################################################################

file(GLOB COMPILER_INCLUDES tools/compilers/*.h)

file(GLOB COMPILER_AAS_INCLUDES tools/compilers/aas/*.h)
file(GLOB COMPILER_AAS_SOURCES tools/compilers/aas/*.cpp)

file(GLOB COMPILER_DMAP_INCLUDES tools/compilers/dmap/*.h)
file(GLOB COMPILER_DMAP_SOURCES tools/compilers/dmap/*.cpp)

#set(TOOLS_INCLUDES tools/edit_public.h)

#set(TOOLS_COMMON_INCLUDES
#  tools/Tools.h
#  tools/ToolsInput.h)

file(GLOB TOOLS_COMMON_INCLUDES tools/*.h)

file(GLOB TOOLS_COMMON_SOURCES tools/*.cpp)
#set(TOOLS_COMMON_SOURCES
#  tools/Tools.cpp)

file(GLOB EDITOR_INCLUDES tools/editors/*.h)
file(GLOB EDITOR_SOURCES tools/editors/*.cpp)

#file(GLOB EDITOR_LIGHTEDITOR_INCLUDES tools/editors/lightEditor/*.h)
#file(GLOB EDITOR_LIGHTEDITOR_SOURCES tools/editors/lightEditor/*.cpp)

file(GLOB EDITOR_CAMERAEXPLORER_INCLUDES tools/editors/cameraExplorer/*.h)
file(GLOB EDITOR_CAMERAEXPLORER_SOURCES tools/editors/cameraExplorer/*.cpp)

#set(HELPERS_INCLUDES
#	tools/helpers/helpers_public.h)
#set(HELPERS_RENDERSTATS_INCLUDES
#	tools/helpers/renderStats/renderStats.h)
#set(HELPERS_RENDERSTATS_SOURCES
#	tools/helpers/renderStats/renderStats.cpp)

file(GLOB HELPERS_INCLUDES tools/helpers/*.h)
#file(GLOB HELPERS_SOURCES tools/helpers/*.cpp)

file(GLOB HELPERS_RENDERSTATS_INCLUDES tools/helpers/renderStats/*.h)
file(GLOB HELPERS_RENDERSTATS_SOURCES tools/helpers/renderStats/*.cpp)

source_group("tools" FILES ${TOOLS_COMMON_INCLUDES})
source_group("tools" FILES ${TOOLS_COMMON_SOURCES})

source_group("tools\\compilers" FILES ${COMPILER_INCLUDES})

source_group("tools\\compilers\\aas" FILES ${COMPILER_AAS_INCLUDES})
source_group("tools\\compilers\\aas" FILES ${COMPILER_AAS_SOURCES})

source_group("tools\\compilers\\dmap" FILES ${COMPILER_DMAP_INCLUDES})
source_group("tools\\compilers\\dmap" FILES ${COMPILER_DMAP_SOURCES})

source_group("tools\\editors" FILES ${EDITOR_INCLUDES})
#source_group("tools\\editors" FILES ${EDITOR_SOURCES})

#source_group("tools\\editors\\lightEditor" FILES ${EDITOR_LIGHTEDITOR_INCLUDES})
#source_group("tools\\editors\\lightEditor" FILES ${EDITOR_LIGHTEDITOR_SOURCES})

source_group("tools\\editors\\cameraExplorer" FILES ${EDITOR_CAMERAEXPLORER_INCLUDES})
source_group("tools\\editors\\cameraExplorer" FILES ${EDITOR_CAMERAEXPLORER_SOURCES})

source_group("tools\\helpers" FILES ${HELPERS_INCLUDES})
#source_group("tools\\helpers" FILES ${HELPERS_SOURCES})

source_group("tools\\helpers\\renderStats" FILES ${HELPERS_RENDERSTATS_INCLUDES})
source_group("tools\\helpers\\renderStats" FILES ${HELPERS_RENDERSTATS_SOURCES})


set(IDTOOLS_INCLUDES
  ${TOOLS_COMMON_INCLUDES}
  ${COMPILER_INCLUDES}
  ${COMPILER_AAS_INCLUDES} # ${COMPILER_AAS_SOURCES}
  ${COMPILER_DMAP_INCLUDES} # ${COMPILER_DMAP_SOURCES}
  ${EDITOR_INCLUDES}
  #${EDITOR_LIGHTEDITOR_INCLUDES}
  ${EDITOR_CAMERAEXPLORER_INCLUDES} # ${EDITOR_CAMERAEXPLORER_SOURCES}
  ${HELPERS_INCLUDES}
  ${HELPERS_RENDERSTATS_INCLUDES}
  )

set(IDTOOLS_SOURCES
  ${TOOLS_COMMON_SOURCES}
  ${COMPILER_AAS_SOURCES}
  ${COMPILER_DMAP_SOURCES}
  #${EDITOR_SOURCES}
  #${EDITOR_LIGHTEDITOR_SOURCES}
  ${EDITOR_CAMERAEXPLORER_SOURCES}
  #${HELPERS_SOURCES}
  ${HELPERS_RENDERSTATS_SOURCES}
  )
