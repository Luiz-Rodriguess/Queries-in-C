CC= gcc

CFLAGS=  -Wall -Wextra -Wno-unused-parameter

PKG=`pkg-config --cflags --libs glib-2.0`

SRC= src

OBJ= obj

INC= include

RES = Resultados

DATA= dataset

LARGE= data_large

TRES= testResults

program: programa-principal programa-testes

programa-principal: flights.o users.o parser.o passengers.o reservations.o util.o table.o query1.o query2.o query3.o query4.o query5.o query6.o query7.o query8.o query9.o query10.o handlequery.o main.o print.o interactive.o flightAccess.o userAccess.o reservationAccess.o statistics.o
	$(CC) $(CFLAGS) $(OBJ)/flights.o $(OBJ)/users.o $(OBJ)/parser.o $(OBJ)/passengers.o $(OBJ)/reservations.o $(OBJ)/util.o $(OBJ)/table.o $(OBJ)/query1.o $(OBJ)/query2.o $(OBJ)/query3.o $(OBJ)/query4.o $(OBJ)/query5.o $(OBJ)/query6.o $(OBJ)/query7.o $(OBJ)/query8.o $(OBJ)/query9.o $(OBJ)/query10.o $(OBJ)/handlequery.o $(OBJ)/main.o $(OBJ)/print.o $(OBJ)/interactive.o $(OBJ)/flightAccess.o $(OBJ)/userAccess.o $(OBJ)/reservationAccess.o $(OBJ)/statistics.o $(PKG) -o programa-principal

programa-testes: flights.o users.o parser.o passengers.o reservations.o util.o table.o query1.o query2.o query3.o query4.o query5.o query6.o query7.o query8.o query9.o query10.o handlequery.o print.o flightAccess.o userAccess.o reservationAccess.o statistics.o testes.o
	$(CC) $(CFLAGS) $(OBJ)/flights.o $(OBJ)/users.o $(OBJ)/parser.o $(OBJ)/passengers.o $(OBJ)/reservations.o $(OBJ)/util.o $(OBJ)/table.o $(OBJ)/query1.o $(OBJ)/query2.o $(OBJ)/query3.o $(OBJ)/query4.o $(OBJ)/query5.o $(OBJ)/query6.o $(OBJ)/query7.o $(OBJ)/query8.o $(OBJ)/query9.o $(OBJ)/query10.o $(OBJ)/handlequery.o $(OBJ)/print.o $(OBJ)/flightAccess.o $(OBJ)/userAccess.o $(OBJ)/reservationAccess.o $(OBJ)/statistics.o $(OBJ)/testes.o $(PKG) -o programa-testes

flights.o: $(SRC)/flights.c $(INC)/flights.h 
	$(CC) $(CFLAGS) -o $(OBJ)/flights.o $(PKG) -c -g $(SRC)/flights.c

users.o: $(SRC)/users.c $(INC)/users.h
	$(CC) $(CFLAGS) -o $(OBJ)/users.o $(PKG) -c -g $(SRC)/users.c

parser.o: $(SRC)/parser.c $(INC)/parser.h 
	$(CC) $(CFLAGS) -o $(OBJ)/parser.o $(PKG) -c -g $(SRC)/parser.c

passengers.o: $(SRC)/passengers.c $(INC)/passengers.h
	$(CC) $(CFLAGS) -o $(OBJ)/passengers.o $(PKG) -c -g $(SRC)/passengers.c

reservations.o: $(SRC)/reservations.c $(INC)/reservations.h
	$(CC) $(CFLAGS) -o $(OBJ)/reservations.o $(PKG) -c -g $(SRC)/reservations.c

util.o: $(SRC)/util.c $(INC)/util.h
	$(CC) $(CFLAGS) -o $(OBJ)/util.o $(PKG) -c -g $(SRC)/util.c

table.o: $(SRC)/table.c $(INC)/table.h
	$(CC) $(CFLAGS) -o $(OBJ)/table.o $(PKG) -c -g $(SRC)/table.c

query1.o: $(SRC)/query1.c $(INC)/query1.h
	$(CC) $(CFLAGS) -o $(OBJ)/query1.o $(PKG) -c -g $(SRC)/query1.c


query2.o: $(SRC)/query2.c $(INC)/query2.h
	$(CC) $(CFLAGS) -o $(OBJ)/query2.o $(PKG) -c -g $(SRC)/query2.c


query3.o: $(SRC)/query3.c $(INC)/query3.h
	$(CC) $(CFLAGS) -o $(OBJ)/query3.o $(PKG) -c -g $(SRC)/query3.c


query4.o: $(SRC)/query4.c $(INC)/query4.h
	$(CC) $(CFLAGS) -o $(OBJ)/query4.o $(PKG) -c -g $(SRC)/query4.c


query5.o: $(SRC)/query5.c $(INC)/query5.h
	$(CC) $(CFLAGS) -o $(OBJ)/query5.o $(PKG) -c -g $(SRC)/query5.c


query6.o: $(SRC)/query6.c $(INC)/query6.h
	$(CC) $(CFLAGS) -o $(OBJ)/query6.o $(PKG) -c -g $(SRC)/query6.c


query7.o: $(SRC)/query7.c $(INC)/query7.h
	$(CC) $(CFLAGS) -o $(OBJ)/query7.o $(PKG) -c -g $(SRC)/query7.c


query8.o: $(SRC)/query8.c $(INC)/query8.h
	$(CC) $(CFLAGS) -o $(OBJ)/query8.o $(PKG) -c -g $(SRC)/query8.c


query9.o: $(SRC)/query9.c $(INC)/query9.h
	$(CC) $(CFLAGS) -o $(OBJ)/query9.o $(PKG) -c -g $(SRC)/query9.c


query10.o: $(SRC)/query10.c $(INC)/query10.h
	$(CC) $(CFLAGS) -o $(OBJ)/query10.o $(PKG) -c -g $(SRC)/query10.c

handlequery.o: $(SRC)/handlequery.c $(INC)/handlequery.h
	$(CC) $(CFLAGS) -o $(OBJ)/handlequery.o $(PKG) -c -g $(SRC)/handlequery.c

interactive.o: $(SRC)/interactive.c $(INC)/interactive.h
	$(CC) $(CFLAGS) -o $(OBJ)/interactive.o $(PKG) -c -g $(SRC)/interactive.c

flightAccess.o: $(SRC)/flightAccess.c $(INC)/flightAccess.h
	$(CC) $(CFLAGS) -o $(OBJ)/flightAccess.o $(PKG) -c -g $(SRC)/flightAccess.c

userAccess.o: $(SRC)/userAccess.c $(INC)/userAccess.h
	$(CC) $(CFLAGS) -o $(OBJ)/userAccess.o $(PKG) -c -g $(SRC)/userAccess.c

reservationAccess.o: $(SRC)/reservationAccess.c $(INC)/reservationAccess.h
	$(CC) $(CFLAGS) -o $(OBJ)/reservationAccess.o $(PKG) -c -g $(SRC)/reservationAccess.c

statistics.o: $(SRC)/statistics.c $(INC)/statistics.h
	$(CC) $(CFLAGS) -o $(OBJ)/statistics.o $(PKG) -c -g $(SRC)/statistics.c

main.o: $(SRC)/main.c 
	$(CC) $(CFLAGS) -o $(OBJ)/main.o $(PKG) -c -g $(SRC)/main.c

testes.o: $(SRC)/testes.c 
	$(CC) $(CFLAGS) -o $(OBJ)/testes.o $(PKG) -c -g $(SRC)/testes.c

print.o: $(SRC)/print.c 
	$(CC) $(CFLAGS) -o $(OBJ)/print.o $(PKG) -c -g $(SRC)/print.c

exec: programa-principal
	./programa-principal $(DATA)/data $(DATA)/input.txt

exec-large: programa-principal
	./programa-principal $(LARGE)/data $(LARGE)/input.txt

run-tests: programa-testes
	./programa-testes $(DATA)/data $(DATA)/input.txt $(DATA)/outputs

run-tests-large: programa-testes
	./programa-testes $(LARGE)/data $(LARGE)/input.txt $(LARGE)/outputs

inter: programa-principal
	./programa-principal
	
clean:
	rm -f program $(OBJ)/*.o
	rm -f program $(RES)/*.csv
	rm -f program $(RES)/*.txt
	rm -f program $(TRES)/*.txt
