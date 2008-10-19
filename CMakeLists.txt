PROJECT( add_toc )
FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE_DIRECTORIES(${KDE4_INCLUDES} .)

SET(add_tocSources add_toc.cpp parser.cpp main.cpp)

KDE4_ADD_EXECUTABLE(add_toc ${add_tocSources})

TARGET_LINK_LIBRARIES(add_toc ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} tidy)

install(TARGETS add_toc  ${INSTALL_TARGETS_DEFAULT_ARGS})
