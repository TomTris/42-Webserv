# NAME = a.out
# FLAGS = -Wall -Werror -Wextra
# SRC = \
# 	Ob/Class_Server.cpp\
# 	Ob/get_answer.cpp\
# 	Ob/parse.cpp\
# 	main.cpp\
# 	socket_create.cpp\
# 	Connection.cpp\
# 	Reader.cpp\
# 	Writer.cpp\
# 	poll_relevant.cpp\
# 	level/server_level.cpp\
# 	level/connection_level.cpp\
# 	level/read_level.cpp\
# 	level/write_level.cpp\
# 	statusCode.cpp


# OBJS = $(SRC:.cpp=.o)

# $(NAME) : all

# all: $(OBJS)
# 	c++ $(FLAGS) $(OBJS)

# clean:
# 	rm -f $(OBJS)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

NAME = b.out
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
	level/server_level.cpp\
	level/connection_level.cpp\
	level/read_level.cpp\
	level/write_level.cpp\
	statusCode.cpp
SANITIZER_FLAGS = -fsanitize=address,undefined

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