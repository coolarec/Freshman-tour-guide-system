# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include  # 使用 C++17 标准并指定 include 目录
LDFLAGS =  # 链接选项（如果需要，可以添加库等）

# 源文件和目标文件设置
SRC = main.cpp schoolMap.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = my_project

# 默认目标
all: $(EXEC)

# 链接目标文件生成可执行文件
$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $(EXEC)

# 编译 .cpp 文件为 .o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# 清理目标文件
clean:
	rm -f $(OBJ) $(EXEC)
