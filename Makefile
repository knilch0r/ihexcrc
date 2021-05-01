.PHONY: all clean test

all: src/main.rs
	cargo build -vv

clean:
	cargo clean

test: ihexcrc.exe
	FAIL=0; for t in tests/*.hex ; do \
	   echo -n 'testing '"$$t"': '; \
	   if ./$(BIN) < $$t | diff -u - tests/`basename $$t .hex`.crc ; then \
	      echo PASSED ; \
	   else \
	      FAIL=1; echo FAILED ; \
	   fi ; \
	done; exit $$FAIL
