CC		= g++
C		= cpp

CFLAGS	= -g 
LFLAGS	= -g

EXE		= alphamask composite
OBJ		= ImageIO.o

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

all: ${EXE}

alphamask: ${OBJ} alphamask.o
	${CC} ${LFLAGS} -o alphamask ${OBJ} alphamask.o ${LDFLAGS}

alphamask.o: alphamask.${C}
	${CC} ${CFLAGS} -c alphamask.${C}

composite: ${OBJ} composite.o
	${CC} ${LFLAGS} -o composite ${OBJ} composite.o ${LDFLAGS}

composite.o: composite.${C}
	${CC} ${CFLAGS} -c composite.${C}

ImageIO.o: ImageIO.${C} ImageIO.h
	${CC} ${CFLAGS} -c ImageIO.${C}

clean:
	rm -f core.* *.o *~ ${EXE}
