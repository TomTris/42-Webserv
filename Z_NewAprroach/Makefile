NAME = b.out
FLAGS = -Wall -Werror -Wextra
SRC = \
	Ob/Class_Server.cpp\
	Ob/get_answer.cpp\
	Ob/parse.cpp\
	\
	main.cpp\
	\
	others/socket_create.cpp\
	others/Connection.cpp\
	others/poll_relevant.cpp\
	others/statusCode.cpp\
	\
	level/server_level.cpp\
	\
	level/connection_level.cpp\
	level/connection_level_extract.cpp\
	\
	level/reader_level_cgi_after_waiting_done_2.cpp\
	level/reader_level_cgi_get_before_waiting_done.cpp\
	level/reader_level_cgi_post_1_write.cpp\
	level/reader_level_cgi_post_2_before_waiting_done.cpp\
	level/read_level.cpp\
	\
	level/write_level.cpp

# SANITIZER_FLAGS = -fsanitize=address,undefined

OBJS = $(SRC:.cpp=.o)

$(NAME) : all

all: $(OBJS)
# c++ $(FLAGS) $(OBJS) -o b.out && valgrind ./b.out
	c++ $(FLAGS) $(OBJS) -o b.out && ./b.out

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all