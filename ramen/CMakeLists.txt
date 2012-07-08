######################################################
# Base

SET( RAMEN_BASE_SOURCES ../base/base/empty.cpp
                        ../base/base/name.cpp
                        ../base/base/ref_counted.cpp
                        ../base/base/typeinfo.cpp

                        ../base/base/cuda/cuda.cpp
                        ../base/base/cuda/cudart.cpp
                        ../base/base/cuda/driver_exceptions.cpp
                        ../base/base/cuda/exceptions.cpp
                        ../base/base/cuda/runtime_exceptions.cpp

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

SET( RAMEN_MOC_CLASSES  ui/user_interface.hpp
						)

SET( RAMEN_SOURCES  anim/any_curve.cpp
					anim/clipboard.cpp
					anim/curve_algorithm.cpp
					anim/curve_to_half_lut.cpp
					anim/expression_curve.cpp
					anim/float_curve.cpp
					anim/float_key.cpp
					anim/keyframe.cpp
					anim/track.cpp
					anim/util.cpp

					app/application.cpp
					app/document.cpp
					app/plugin_manager.cpp
					app/preferences.cpp

					expressions/expression.cpp

					dependency/graph.cpp
					dependency/node.cpp
					dependency/exceptions.cpp

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

					params/animated_param.cpp
					params/animated_param_command.cpp
					params/aspect_ratio_param.cpp
					params/bool_param.cpp
					params/button_param.cpp
					params/color_param.cpp
					params/combo_group_param.cpp
					params/composite_param.cpp
					params/curve_param.cpp
					params/expression_commands.cpp
					params/file_param.cpp
					params/float_param.cpp
					params/float2_param.cpp
					params/float3_param.cpp
					params/group_param.cpp
					params/motion_blur_info.cpp
					params/motion_blur_param.cpp
					params/numeric_param.cpp
					params/ocio_colorspace_param.cpp
					params/param.cpp
					params/param_set.cpp
					params/parameterised.cpp
					params/popup_param.cpp
					params/proportional_param.cpp
					params/separator_param.cpp
					params/static_param.cpp
					params/static_param_command.cpp
					params/string_param.cpp
					params/tab_group_param.cpp

                    python/export_ramen.cpp
                    python/interpreter.cpp
					python/pyside.cpp

					serialization/yaml.cpp
					serialization/yaml_iarchive.cpp
					serialization/yaml_node.cpp
					serialization/yaml_oarchive.cpp

					ui/events.cpp
					ui/graph_layout.cpp
					ui/user_interface.cpp

					undo/command.cpp
					undo/stack.cpp

					util/calculator.cpp
					util/error_log.cpp
					util/string.cpp

					system/system.cpp
					)

# tests
IF( CMAKE_BUILD_TYPE MATCHES Debug)
	SET( RAMEN_TEST_SOURCES	test/test.cpp
							test/container.cpp
							test/depgraph.cpp
							test/expressions.cpp
							test/math.cpp
							test/memory.cpp
							test/filesystem.cpp
							test/ImathExt.cpp
							test/nodes.cpp
							test/params.cpp
							test/param_set.cpp
							test/python.cpp
							test/string.cpp
							test/test_node.cpp
							test/undo.cpp
							)

	SET( RAMEN_TEST_LIBRARIES)
ENDIF()

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

# ui files
SET( RAMEN_DESIGNER_FILES )

# QT
QT4_WRAP_CPP( RAMEN_MOC_SOURCES	${RAMEN_MOC_CLASSES} OPTIONS ${MOC_EXTRA_ARGS})
QT4_WRAP_UI( RAMEN_DESIGNER_SOURCES ${RAMEN_DESIGNER_FILES})
QT4_ADD_RESOURCES( RAMEN_RESOURCES )

SET( RAMEN_LIB_SOURCES	${RAMEN_BASE_SOURCES}
                        ${RAMEN_SOURCES}
						${RAMEN_MOC_SOURCES}

						${RAMEN_DESIGNER_SOURCES}
						${RAMEN_PLATFORM_SOURCES}
						${RAMEN_TEST_SOURCES}
						${RAMEN_RESOURCES}
						)

# Optionals
IF( WITH_NODES)
	QT4_WRAP_CPP( RAMEN_MOC_NODE_SOURCES ${RAMEN_MOC_NODE_CLASSES} OPTIONS ${MOC_EXTRA_ARGS})
	SET( RAMEN_LIB_SOURCES	${RAMEN_LIB_SOURCES}
							${RAMEN_NODE_SOURCES}
							${RAMEN_MOC_NODE_SOURCES}
							)
ENDIF()

# Ramen lib

ADD_LIBRARY( ramen SHARED ${RAMEN_LIB_SOURCES})
SET_TARGET_PROPERTIES( ramen PROPERTIES COMPILE_FLAGS "-DRAMEN_MAKING_DSO -DBASE_MAKING_DSO"
                                        VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH}
										SOVERSION ${RAMEN_VERSION_MAJOR}
                        )
TARGET_LINK_LIBRARIES( ramen	${RAMEN_ALL_LIBS}
								${EXTRA_PLATFORM_LIBS}
								${RAMEN_TEST_LIBRARIES}
								)

SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/lib/)

######################################################
# Ramen app

SET( RAMEN_APP_SOURCES  main.cpp
						assert.cpp
						)

ADD_EXECUTABLE( ramen.bin ${RAMEN_APP_SOURCES})

TARGET_LINK_LIBRARIES( ramen.bin	ramen
									${RAMEN_ALL_LIBS}
									${EXTRA_PLATFORM_LIBS}
									${BREAKPAD_CLIENT_LIBRARY}
									${RAMEN_TEST_LIBRARIES}
									)

SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/bin/)
