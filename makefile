target=nodes

run: build
	./$(target)

build:
	gcc $(target).c -o $(target)

debug-build:
	gcc $(target).c -g -o $(target)-debug

memdebug: debug-build
	valgrind --track-origins=yes --leak-check=full ./$(target)-debug

debug: debug-build
	lldb ./$(target)-debug

clean:
	rm $(target) $(target)-debug *.dSYM
