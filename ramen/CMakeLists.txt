######################################################
# Base

SET( RAMEN_BASE_SOURCES ../base/base/empty.cpp
                        ../base/base/name.cpp
                        ../base/base/ref_counted.cpp
                        ../base/base/typeinfo.cpp

                        ../base/base/GL/gl.cpp
                        ../base/base/GL/glu.cpp

                        ../base/base/hash/md5.cpp

                        ../base/base/string/edit_distance.cpp

                        ../base/base/ui/nice_numbers.cpp
                        ../base/base/ui/viewport2.cpp
                        )

######################################################
# Ramen lib

CONFIGURE_FILE( ${PROJECT_SOURCE_DIR}/ramen/version.hpp.in ${PROJECT_SOURCE_DIR}/ramen/version.hpp)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR})

SET( RAMEN_SOURCES  anim/any_curve.cpp
					anim/clipboard.cpp
					anim/curve_algorithm.cpp
					anim/curve_to_half_lut.cpp
					anim/double_curve.cpp
					anim/double_key.cpp
					anim/expression_curve.cpp
					anim/keyframe.cpp
					anim/track.cpp
					anim/util.cpp

					app/application.cpp
					app/document.cpp
					app/plugin_manager.cpp
					app/preferences.cpp

					expressions/expression.cpp

                    dependency/dependency.cpp
					dependency/graph.cpp
					dependency/node.cpp
					dependency/exceptions.cpp
					dependency/sub_graph.cpp

					filesystem/path.cpp
					filesystem/path_sequence.cpp

					ggems/ggveclib.c
					ggems/nearestpoint.c
					ggems/convex_opt.c
					ggems/poly_clip.cpp
					ggems/poly_scan.cpp

					GL/lut3d.cpp
					GL/program.cpp

					half/halfExpLogTable.cpp

					hash/generator.cpp

					manipulators/manipulable.cpp
					manipulators/manipulator.cpp

					memory/manager.cpp

					nodes/composite_node.cpp
					nodes/node.cpp
					nodes/class_metadata.cpp
					nodes/factory.cpp
					nodes/graph.cpp
					nodes/graph_modifier.cpp
					nodes/plug.cpp
					nodes/visitor.cpp
					nodes/world_node.cpp

					ocio/context.cpp
					ocio/manager.cpp
					ocio/gl_display_manager.cpp
					ocio/gl_lut3d.cpp

					params/composite_param.cpp
					params/param.cpp
					params/param_set.cpp
					params/parameterised.cpp

                    python/interpreter.cpp

					ui/events.cpp
					ui/graph_layout.cpp

					undo/command.cpp
					undo/stack.cpp

					util/calculator.cpp
					util/error_log.cpp
					util/string.cpp

					system/system.cpp
					)

# tests
SET( RAMEN_TEST_SOURCES	test/test.cpp
						test/nodes.cpp
						test/string.cpp
						test/test_node.cpp
						)

# platform sources
IF( APPLE)
	SET( RAMEN_PLATFORM_SOURCES system/system_pvt_osx.cpp)
ENDIF()

IF( WIN32)
	SET( RAMEN_PLATFORM_SOURCES system/system_pvt_win.cpp)
	SET( EXTRA_PLATFORM_LIBS Userenv)
ENDIF()

IF( CMAKE_SYSTEM_NAME MATCHES "Linux")
		SET( RAMEN_PLATFORM_SOURCES system/system_pvt_linux.cpp)
ENDIF()

SET( RAMEN_LIB_SOURCES	${RAMEN_BASE_SOURCES}
                        ${RAMEN_SOURCES}

						${RAMEN_PLATFORM_SOURCES}
						${RAMEN_TEST_SOURCES}
						)

######################################################
# Ramen lib

ADD_LIBRARY( ramen SHARED ${RAMEN_LIB_SOURCES})
SET_TARGET_PROPERTIES( ramen PROPERTIES
                        COMPILE_FLAGS "-DBASE_MAKING_DSO"
                        VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH}
                        SOVERSION ${RAMEN_VERSION_MAJOR}
                        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib
                        )
TARGET_LINK_LIBRARIES( ramen	${RAMEN_ALL_LIBS}
								${EXTRA_PLATFORM_LIBS}
								${RAMEN_TEST_LIBRARIES}
								)

######################################################
# Ramen py module

SET( RAMEN_PY_MODULE_SRCS   python/export_application.cpp
                            python/export_composite_node.cpp
                            python/export_document.cpp
                            python/export_manipulable.cpp
                            python/export_node.cpp
                            python/export_param.cpp
                            python/export_parameterised.cpp
                            python/export_ramen.cpp
                            python/export_system.cpp
                            python/export_world_node.cpp
                            )

ADD_LIBRARY( _ramen SHARED ${RAMEN_PY_MODULE_SRCS})

SET_TARGET_PROPERTIES( _ramen PROPERTIES
                       PREFIX ""
                       LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/python/app/ramen
                    )
TARGET_LINK_LIBRARIES( _ramen	ramen
                                ${RAMEN_ALL_LIBS}
								${EXTRA_PLATFORM_LIBS}
								)
