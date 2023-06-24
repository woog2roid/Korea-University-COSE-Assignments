SRCS = evaluator.c gantt.c main.c scheduler.c process.c
OBJS = $(SRCS:.c=.o)
TARGET = project

all: $(TARGET)

$(TARGET): $(OBJS)
	cc $(OBJS) -o $(TARGET)
	rm -rf $(OBJS)

%.o: %.c main.h
	cc -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)

re: fclean all