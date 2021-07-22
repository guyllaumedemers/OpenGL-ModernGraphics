#pragma once
#include "Utilities.h"

namespace GLSLFileReader {

	std::string ReadShaderSource(const char* filePath) {
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);			// initialize the filestream
		std::string line = "";

		if (!fileStream) {											// safety catch
			std::puts("filepath invalid");
			return "";
		}

		while (!fileStream.eof()) {									// while I havent reach the end of the file, I loop
			std::getline(fileStream, line);							// fill in the content of the line string
			content.append(line + "\n");							// append my file
		}
		fileStream.close();											// obviously closing the stream at the end
		return content;
	}
}