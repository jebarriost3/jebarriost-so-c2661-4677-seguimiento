.phony: clean

SRCS=jardin_ornamental.c jardin_utilidades.c jardin_ornamental_limitado.c \
jardin_limitado_utilidades.c agente.c fumador.c fumadores_utilidad.c \
ctrl_fumadores.c jardin_ornamental_general.c jardin_ornamental_utilidades_general.c
OBJS=$(SRCS:.c=.o)
EXECS=jardin_ornamental jardin_ornamental_limitado jardin_ornamental_general agente fumador ctrl_fumadores

all: ${EXECS}

jardin_ornamental: jardin_ornamental.o jardin_utilidades.o
	${CC} -o $@ $^

jardin_ornamental.o: jardin_ornamental.c jardin_ornamental.h
	${CC} -c $<

jardin_utilidades.o: jardin_utilidades.c jardin_ornamental.h
	${CC} -c $<

jardin_ornamental_limitado: jardin_ornamental_limitado.o jardin_limitado_utilidades.o
	${CC} -o $@ $^

jardin_ornamental_limitado.o: jardin_ornamental_limitado.c jardin_ornamental_limitado.h
	${CC} -c $<

jardin_limitado_utilidades.o: jardin_limitado_utilidades.c jardin_ornamental_limitado.h
	${CC} -c $<

jardin_ornamental_general: jardin_ornamental_general.o jardin_ornamental_utilidades_general.o
	${CC} -o $@ $^

jardin_ornamental_general.o: jardin_ornamental_general.c jardin_ornamental_general.h
	${CC} -c $<

jardin_ornamental_utilidades_general.o: jardin_ornamental_utilidades_general.c jardin_ornamental_general.h
	${CC} -c $<

agente: agente.o fumadores_utilidad.o
	${CC} -o $@ $^

agente.o: agente.c fumadores.h
	${CC} -c $<

fumador: fumador.o fumadores_utilidad.o
	${CC} -o $@ $^

fumador.o: fumador.c fumadores.h
	${CC} -c $<

ctrl_fumadores: ctrl_fumadores.o fumadores_utilidad.o
	${CC} -o $@ $^

ctrl_fumadores.o: ctrl_fumadores.c fumadores.h
	${CC} -c $<

fumadores_utilidad.o: fumadores_utilidad.c fumadores.h
	${CC} -c $<

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~
