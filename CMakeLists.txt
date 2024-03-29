cmake_minimum_required(VERSION 2.6)

Project(BilateralFilterDemo)

# Where to copy executables when 'make install' is run
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI(UISrcs MainWindow.ui)
QT4_WRAP_CPP(MOCSrcs MainWindow.h BlurComputationThread.h ComputationThread.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# FIND_PACKAGE(VTK REQUIRED)
# INCLUDE( ${USE_VTK_FILE} )

FIND_PACKAGE(ITK REQUIRED)
INCLUDE( ${USE_ITK_FILE} )

ADD_EXECUTABLE(BilateralFilterDemo main.cpp MainWindow.cpp
BlurComputationThread.cpp ComputationThread.cpp
Helpers.cpp ${MOCSrcs} ${UISrcs})
TARGET_LINK_LIBRARIES(BilateralFilterDemo 
#${VTK_LIBRARIES} 
${QT_LIBRARIES} ${ITK_LIBRARIES})
INSTALL( TARGETS BilateralFilterDemo RUNTIME DESTINATION ${INSTALL_DIR} )
