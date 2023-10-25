PROGRAM_NAME = sotest

main:
	gcc src/main.c -o bin/${PROGRAM_NAME}.exe -ldl

run_unknow: main
	bin/${PROGRAM_NAME}.exe unknow_file.sc

run_tests: main
	bin/${PROGRAM_NAME}.exe resources/test.sc
