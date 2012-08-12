INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}
                     ${QT_QTGUI_INCLUDE_DIR}
                     ${QT_QTOPENGL_INCLUDE_DIR}

					 ${PYSIDE_INCLUDE_DIRS}
					 ${SHIBOKEN_INCLUDE_DIRS}
                    )

SET( RAMEN_UI_LIBS  ${QT_QTGUI_LIBRARIES}
                    ${QT_QTOPENGL_LIBRARIES}

					${PYSIDE_LIBRARIES}
					${SHIBOKEN_LIBRARIES}

					${RAMEN_ALL_LIBS}
					)


SET( RAMEN_UI_LIB_SOURCES   splash_screen.hpp
                            splash_screen.cpp
                            user_interface.hpp
                            user_interface.cpp
                            )

######################################################
# Ramen ui lib

ADD_LIBRARY( ramen_ui SHARED ${RAMEN_UI_LIB_SOURCES})
SET_TARGET_PROPERTIES( ramen_ui PROPERTIES
                        VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH}
                        SOVERSION ${RAMEN_VERSION_MAJOR}
                        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib
                        )
TARGET_LINK_LIBRARIES( ramen_ui	ramen ${RAMEN_UI_LIBS})

######################################################
# Ramen ui py module

SET( RAMEN_UI_PY_MODULE_SRCS    python/export_ramen_ui.cpp
                                python/export_splash_screen.cpp
                                python/export_user_interface.cpp
                                )

ADD_LIBRARY( _ramen_ui SHARED ${RAMEN_UI_PY_MODULE_SRCS})

SET_TARGET_PROPERTIES( _ramen_ui PROPERTIES
                       PREFIX ""
                       LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/python/app/ramen
                    )
TARGET_LINK_LIBRARIES( _ramen_ui	ramen_ui
                                    ramen
                                    ${RAMEN_UI_LIBS}
                                    )
