# 参考: https://zhuanlan.zhihu.com/p/430029724
###################################################################################################
# 编译工具链设置
TOOL_CHAIN = 
CC = ${TOOL_CHAIN}gcc
AR = ${TOOL_CHAIN}ar

DEBUG_BUILD ?= yes

# SHOW_COMMAND=yes，显示编译命令
# ifeq (${SHOW_COMMAND}, yes)
# QUIET :=
# else
# QUIET := @
# endif
###################################################################################################
# 目录设置
# 当前路径
PWD_PATH = $(abspath .)

#####################################################################################
# 源文件.c
SRC := ${PWD_PATH}/*.c
# 展开*匹配，获取所有源文件完整路径
SRC := $(wildcard ${SRC})

#####################################################################################
# 头文件路径设置
INCLUDE_PATH += ${PWD_PATH}

###################################################################################################
# 编译选项
CFLAGS := -Wall -g


###################################################################################################
OBJ := $(patsubst %.c,%.o,${SRC})
OBJS_D := $(patsubst %.c,%.d,$(SRC))
# 头文件存放路径设置
INC := $(foreach path,${INCLUDE_PATH},-I${path})

# 生成目标
TARGET := test

.PHONY: all clean
${TARGET}:${OBJ}
	@echo "[Linking $@]"
	${QUIET}${CC} ${OBJ} ${CFLAGS} -o $@ 

-include $(OBJS_D)
$(OBJ):%.o:%.c
	@echo "[Compiling $@]"
	${QUIET}$(CC) $(CPPFLAGS) $(CFLAGS)  -MM -MT $@ -MF $(patsubst %.o, %.d, $@) $<
	${QUIET}${CC} -c $< -o $@ ${CFLAGS}

clean:
	@echo "[cleaning ${TARGET}]"
	${QUIET}rm -rf ${TARGET}
	${QUIET}rm -rf *.o
	${QUIET}rm -rf *.d