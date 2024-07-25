NAME = Webserve
FLAGS = -Wall -Werror -Wextra
SRC = \
	Files/Parser/parse.cpp\
	Files/Parser/Class_Server.cpp\
	\
	Files/body_handle_post.cpp\
	Files/connection_relevant.cpp\
	Files/get_from_paring.cpp\
	Files/handle_path.cpp\
	Files/header_handle.cpp\
	Files/reader.cpp\
	Files/socket_create.cpp\
	Files/statusCode.cpp\
	Files/Webserver.cpp\
	Files/writer.cpp\
	Files/Class_Connection.cpp\
	Files/deleter.cpp

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