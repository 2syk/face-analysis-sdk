# -*-cmake-*-
cmake_policy (SET CMP0020 NEW)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTORCC ON) # Instruct CMake to run rcc automatically when needed
set(CMAKE_AUTOUIC ON) # Instruct CMake to run uic automatically when needed

find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
# add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#INCLUDE( ${QT_USE_FILE} )

#set(CMAKE_VERBOSE_MAKEFILE true)

SET(BundleName "qt-gui")

SET(guiFILES "application-states.cpp"
  "controllers.cpp"
  "command-line-configuration.cpp"
  "configuration.cpp"
  "gui/avatar-selection.cpp"
  "gui/main-window.cpp"
  "gui/avatar.cpp"
  "gui/mesh-drawer.cpp"
  "gui/graphics-scrollbar.cpp"
  "gui/windowed-gui-controller.cpp"
  "gui/item-controllers.cpp"
  "gui/worker-thread.cpp"
  "gui/item-positions-calculator.cpp"
)

SET(MOC_HEADERS
  "controllers.hpp"
  "gui/avatar-selection.hpp"
  "gui/main-window.hpp"
  "gui/avatar.hpp"
#  "gui/mesh-drawer.hpp"
  "gui/graphics-scrollbar.hpp"
  "gui/windowed-gui-controller.hpp"
  "gui/item-controllers.hpp"
  "gui/worker-thread.hpp")
#  "gui/item-positions-calculator.hpp")

#QT5_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})

INCLUDE_DIRECTORIES(".")

# Non-Apple users get the Foundation librariries from GnuStep
IF(UNIX AND NOT APPLE)
  INCLUDE_DIRECTORIES(/usr/local/include/GNUstep /usr/include/GNUstep)
  EXECUTE_PROCESS(COMMAND gnustep-config --objc-flags
    OUTPUT_VARIABLE GnuStep_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS(COMMAND gnustep-config --base-libs
    OUTPUT_VARIABLE GnuStep_LINK_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  STRING(REPLACE "\n" "" GnuStep_FLAGS ${GnuStep_FLAGS})
  STRING(REPLACE "\n" "" GnuStep_LINK_FLAGS ${GnuStep_LINK_FLAGS})

  SET(GnuStep_LINK_FLAGS "-Wl,--no-as-needed ${GnuStep_LINK_FLAGS}")
  set_source_files_properties(osx-configuration.mm PROPERTIES
         COMPILE_FLAGS "${GnuStep_FLAGS}"
         LINKER_FLAGS "${GnuStep_LINK_FLAGS}")
ELSE()
  set_source_files_properties(osx-configuration.mm PROPERTIES COMPILE_FLAGS -ObjC++)
ENDIF()

if(WIN32)
    set(TARGET_TYPE WIN32)
elseif(APPLE)
    set(TARGET_TYPE MACOSX_BUNDLE)
endif()

add_executable(demo-application ${TARGET_TYPE}
  main.cpp
  command-line-configuration.cpp
  ${guiFILES}
  ${MOC_SRCS}
  ${EXTRA_SRC})

set_target_properties(demo-application PROPERTIES DEBUG_POSTFIX "d")  
target_link_libraries(demo-application ${Qt5Widgets_LIBRARIES} ${LIBS} clmTracker avatarAnim utilities ${EXTRA_LIBS})

install(TARGETS demo-application DESTINATION bin)

# Apple Specific Targets

IF(APPLE)
  SET(MACOSX_BUNDLE_BUNDLE_NAME ${BundleName})
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Configuration.plist PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  
  SET(EXTRA_SRC "Configuration.plist")
  SET(CONFIG_SRC "osx-configuration.mm")

ENDIF()

ADD_EXECUTABLE(${BundleName} ${TARGET_TYPE} 
  main.cpp 
  ${CONFIG_SRC}
  ${guiFILES} 
  ${MOC_SRCS}
  ${EXTRA_SRC}
  )
IF(UNIX AND NOT APPLE)
  SET_TARGET_PROPERTIES(${BundleName} PROPERTIES
    LINK_FLAGS "${GnuStep_LINK_FLAGS}")
ENDIF()

set_target_properties(${BundleName} PROPERTIES DEBUG_POSTFIX "d")
TARGET_LINK_LIBRARIES(${BundleName} ${LIBS} ${Qt5Widgets_LIBRARIES} clmTracker avatarAnim ${EXTRA_LIBS})

install(TARGETS ${BundleName} DESTINATION bin)

# IF(APPLE)

#  # set_target_properties(${BundleName} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.plist)
 

# ELSEIF(UNIX)

# ENDIF()


IF(APPLE)

  SET(plugin_dest_dir "${BundleName}.app/Contents/PlugIns")
  SET(qtconf_dest_dir "${BundleName}.app/Contents/Resources")
  SET(APP_FOLDER "\${CMAKE_INSTALL_PREFIX}/${BundleName}.app")
  
  INSTALL(TARGETS ${BundleName}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )
 
  FILE(GLOB trackerFiles "${CMAKE_CURRENT_SOURCE_DIR}/../tracker/resources/*")
  FILE(GLOB avatarFiles "${CMAKE_CURRENT_SOURCE_DIR}/../avatar/resources/*")

  INSTALL(FILES ${trackerFiles} DESTINATION ${qtconf_dest_dir} COMPONENT Runtime)
  INSTALL(FILES ${avatarFiles} DESTINATION ${qtconf_dest_dir} COMPONENT Runtime)

  SET(DIRS ${QT_LIBRARY_DIR} "/usr/local/" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  
  INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APP_FOLDER}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)

  include(CPack)

ENDIF()
