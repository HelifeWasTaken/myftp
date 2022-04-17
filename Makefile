##
## EPITECH PROJECT, 2022
## myftp
## File description:
## Makefile
##

CC			=		gcc

LDFLAGS		=

WFLAGS 		=		-W -Wall -Wextra -Werror

STDFLAGS 	=		-std=c99

DFLAGS 		=		-D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L

IFLAGS 		=		-I./include

CFLAGS 		=		$(WFLAGS) $(STDFLAGS) $(DFLAGS) $(IFLAGS)

ifeq ($(DEBUG), 1)
	CFLAGS += -Wno-error=init-self -Winit-self
	CFLAGS += -Wno-error=shadow -Wshadow
	CFLAGS += -Wno-error=pointer-arith -Wpointer-arith
	CFLAGS += -Wno-error=duplicated-cond -Wduplicated-cond
	CFLAGS += -Wno-error=switch-enum -Wswitch-enum
	CFLAGS += -Wno-error=declaration-after-statement
	CFLAGS +=-Wdeclaration-after-statement
	CFLAGS += -Wno-error=float-equal -Wfloat-equal
	CFLAGS += -Wno-error=tautological-compare -Wtautological-compare
	CFLAGS += -Wno-error=array-bounds -Warray-bounds
	CFLAGS += -Wno-error=alloc-zero -Walloc-zero
	CFLAGS += -Wno-error=cast-qual -Wcast-qual
	CFLAGS += -Wno-error=extra -Wextra -Wnonnull
	CFLAGS += -fno-builtin
	CFLAGS += -ftrapv -ggdb -g3
	CFLAGS += -DDEBUG
endif

NAME 		=		myftp

MAIN		=		./main.c

SRC_INIT 	=		./src/init/start_server.c 					\
					./src/init/init_ftp_socket.c

SRC_UTILS 	=		./src/utils/usage.c 						\
					./src/utils/ftp_assert.c 					\
					./src/utils/close_server.c 					\
					./src/utils/socket_write.c 					\
					./src/utils/rfc.c

SRC_STR 	=		./src/str/xstrdup.c							\
					./src/str/str_is_num.c 						\
					./src/str/vector_array.c

SRC_ALLOC	=		./src/alloc/alloc_utils.c

SRC_SERVER 	=		./src/server/mrecv.c 							\
					./src/server/select.c 							\
					./src/server/ftp_accept_client.c 				\
					./src/server/server.c 							\
					./src/server/ftp_manage_client.c				\
					./src/server/ftp_disconnect_client.c			\
					./src/server/ftp_client_user_pass_cmd.c			\
					./src/server/ftp_path_client_cwd_cdup_cmd.c 	\
					./src/server/ftp_path_client_cwd_cdup_cmd2.c	\
					./src/server/ftp_client_del_cmd.c 				\
					./src/server/ftp_client_pwd_cmd.c 				\
					./src/server/ftp_client_noop_cmd.c 				\
					./src/server/ftp_client_pasv_cmd.c 				\
					./src/server/ftp_client_port_cmd.c 				\
					./src/server/ftp_client_retr_cmd.c 				\
					./src/server/ftp_client_stor_cmd.c 				\
					./src/server/ftp_client_list_cmd.c 				\
					./src/server/ftp_client_help_cmd.c


SRC			=		$(SRC_INIT)									\
					$(SRC_UTILS)								\
					$(SRC_SERVER) 								\
					$(SRC_STR)									\
					$(SRC_ALLOC)

OBJ_MAIN	=		$(MAIN:.c=.o)

OBJ 		=		$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_MAIN) $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(OBJ_MAIN) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(OBJ_MAIN)

fclean: clean
	rm -f $(NAME)

re: fclean all
