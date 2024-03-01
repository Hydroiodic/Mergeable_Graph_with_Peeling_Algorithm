compiler = g++
std = c++17
program = main
codes = *.cpp

$(program) : $(codes)
	@$(compiler) -std=$(std) -g -o $(program) $(codes)

test_1:
	./main ./test_files/test_1.txt

test_2:
	./main ./test_files/test_2.txt

test_3:
	./main ./test_files/test_3.txt

clean:
ifeq ($(OS),Windows_NT)
	del .\main.exe
else
	rm main
endif
