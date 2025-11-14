#ifndef DATAENTITY_HPP
#define DATAENTITY_HPP

#include <string>

// Базовый абстрактный класс для всех сущностей системы
// Содержит общие поля и методы для наследования
class DataEntity {
protected:
    int id;                    // Уникальный идентификатор сущности
    std::string name;          // Название сущности

public:
    // Конструктор принимает ID и название для инициализации
    DataEntity(int entityId, const std::string& entityName);

    // Виртуальный деструктор для корректного удаления наследников
    virtual ~DataEntity() = default;

    // Чисто виртуальный метод - делает класс абстрактным
    // Должен быть реализован в классах-наследниках
    virtual void displayInfo() const = 0;

    // Геттеры для доступа к приватным полям
    int getId() const;                    // Возвращает ID
    std::string getName() const;          // Возвращает название

    // Сеттер для изменения названия сущности
    void setName(const std::string& newName);
};

#endif