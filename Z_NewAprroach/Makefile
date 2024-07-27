NAME = a.out
FLAGS = -Wall -Werror -Wextra
SRC = \
	Ob/Class_Server.cpp\
	Ob/get_answer.cpp\
	Ob/parse.cpp\
	main.cpp\
	socket_create.cpp\
	Connection.cpp\
	Reader.cpp\
	Writer.cpp\
	poll_relevant.cpp\
	level/server_level.cpp
	# connection_level.cpp\
	# read_level.cpp\
	# write_level.cpp


OBJS = $(SRC:.cpp=.o)

$(NAME) : all

all: $(OBJS)
	c++ $(FLAGS) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all