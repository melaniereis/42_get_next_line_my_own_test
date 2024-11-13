#==============================================================================#
#                           GET_NEXT_LINE PROJECT                              #
#==============================================================================#

# Main target names
NAME = get_next_line_tester.out
NAME_BONUS = get_next_line_tester_bonus.out

# Buffer sizes to test
BUFFER_SIZES = 1 5 32 42 10000000

#------------------------------------------------------------------------------#
#                                COLORS & STYLES                               #
#------------------------------------------------------------------------------#

# ANSI color codes for prettier output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m
RESET = \033[0m

# Text style codes
BOLD = \033[1m
DIM = \033[2m
ITALIC = \033[3m
UNDERLINE = \033[4m

# Emojis for visual feedback
CHECK = ✓
CLEAN = 🧹
BUILD = 🔨
ROCKET = 🚀
BOOK = 📚
SPARKLES = ✨

#------------------------------------------------------------------------------#
#                            	  NAMES AND PATHS                              #
#------------------------------------------------------------------------------#

# Directory structure
BUILD_PATH = .build
SRC_PATH = ../.
BONUS_PATH = ../.
INC_PATH = ../.
INC_PATH_BONUS = ../.
FILES_PATH = ./files/
HEADERS = ${INC_PATH}/get_next_line.h
HEADERS_BONUS = ${INC_PATH}/get_next_line_bonus.h

# Source files for main library
SRCS = ${addprefix ${SRC_PATH}/, get_next_line.c get_next_line_utils.c}
# Object files derived from source files
OBJS = ${addprefix ${BUILD_PATH}/, ${notdir ${SRCS:.c=.o}}}
# Source files for bonus library
SRCS_BONUS = ${addprefix ${BONUS_PATH}/, get_next_line_bonus.c \
			 get_next_line_utils_bonus.c}
# Object files derived from bonus source files
OBJS_BONUS = ${addprefix ${BUILD_PATH}/, ${notdir ${SRCS_BONUS:.c=.o}}}
FILES = big_line_no_nl.txt empty.txt hello-whirl.txt \
judgement-night.txt mini-vulf.txt quality-over-opinion.txt test.txt zzz.txt \
big_line_with_nl.txt excuse-me.txt into-the-void.txt kglw.txt onechar.txt \
read_error.txt vulf.txt

#------------------------------------------------------------------------------#
#                            	   FLAGS & COMMANDS                            #
#------------------------------------------------------------------------------#

CC = cc                           # Compiler to use
CCFLAGS = -Wall -Wextra -Werror   # Compiler flags for warnings and errors
BUFFFLAGS = -D BUFFER_SIZE=
RM = rm -fr                       # Command to remove files/directories forcefully
MKDIR_P = mkdir -p                # Command to create directories (with parent)
INC = -I ${INC_PATH}              # Include path for header file

#------------------------------------------------------------------------------#
#                                    RULES                                     #
#------------------------------------------------------------------------------#

all:
	@for size in ${BUFFER_SIZES}; do \
		printf "\n${CYAN}${BOLD}Testing with BUFFER_SIZE=$$size${RESET}\n"; \
		${MAKE} -s test BUFFER_SIZE=$$size; \
	done

test:
	@${CC} ${CCFLAGS} ${BUFFFLAGS}${BUFFER_SIZE} main.c ${SRCS} -o ${NAME}
	@printf "${YELLOW}${BOLD}Running tests...${RESET}\n"
	@for file in ${FILES}; do \
		printf "\n${PURPLE}${BOLD}Testing file: $$file${RESET}\n"; \
		./${NAME} $$file; \
	done
	@${RM} ${NAME}

bonus:
	@for size in ${BUFFER_SIZES}; do \
		printf "\n${CYAN}${BOLD}Testing bonus with BUFFER_SIZE=$$size${RESET}\n"; \
		${MAKE} -s test_bonus BUFFER_SIZE=$$size; \
	done

test_bonus:
	@${CC} ${CCFLAGS} ${BUFFFLAGS}${BUFFER_SIZE} main_bonus.c ${SRCS_BONUS} -o ${NAME_BONUS}
	@printf "${YELLOW}${BOLD}Running tests...${RESET}\n"
	@./${NAME_BONUS} ${FILES}
	@${RM} ${NAME_BONUS}

clean:

fclean: clean
	@${RM} ${NAME} ${NAME_BONUS}

re: fclean all

.PHONY: all test clean fclean re bonus test_bonus run_single_test