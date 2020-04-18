all: detach
clean:
	rm -f detach
install: detach
	chmod 544 ./detach
	install ./detach /usr/bin
uninstall:
	rm -f /usr/bin/detach
detach:
	gcc main.c -o detach
