CC=gcc
CFLAGS=-O2 -Wall -W
BIN=ihexcrc.exe

$(BIN): ihexcrc.c crc32.c crc32.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(BIN)

test: ihexcrc.exe
	FAIL=0; set -o pipefail ; for t in tests/*.hex ; do \
	   echo -n 'testing '"$$t"': '; \
	   if ./$(BIN) < $$t | diff -u - tests/`basename $$t .hex`.crc ; then \
	      echo PASSED ; \
	   else \
	      FAIL=1; echo FAILED ; \
	   fi ; \
	done; exit $$FAIL
