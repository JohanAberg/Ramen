PROJECT( ramen)

SET( RAMEN_VERSION_MAJOR "2")
SET( RAMEN_VERSION_MINOR "0")
SET( RAMEN_VERSION_PATCH "0")
SET( RAMEN_VERSION ${RAMEN_VERSION_MAJOR}.${RAMEN_VERSION_MINOR}.${RAMEN_VERSION_PATCH})

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.0 FATAL_ERROR)
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/base/cmake/Modules)
SET( CMAKE_SKIP_RPATH ON)

#disable in source builds
IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	MESSAGE(FATAL_ERROR "CMake generation for Ramen is not allowed within the source directory!")
ENDIF()

####################################################################
# edit the paths if needed

SET( CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH}
						/usr/include/jpeg
						/usr/include/tiff
						)

SET( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})

####################################################################
# options

####################################################################
# compiler & linker settings

SET( CMAKE_SKIP_RPATH ON)
SET( CMAKE_SKIP_INSTALL_RPATH ON)

# Set the default built type
IF( NOT CMAKE_BUILD_TYPE)
  SET( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF()

IF( CMAKE_COMPILER_IS_GNUCXX)
    SET( RAMEN_COMMON_CXX_FLAGS "-rdynamic -pipe -fvisibility=hidden -fvisibility-inlines-hidden")
    #SET( RAMEN_COMMON_CXX_FLAGS "${RAMEN_COMMON_CXX_FLAGS} -std=c++0x")

    SET( RAMEN_COMMON_EXE_LINKER_FLAGS "-Wl,--export-dynamic")

    IF( CMAKE_BUILD_TYPE MATCHES Release)
        SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RAMEN_COMMON_CXX_FLAGS}")
        SET( CMAKE_EXE_LINKER_FLAGS_RELEASE "${RAMEN_COMMON_EXE_LINKER_FLAGS}")
    ELSE()
        IF( CMAKE_BUILD_TYPE MATCHES Debug)
            SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${RAMEN_COMMON_CXX_FLAGS} -fno-inline")
            SET( CMAKE_EXE_LINKER_FLAGS_DEBUG "{RAMEN_COMMON_EXE_LINKER_FLAGS}")
        ENDIF()
    ENDIF()
ELSEIF( CMAKE_C_COMPILER_ID STREQUAL "Clang")

ENDIF()

ADD_DEFINITIONS( -DRAMEN_ENABLE_ASSERT_HANDLER)

####################################################################
# dependencies

# GL
FIND_PACKAGE( OpenGL REQUIRED)
FIND_PACKAGE( GLEW REQUIRED)

# boost
SET( Boost_ADDITIONAL_VERSIONS "1.49.0")
SET( BOOST_NEEDED_LIBS  filesystem
						thread
						signals
						system
						regex
						date_time
						program_options
						python
						unit_test_framework
						)

FIND_PACKAGE( Boost COMPONENTS ${BOOST_NEEDED_LIBS} REQUIRED)
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_VERSION=3)
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_NO_DEPRECATED)

# base libs
FIND_PACKAGE( TBB REQUIRED)
FIND_PACKAGE( YamlCpp REQUIRED)

# cuda
#FIND_PACKAGE( CUDA REQUIRED)

# Remove libcuda from CUDA_LIBRARIES ( as it's part of the nvidia driver.)
#SET( CUDA_LIBRARIES ${CUDA_CUDART_LIBRARY})
#ADD_DEFINITIONS( -DBASE_WITH_GL)

#FIND_PACKAGE( Thrust REQUIRED)

# Qt
SET( QT_MT_REQUIRED false)
SET( QT_MIN_VERSION "4.0.0")
FIND_PACKAGE( Qt4 REQUIRED)
ADD_DEFINITIONS( -DQT_NO_KEYWORDS)
SET( MOC_EXTRA_ARGS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

# imageio libraries
FIND_PACKAGE( OpenEXR REQUIRED)
#FIND_PACKAGE( JPEG REQUIRED)
#FIND_PACKAGE( PNG REQUIRED)
#FIND_PACKAGE( TIFF REQUIRED)
#FIND_PACKAGE( OpenImageIO REQUIRED)

FIND_PACKAGE( OpenColorIO REQUIRED)
#FIND_PACKAGE( CIMG REQUIRED)

# more libs
#FIND_PACKAGE( AGG REQUIRED)
FIND_PACKAGE( Eigen REQUIRED)
#FIND_PACKAGE( Freetype REQUIRED)
FIND_PACKAGE( ZLIB REQUIRED)
#FIND_PACKAGE( SeExpr REQUIRED)

# python
FIND_PACKAGE( PythonLibs REQUIRED)
FIND_PACKAGE( Shiboken REQUIRED)
FIND_PACKAGE( PySide REQUIRED)

INCLUDE_DIRECTORIES(    ${PROJECT_SOURCE_DIR}
						${PROJECT_SOURCE_DIR}/base/
						${PROJECT_SOURCE_DIR}/ramen/

                        ${QT_INCLUDE_DIR}
                        ${QT_QTCORE_INCLUDE_DIR}

						${Boost_INCLUDE_DIR}
						${TBB_INCLUDE_DIRS}
						${YAMLCPP_INCLUDE_DIRS}

						${OPENGL_INCLUDE_DIR}
						${GLEW_INCLUDE_DIRS}
                        ${CUDA_INCLUDE_DIRS}
                        ${THRUST_INCLUDE_DIRS}

						${OPENEXR_INCLUDE_DIR}
						${OPENEXR_INCLUDE_DIR}/OpenEXR/

						${JPEG_INCLUDE_DIR}
						${PNG_INCLUDE_DIR}
						${TIFF_INCLUDE_DIR}
						${OPENIMAGEIO_INCLUDE_DIRS}

						${OPENCOLORIO_INCLUDE_DIRS}

						${CIMG_INCLUDE_DIRS}

						${EIGEN_INCLUDE_DIRS}
						${AGG_INCLUDE_DIRS}
						${FREETYPE_INCLUDE_DIRS}

						${SEEXPR_INCLUDE_DIRS}

						${PYTHON_INCLUDE_DIRS}

						${ZLIB_INCLUDE_DIRS}
						${BREAKPAD_INCLUDE_DIRS}
						)

SET( RAMEN_ALL_LIBS	${QT_QTCORE_LIBRARIES}

					${OPENCOLORIO_LIBRARIES}

					${OPENIMAGEIO_LIBRARIES}
					${OPENEXR_LIBRARIES}
					${JPEG_LIBRARIES}
					${PNG_LIBRARIES}
					${TIFF_LIBRARIES}

                    ${CUDA_LIBRARIES}
					${GLEW_LIBRARIES}
					${OPENGL_LIBRARIES}

					${Boost_LIBRARIES}
					${TBB_LIBRARIES}
					${YAMLCPP_LIBRARIES}

					${AGG_LIBRARIES}
					${FREETYPE_LIBRARIES}

					${SEEXPR_LIBRARIES}

					${PYTHON_LIBRARIES}

					${ZLIB_LIBRARIES}
					)

ADD_SUBDIRECTORY( ramen)
ADD_SUBDIRECTORY( ramen_ui)

######################################################
# Ramen bin
######################################################

FILE( GLOB_RECURSE RAMEN_PY_SOURCES python/*.py )

ADD_EXECUTABLE( ramen.bin   ramen/main.cpp
                            ramen/assert.cpp
                            ${RAMEN_PY_SOURCES}
                            )

TARGET_LINK_LIBRARIES( ramen.bin	ramen
									${RAMEN_ALL_LIBS}
									${EXTRA_PLATFORM_LIBS}
									${BREAKPAD_CLIENT_LIBRARY}
									${RAMEN_TEST_LIBRARIES}
									)

SET_TARGET_PROPERTIES( ramen.bin PROPERTIES
                       RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/ramen-${RAMEN_VERSION}/bin
                        )


##########################################################
### Docs
##########################################################

# Docs
FIND_PACKAGE( Doxygen)

IF( DOXYGEN_FOUND)
	CONFIGURE_FILE( ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)
	ADD_CUSTOM_TARGET( doc ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
						COMMENT "Generating API documentation with Doxygen" VERBATIM)
ENDIF()
