HEADERS = Vars.h Funcs.h Semaphore.h

default: build

cos331-ransom-noah-proj03.out: cos331-ransom-noah-proj03.c Funcs.c Semaphore.c $(HEADERS)
	gcc cos331-ransom-noah-proj03.c Funcs.c Semaphore.c -o cos331-ransom-noah-proj03.out

clean:
	rm -f cos331-ransom-noah-proj03.out

build: cos331-ransom-noah-proj03.out
