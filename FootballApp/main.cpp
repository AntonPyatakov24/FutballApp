#include <iostream>
#include "Menu.hpp"
#include <Windows.h> // для SetConsoleOutputCP

// Точка входа в программу
// Инициализирует систему и запускает главное меню
int main() {
    // Устанавливаем русскую кодировку для корректного отображения текста
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::cout << "=== СИСТЕМА УПРАВЛЕНИЯ ФУТБОЛЬНОЙ ЛИГОЙ ===\n\n";

    // Создаем и запускаем главное меню программы
    Menu menu;
    menu.run();

    return 0;
}