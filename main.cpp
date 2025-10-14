#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lexer.hpp"

int main() {
	std::ifstream file("main.jks");
	std::ostringstream buffer;
	buffer << file.rdbuf(); // Read entire file into stringstream
	
	std::string fileContent = buffer.str(); // Convert to std::string
	
	file.close();
	Lexer lexer("main.jks", fileContent);
	std::vector<Token> tokens = lexer.tokenize();
	lexer.logErrors();
	for (Token tok : tokens) {
		std::cout << (int)tok.type << '(' << tok.value << "):(" << tok.line << ':' << tok.col << ")\n";
	}
}
