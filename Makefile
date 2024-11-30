calculator: calculator.l calculator.y calculator.h
	bison -d -Wcounterexamples calculator.y
	flex -o calculator.lex.c calculator.l
	gcc -o $@ -Wno-implicit-function-declaration calculator.tab.c calculator.lex.c calculator.c -lfl -lm

run:
	./calculator test/test.txt

clean:
	rm -f calculator calculator.tab.c calculator.tab.h calculator.lex.c