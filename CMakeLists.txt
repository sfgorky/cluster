cmake_minimum_required(VERSION 2.8.9)
include(${SCRIPT_DIR}/CMakeAll.txt)

project("cluster_prj")
message("Project name   : ${PROJECT_NAME}")
message("Project include: ${PROJECT_SOURCE_DIR}")

set(libname "cluster")
set(binname "rcluster")

set(CMAKE_BUILD_TYPE Debug)

# sets the fpic code (it is set automatically for shared libs.
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(.)

set(cluster
    Random.cpp
    Random.h
    GrahamScan.cpp
    GrahamScan.h
    ClusterFunctions.cpp
    ClusterFunctions.h
    ClusterSet.cpp
    ClusterSet.h
    DataSet.cpp
    DataSet.h
    DataSetUtil.cpp
    DataSetUtil.h
    Point.cpp
    Point.h
    computeDBSCAN.cpp
    computeDBSCAN.h
    KMeanTest.cpp
    KMeanTest.h
    KMean.cpp
    KMean.h
)

set(cluster_header
    DataSet.h                                                                   
    DataSetUtil.h
    Point.h
    ClusterSet.h
    ClusterFunctions.h
)

#########################################################################
# Headers to be installed
set(HEADER
    ${clustser_header}
)

########################################################################
# Source code
set(SOURCE 
    ${cluster}
 )


add_library(${libname}         SHARED ${SOURCE} )
add_library(${libname}_static  STATIC ${SOURCE} )

# install the bits to the final location
install(TARGETS ${libname}          DESTINATION ${inst_lib_dir} )
install(TARGETS ${libname}_static   DESTINATION ${inst_lib_dir} )

install(FILES   ${HEADER}   DESTINATION ${inst_include_dir}/${libname})

#########################################################################
# The test code
macro(AddTestCase testname fname)

    add_executable( ${testname} ${fname} )
    target_link_libraries( ${testname} cluster )
    
endmacro(AddTestCase testname fname)

#########################################################################
# The test code

set (ClassTestCaseList
    #    KMean 
    GrahamScan
)

forEach(test ${ClassTestCaseList})
    message("*** Adding test case for: ${test}")
    AddUnitClassTestCase( ${test} "cluster" )
endForEach(test)

#########################################################################

add_executable(${binname}
    ${SOURCE}
    cluster.cpp
)                                                                               

target_link_libraries( ${binname} 
        mylib_static
)  

#install project to the main bin directory
INSTALL(TARGETS ${binname} DESTINATION  ${inst_bin_dir})


                                                                                   
