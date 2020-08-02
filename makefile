CMPL=g++ -Wall

THRD_PRT_LIBS=-lncurses -pthread

all:
	$(CMPL) main.cpp 		\
		terminal.cpp 		\
		userInput.cpp		\
		playgroundfield.cpp \
		fieldworkers.cpp    \
		fieldworker.cpp     \
		display.cpp			\
		$(THRD_PRT_LIBS)