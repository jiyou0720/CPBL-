CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -I src
LIBS = -lncursesw -lSDL2 -lSDL2_mixer
TARGET  = manor
SRCDIR  = src
SRCS    = $(SRCDIR)/main.c    \
          $(SRCDIR)/state.c   \
          $(SRCDIR)/map.c     \
          $(SRCDIR)/render.c  \
          $(SRCDIR)/player.c  \
          $(SRCDIR)/save.c    \
          $(SRCDIR)/notes.c   \
          $(SRCDIR)/puzzle.c  \
          $(SRCDIR)/enemy.c   \
          $(SRCDIR)/intro.c \
          $(SRCDIR)/ending.c \
          $(SRCDIR)/bgm.c \
          $(SRCDIR)/ranking.c  
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)
	@echo "==> 빌드 완료: ./$(TARGET)"

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "==> 정리 완료"
