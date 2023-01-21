TESTCOVERAGE = -fprofile-arcs -ftest-coverage
STD = -Wall -Werror -Wextra --std=c++17

all: clean test matrix_oop.a gcov_report


matrix_oop.a:
	g++ $(STD) -c matrix_oop.cpp -o matrix_oop.o
	ar rcs matrix_oop.a matrix_oop.o


gcov_report: test
	./test
	mkdir report
	gcovr -r ../ -e ../googletest-1.10.x --html --html-details -o report/result.html
	open report/result.html


test: matrix_oop.o
	g++ test.cpp $^ -o test -lgtest -lgmock -pthread --coverage 
	./test


matrix_oop.o: matrix_oop.cpp
	g++ $(STD) $(TESTCOVERAGE) -c $^ -o $@


test.o: test.cpp
	g++ $(STD) $(TESTCOVERAGE) -c $^ -o $@


check_leaks:
	CK_FORK=no valgrind --leak-check=full --track-origins=yes -s ./test


check_clang:
	cp ../materials/linters/.clang-format .clang-format
	# clang-format -i test.cpp matrix_oop.cpp matrix_oop.h
	clang-format -n *.cpp *.h


check_cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem main.cpp matrix_oop.cpp matrix_oop.hpp test.cpp


rebuild: clean main


clean:
	rm -rf *.o main.out *.gch *.gcda *.gcno test report .clang-format *.a