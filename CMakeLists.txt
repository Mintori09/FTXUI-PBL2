cmake_minimum_required(VERSION 3.11)
project(ftxui_starter LANGUAGES CXX VERSION 1.0.0)

# Xuất tệp lệnh biên dịch
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Thiết lập tiêu chuẩn C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thêm thư mục FTXUI
add_subdirectory("ftxui" "ftxui_build")

# Tạo executable từ tệp nguồn
add_executable(ftxui_starter main.cpp)

# Tìm tất cả các tệp nguồn trong thư mục src và thêm vào executable
file(GLOB_RECURSE SOURCES "src/*.cpp" "Menu/*.cpp")
target_sources(ftxui_starter PRIVATE ${SOURCES})

# Liên kết các thư viện của FTXUI
target_link_libraries(ftxui_starter    
    PRIVATE ftxui::screen    
    PRIVATE ftxui::dom    
    PRIVATE ftxui::component
)
