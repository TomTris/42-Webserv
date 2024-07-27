NAME = a.out
FLAGS = -Wall -Werror -Wextra
SRC = \
	Ob/Class_Server.cpp\
	Ob/get_answer.cpp\
	Ob/parse.cpp\
	Connection.cpp\
	main.cpp\
	Reader.cpp\
	socket_create.cpp\
	Writer.cpp

OBJS = $(SRC:.cpp=.o)

$(NAME) : all

all: $(OBJS)
	c++ $(FLAGS) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all