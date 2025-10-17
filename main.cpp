#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"

int main() {
	const std::string filename = "main.jks";
	std::string fileContent;
	std::vector<std::string> lines;

	std::ifstream file(filename);
	std::ostringstream buffer;
	buffer << file.rdbuf();
	fileContent = buffer.str();	
	file.close();

	std::stringstream ss(fileContent);
	std::string line;
	while (std::getline(ss, line)) {
		lines.push_back(line);
	}

	Lexer lexer(fileContent);
	std::vector<Token> tokens = lexer.tokenize();

	if (!lexer.lexerErrors.empty()) {
		for (auto& error : lexer.lexerErrors) {
			std::cout << filename << ':' << error.line << ':' << error.col << ": ";
			std::cout << "\033[1;31mSyntaxError:\033[0m " << error.message << '\n';
			std::cout << error.line << "|" << lines[error.line - 1] << "\n";
			std::cout << std::string(std::ceil(std::log10(error.line + 0.1)) + error.col, ' ') << "^\n";
		}
		std::cout << lexer.lexerErrors.size() << " errors generated.\n";
		return -1;
	}
	for (Token tok : tokens) {
		std::cout << (int)tok.type << '(' << tok.value << "):(" << tok.line << ':' << tok.col << ")\n";
	}
	Parser parser(tokens);
}
