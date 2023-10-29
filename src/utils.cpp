#include <JSON/utils.h>
#include <JSON/parser.h>
#include <JSON/printer.h>

namespace JSON {

class CopyParser : public JSON::Parser {

    JSON::Printer printer;

    void onObjectStart() override {
        printer.startObject();
    }

    void onObjectEnd() override {
        printer.endObject();
    }

    void onArrayStart() override {
        printer.startArray();
    }

    void onArrayEnd() override {
        printer.endArray();
    }

    void onKey(std::string& key) override {
        printer.key(key);
    }

    void onNumber(double value) override {
        printer.value(value);
    }

    void onBoolean(bool value) override {
        printer.value(value);
    }

    void onString(std::string& value) override {
        printer.value(value);
    }

    void onNull() override {
        printer.value();
    }

public:

    CopyParser(std::ostream& output, int ident, bool escapeUnicode) :
        printer(output, ident, escapeUnicode) {}
};

void copy(std::ostream& output, std::istream& input, int indent, bool escapeUnicode, const Path& path) {
    CopyParser(output, indent, escapeUnicode).parse(input, path);
}

class ValidateParser : public JSON::Parser {
    void onObjectStart() override {}
    void onObjectEnd() override {}
    void onArrayStart() override {}
    void onArrayEnd() override {}
    void onKey(std::string& key) override {}
    void onNumber(double value) override {}
    void onBoolean(bool value) override {}
    void onString(std::string& value) override {}
    void onNull() override {}
};

bool check(std::istream& input) {
    try {
        ValidateParser().parse(input);
        return true;
    } catch (const JSON::Error&) {
        return false;
    }
}

}