#include <iostream>
#include <json/printer.h>

int main() {
    JSON::Printer printer(std::cout, 4, false, true);
    printer.startObject();
        printer.key("uni");
        printer.value("\u0001\u20AC\u00A3\u0418");
        printer.key("value");
        printer.value(-102.08900e-0000190);
        printer.key("salut");
        printer.value("\"non\"");
        printer.key("null");
        printer.value();
        printer.key("true");
        printer.startArray();
            printer.value(true);
            printer.value(false);
        printer.endArray();
    printer.endObject();
    std::cout << std::endl;
    return 0;
}