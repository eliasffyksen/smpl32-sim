compile: $(wildcard src/*.cpp)
	mkdir -p build
	clang++ -o build/simp32 -Wall $^

run: compile
	./build/simp32