cmake_minimum_required(VERSION 3.16)
project(dual_camera_stream_multithread)

# -------------------------- [ SET OPENCV DIRECTORY PATH TO POINT TOWARDS OpenCVConfig.cmake FILE ] -------------------------- #
# Windows path
# set(OpenCV_DIR "E:/Coding Folder/OpenCV Folder/build/install/x64/vc16/lib")  # Specify the path to the OpenCVConfig.cmake file
# RPI Path NOT NEEDED
# set(OpenCV_DIR "/home/pi/OpenCV/build/unix-install")
# Jetson Nano path
# Jetson Nano path NOT NEEDED
# set(OpenCV_DIR "~/opencv/build")

# -------------------------- [ SET OpenCV_INCLUDE_DIRS TO POINT TOWARDS LIBRARIES ] -------------------------- #
# Windows path
# set(OpenCV_INCLUDE_DIRS "E:/Coding Folder/OpenCV Folder/build/install/x64/vc16/lib")
# RPI Path
# set(OpenCV_INCLUDE_DIRS, "~/opencv/include/opencv2/")

# set(OpenCV_INCLUDE_DIRS, "~/opencv/include/opencv2/")
# Jetson Nano path
set(OpenCV_INCLUDE_DIRS "~/opencv/build/include")

# -------------------------- [ SET OpenCV_Version to 4.7 ] -------------------------- #
set(OpenCV_VERSION "4.7")

# set(OpenCV_LIBS "E:/Coding Folder/OpenCV Folder/build/install/x64/vc16/lib")
message(STATUS "                                                                         ")
message(STATUS "-------------------------------------------------------------------------")
message(STATUS "    ******** OpenCV library status ********")
message(STATUS "    config directory OpenCV_DIR          : ${OpenCV_DIR}")
message(STATUS "    OpenCV_VERSION                       : ${OpenCV_VERSION}")
message(STATUS "    OpenCV_LIBS                          : ${OpenCV_LIBS}")
message(STATUS "    OpenCV_INCLUDE_DIRS include path     : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "-------------------------------------------------------------------------")
message(STATUS "                                                                         ")

# Find OpenCV
find_package(OpenCV REQUIRED)
include(CTest)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(dual_camera_stream_multithread dual_camera_stream_multithread.cpp) # THIS WORKS

# add_executable(dual_camera_stream ${SOURCES})

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Link OpenCV libraries | Automatically ilnks against necessary OpenCV libraries based on find_package(OpenCV REQUIRED) command
target_link_libraries(dual_camera_stream_multithread PRIVATE ${OpenCV_LIBS})

# Multithread library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(dual_camera_stream_multithread PRIVATE Threads::Threads)

# Additional directory for the linker
# link_directories("E:/Coding Folder/OpenCV Folder/build/lib/Release")
