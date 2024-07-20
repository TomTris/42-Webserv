CFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =  Parser/parse.cpp Server/Server.cpp handle_delete_request.cpp handle_get_request.cpp handle_path.cpp handle_post_request.cpp \
handle_unknown_request.cpp method_find.cpp read_file.cpp Webserver.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re