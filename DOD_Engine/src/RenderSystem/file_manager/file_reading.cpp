#include "file_readings.h"

std::string read_file(const char* name_file) {
    SetConsoleOutputCP(CP_UTF8);
    std::fstream file(name_file);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return " ";
    }
    std::string line;
    std::string shader;
    while(std::getline(file, line)) {
        shader.append(line + "\n");
    }
    return shader;
}