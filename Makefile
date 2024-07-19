NAME = Webserve
FLAGS = -Wall -Werror -Wextra
SRC = method_find.cpp socket_create.cpp Webserver.cpp\
	read_file.cpp\
	handle_get_request.cpp\
	handle_post_request.cpp\
	handle_delete_request.cpp\
	handle_unknown_request.cpp

OBJS = $(SRC:.cpp=.o)

$(NAME) : all

all: 
	rm socket_create.o;make all2;
all2: $(OBJS)
	c++ $(FLAGS) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all