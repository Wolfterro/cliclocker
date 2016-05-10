SRC = cliclocker.c
CC = cc
BIN = cliclocker
INSTALLPATH = /usr/local/bin/
CFLAGS = -Wall
LDFLAGS = -lcurses


cliclocker : ${SRC}

	@echo "build ${SRC}"
	@echo "CC ${SRC} ${CFLAGS} ${LDFLAGS}"
	@${CC} ${SRC} -o ${BIN} ${LDFLAGS} ${CFLAGS}

install : ${BIN}

	@echo "installing binary file to ${INSTALLPATH}${BIN}"
	@cp ${BIN} ${INSTALLPATH}
	@chmod 755 ${INSTALLPATH}${BIN}
	@echo "installed"

uninstall :

	@echo "uninstalling binary file (${INSTALLPATH}${BIN})"
	@rm -f ${INSTALLPATH}${BIN}
	@echo "${BIN} uninstalled"
clean :

	@echo "cleaning ${BIN}"
	@rm ${BIN}
	@echo "${BIN} cleaned"

