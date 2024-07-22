NAME = Webserve
FLAGS = -Wall -Werror -Wextra
SRC = \
	Files/method_find.cpp\
	Files/socket_create.cpp\
	Files/Webserver.cpp\
	Files/read_file.cpp\
	Files/handle_get_request.cpp\
	Files/handle_post_request.cpp\
	Files/handle_delete_request.cpp\
	Files/handle_unknown_request.cpp

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