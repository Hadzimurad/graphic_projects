#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void updateGameLogic() {
    const int n = 10;
    std::string n1 =  "░░░░░░░░░░";
    for(int i = 0; i < n; ++i) {
        // Вычисления физики, ИИ и логики
        std::string n2 = "█";
        for(int j = 0; j < n; ++j) std::cout << n1[j];
        n1[i] = n2;
        std::this_thread::sleep_for(std::chrono::milliseconds(40)); // ~60 FPS
    }
}

int main() {
    // Запуск потока логики
    std::thread logicThread(updateGameLogic);

    std::cout << "Waiting load..\n";
    // Обязательно ждем завершения потока перед выходом из программы
    if (logicThread.joinable()) {
        logicThread.join();
    }
    std::cout << "\nWork programm over\n";
    return 0;
}
