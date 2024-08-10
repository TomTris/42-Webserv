NAME = webserv
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

OBJS = $(SRC:.cpp=.o)
CGI_SRC = \
	cgi-bin/process_form_cgi.cpp\
	cgi-bin/getCgiWithCookie.cpp\
	cgi-bin/infinitiv.cpp

CGI_OBJS = $(CGI_SRC:.cpp=.out)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME) $(CGI_OBJS)

%.out: %.cpp
	$(CXX) $(FLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(CGI_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all