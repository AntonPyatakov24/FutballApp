#include "DataEntity.hpp"

// Конструктор использует список инициализации для эффективной загрузки полей
DataEntity::DataEntity(int entityId, const std::string& entityName)
    : id(entityId), name(entityName) {
}

// Геттер для ID - возвращает значение поля
int DataEntity::getId() const {
    return id;
}

// Геттер для названия - возвращает копию строки
std::string DataEntity::getName() const {
    return name;
}

// Сеттер для названия - заменяет текущее название новым
void DataEntity::setName(const std::string& newName) {
    name = newName;
}