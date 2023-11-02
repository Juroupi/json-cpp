bin/libjson.a: bin/error.o bin/lexer.o bin/parser.o bin/printer.o bin/value.o bin/utils.o \
            bin/path.o bin/path/lexer.o bin/path/parser.o bin/path/cursor.o bin/path/element.o
	ar -rsc $@ $^

bin/%.o: src/%.cpp include/json/%.h
	@mkdir -p bin/path
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

install:
	@install -d /usr/local/include/json
	@install -m 644 include/json/*.h /usr/local/include/json
	@install -d /usr/local/include/json/path
	@install -m 644 include/json/path/*.h /usr/local/include/json/path
	@install -m 644 bin/libjson.a /usr/local/lib

uninstall:
	@rm -rf /usr/local/include/json
	@rm -f /usr/local/lib/libjson.a

clean:
	rm -rf bin