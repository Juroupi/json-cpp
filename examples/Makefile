EXAMPLES = lex parse print copy value load check load_path find

examples: $(EXAMPLES)

%: %.cpp ../bin/json.o
	$(CXX) $(CXXFLAGS) -I../include $^ -o $@

clean:
	rm -rf $(EXAMPLES)