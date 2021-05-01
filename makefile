CMPL=g++ -Wall

THRD_PRT_LIBS=-lncurses -pthread
DEBUG_INFO=-g

debug:
	$(CMPL) main.cpp 		\
		terminal.cpp 		\
		userInput.cpp		\
		playgroundfield.cpp \
		fieldworkers.cpp    \
		fieldworker.cpp     \
		display.cpp			\
		$(THRD_PRT_LIBS)	\
		$(DEBUG_INFO)

all:
	$(CMPL) main.cpp 		\
		terminal.cpp 		\
		userInput.cpp		\
		playgroundfield.cpp \
		fieldworkers.cpp    \
		fieldworker.cpp     \
		display.cpp			\
		$(THRD_PRT_LIBS)