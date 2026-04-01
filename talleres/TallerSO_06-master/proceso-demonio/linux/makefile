.phony: clean

SRCS=proceso-demonio.c servidor3.c
OBJS=$(SRCS:.c=.o)
EXECS=proceso-demonio servidor3

all: ${EXECS}

proceso-demonio: proceso-demonio.o
	${CC} -o $@ $<

servidor3: servidor3.o ../../tuberias-nombradas/linux/tuberias_nombradas.o
	${CC} -o $@ $^

servidor3.o: servidor3.c ../../tuberias-nombradas/linux/tuberias_nombradas.h ../../tuberias-nombradas/linux/servidor.h

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
