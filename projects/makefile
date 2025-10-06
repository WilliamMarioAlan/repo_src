# ====================================================
#   make         # 编译生成可执行文件
#   make clean   # 清理所有编译产物
# ====================================================

CC := gcc                  # 使用的 C 编译器
CFLAGS := -Wall -Wextra -O2  # 编译选项，打开所有警告，开启优化

TARGET := target            # 最终生成的可执行文件名

SRCS := $(wildcard *.c)    # 当前目录下所有的 .c 文件
OBJS := $(SRCS:.c=.o)      # 对应的目标文件名

.PHONY: all clean

# 默认目标，编译可执行文件
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译 .c 文件为 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译产物
clean:
	rm -f $(OBJS) $(TARGET)

