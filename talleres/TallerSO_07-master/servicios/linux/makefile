SRCS=systemd-service.c servicio1.c cliente1.c servicio2.c
OBJS=$(SRCS:.c=.o)
EXECS=systemd-service servicio1 cliente1 servicio2
INSTALLDIR=/usr/local/bin

all: ${EXECS}

systemd-service: systemd-service.o

servicio1: servicio1.o
	${CC} -o $@ $<

cliente1: cliente1.o
	${CC} -o $@ $<

servicio2: servicio2.o
	${CC} -o $@ $<

install: servicio1 servicio1.service
	install servicio1 -t ${INSTALLDIR}
	cp servicio1.service /etc/systemd/system/servicio1.service

install2: servicio2 servicio2.service
	install servicio2 -t ${INSTALLDIR}
	cp servicio2.service /etc/systemd/system/servicio2.service

clean:
	rm -f ${OBJS}
	rm -f ${EXECS}
	rm -f *~

.phony: clean
