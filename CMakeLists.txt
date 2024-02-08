# cmake版本
cmake_minimum_required(VERSION 3.5.1)
# 项目名称
project(xtinygui)
# 打印消息
message(--------${PROJECT_NAME}-----)
# 设置路径
set(XXX_PATH /home/xxx/xxx)
 
include_directories(
    ${LIB_PATH}/xxx/include  #头文件目录
)
link_directories(
    ${LIB_PATH}/xxx/lib   #库目录
)
# 链接库
link_libraries(
    ${XXX_PATH}/xxx/xxx/xxx.so
    ${XXX_PATH}/xxx/xxx/xxx.so
)