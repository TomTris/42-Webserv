NAME = Webserve
FLAGS = -Wall -Werror -Wextra
SRC = method_find.cpp socket_create.cpp Webserve.cpp
OBJS = $(SRC:.cpp=.o)

$(NAME) : all

all: $(OBJS)
	c++ $(FLAGS) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all