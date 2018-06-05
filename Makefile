knapsack-test: main.o common.o exact.o exact-2.o ibarra1975.o ibarra1975-2.o glpk.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o knapsack-test \
	main.o common.o exact.o exact-2.o ibarra1975.o ibarra1975-2.o glpk.o

common.o: common.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o common.o common.c

exact-2.o: exact-2.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o exact-2.o exact-2.c

exact.o: exact.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o exact.o exact.c

ibarra1975-2.o: ibarra1975-2.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o ibarra1975-2.o ibarra1975-2.c

ibarra1975.o: ibarra1975.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o ibarra1975.o ibarra1975.c

main.o: main.c common.h platform.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o main.o main.c

glpk.o: glpk.c common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o glpk.o glpk.c

.PHONY: clean

clean:
	rm knapsack-test main.o common.o exact.o exact-2.o ibarra1975.o ibarra1975-2.o glpk.o
