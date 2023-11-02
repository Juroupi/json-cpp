EXAMPLES = lex parse print copy value load check load_path find

examples: $(EXAMPLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) -I../include -L../bin $^ -o $@ -ljson

clean:
	rm -rf $(EXAMPLES)