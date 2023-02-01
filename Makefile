CC	= gcc

RM	= rm -f

NAME= yarsaat

SRC	=	src/main.c \
		src/nvec.c \
		src/product_tree.c \
		src/y_batchgcd.c

OBJ	= $(SRC:.c=.o)

CFLAGS	= -W -Wall -Wextra -g

LDFLAGS	= -lgmp -lm

all:		$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all