washer: washer.o split.o exec_sem_action.o
	gcc washer.o split.o exec_sem_action.o -o washer

exec_sem_action.o: macro/exec_sem_action.c
	gcc -c macro/exec_sem_action.c

washer.o: washer.c
	gcc -c washer.c

split.o: split/split.c
	gcc -c split/split.c

table: table.o exec_sem_action.o
	gcc table.o exec_sem_action.o -o table

cleaner: cleaner.o exec_sem_action.o
	gcc cleaner.o exec_sem_action.o -o cleaner

table.o: table.c
	gcc -c table.c

cleaner.o: cleaner.c
	gcc -c cleaner.c

clean:
	rm *.o
	rm washer table cleaner
