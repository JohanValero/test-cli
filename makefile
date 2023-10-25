PROGRAM_NAME = sotest

main:
	gcc src/main.c -o bin/${PROGRAM_NAME}.exe

run_unknow:
	bin/${PROGRAM_NAME}.exe unknow_file.sc

run_tests:
	gcc src/main.c -o bin/${PROGRAM_NAME}.exe
	bin/${PROGRAM_NAME}.exe resources/test.sc