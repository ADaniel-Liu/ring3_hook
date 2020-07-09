test: libhopen.so
	@cc -g -o monitor monitor.c 
	@cc -g -o  test test.c
	@cc -g -o  test1 test1.c

libhopen.so: hook_open.c
	@gcc -shared -fPIC -g hook_open.c -o libhopen.so -ldl

clean:
	@rm -f libhopen.so test test1 monitor
install:
	@/usr/bin/bash install.sh
