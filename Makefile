bin/json.o: bin/error.o bin/lexer.o bin/parser.o bin/printer.o bin/value.o bin/utils.o \
            bin/path.o bin/path/lexer.o bin/path/parser.o bin/path/cursor.o bin/path/element.o
	ld -r $^ -o $@

bin/%.o: src/%.cpp include/json/%.h
	@mkdir -p bin/path
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

clean:
	rm -rf bin