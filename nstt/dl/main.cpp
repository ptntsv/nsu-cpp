#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "src/expressions/env.hpp"
#include "src/expressions/expr.hpp"
#include "src/lexer.hpp"
#include "src/token.hpp"

using std::cout;
using std::endl;

std::string readFromFile(const std::string& fileName) {
    std::string content;
    std::string tmp;
    std::ifstream file{fileName};
    while (std::getline(file, tmp)) {
        content += tmp + '\n';
    }
    return content;
}

int main() {
    Env env;
    std::string content = readFromFile("../tests/inputs/t1.txt");
    Lexer l{content};
    std::vector<Token*> ts = l.tokenize();
    for (size_t i = 0; i < ts.size(); i++) {
        cout << *ts[i] << endl;
    }
    return 0;
}
