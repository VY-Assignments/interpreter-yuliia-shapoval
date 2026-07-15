#include <iostream>
#include <string>
#include <vector>
#include "Lexer.h"
#include "Parser.h"
#include "Evaluator.h"

int main() {
    Environment env;
    std::string input;
    std::cout << "Type your expressions or commands (type 'exit' to stop):" << std::endl;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            break;
        }

        if (input == "exit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenize();

            Parser parser(tokens);
            std::unique_ptr<ASTNode> ast = parser.parse();

            Evaluator evaluator(env);
            double result = evaluator.evaluate(ast.get());

            if (input.rfind("def ", 0) != 0 && input.rfind("var ", 0) != 0) {
                std::cout << result << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}