cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

set(AWS_LAMBDA_CMAKE "/home/eric/out/lib/aws-lambda-runtime/cmake/")
set(CMAKE_PREFIX_PATH "${AWS_LAMBDA_CMAKE};${CMAKE_PREFIX_PATH}")

find_package(aws-lambda-runtime REQUIRED)
find_package(AWSSDK COMPONENTS s3 sqs)


add_executable(CalcEngine.Lambda "main.cpp")
include_directories(${CMAKE_SOURCE_DIR}/CalcEngine)
target_link_libraries(CalcEngine.Lambda PUBLIC AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES} CalcEngine)
include_directories(/mnt/c/Users/ericm/source/repos/vcpkg/vcpkg/installed/x86-windows/include/ /usr/include)
aws_lambda_package_target(CalcEngine.Lambda)