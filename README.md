# LazyParser
A Lazy Deliminated String Parser. Thread-safe, Light-weight, and Header Only. C++17 Required.

# Basic Use Demo
It can parse any single-character seperated strings on-the-fly. At minimum, `C++17` is required.
```cpp
#include <iostream>
#include "LazyParser.hpp"


void test_single_line(std::string_view line, char delim = ',') {
    LazyParser parser(line, delim);
    std::cout << "Parsing line: \"" << line << "\"\n";
    size_t i = 0;
    try {
        while (true) {
            std::string_view field = parser.get_ith(i);
            std::cout << "  Field[" << i << "]: \"" << field << "\"\n";
            ++i;
        }
    }
    catch (const std::exception& e) {
        std::cout << "  (Reached end after " << i << " fields)\n\n";
    }
    catch (...) {
        std::cout << "  (Reached end after " << i << " fields)\n\n";
    }
}


void test_csv(std::string_view csv) {
    LazyParser line_parser(csv, '\n'); // outer parser: split by newline
    std::cout << "Parsing CSV data:\n" << csv << "\n\n";

    size_t row_idx = 0;
    try {
        while (true) {
            std::string_view row = line_parser.get_ith(row_idx);
            std::cout << "Row " << row_idx << ":\n";
            LazyParser field_parser(row, ','); // inner parser: split by comma

            size_t col_idx = 0;
            try {
                while (true) {
                    std::string_view val = field_parser.get_ith(col_idx);
                    std::cout << "  [" << row_idx << "," << col_idx << "] = \"" << val << "\"\n";
                    ++col_idx;
                }
            }
            catch (...) {
                // done with fields
            }
            ++row_idx;
            std::cout << "\n";
        }
    }
    catch (...) {
        std::cout << "(Done parsing CSV)\n";
    }
}


int main() {
    std::cout << "=== Test 1: Single line parsing ===\n";
    test_single_line("apple,banana,,carrot");

    std::cout << "=== Test 2: Another single line ===\n";
    test_single_line(",,42,hello world,");

    std::cout << "=== Test 3: CSV parsing with nested LazyParser ===\n";
    std::string csv_text =
        "id,name,score\n"
        "1,Alice,90\n"
        "2,Bob,85\n"
        "3,Charlie,88\n"
        "4,,92\n";

    test_csv(csv_text);
    return 0;
}

}
```
