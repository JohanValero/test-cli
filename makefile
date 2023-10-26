PROGRAM_NAME = sotest.bin

main:
	gcc src/main.c -o bin/${PROGRAM_NAME} -ldl

run_unknow: main
	bin/${PROGRAM_NAME} unknow_file.sc

run_tests: main
	bin/${PROGRAM_NAME} resources/test.sc

generate_so:
	gcc -fPIC -shared src/lib1.c -o bin/test.so