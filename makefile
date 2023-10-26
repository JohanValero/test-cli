PROGRAM_NAME = sotest.bin

main:
	gcc src/main.c -o bin/${PROGRAM_NAME} -ldl

generate_so:
	gcc -fPIC -shared src/lib1.c -o bin/test.so

run_file_tests: main generate_so
	bin/${PROGRAM_NAME} resources/test.sc

run_terminal: main generate_so
	bin/${PROGRAM_NAME}