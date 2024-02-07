#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <string>
#include <ctime>

/**
 * \struct Employee
 * \brief Структура, представляющая сотрудника с полями: имя, должность, отдел, зарплата.
 */
typedef unsigned int uint;
struct Employee {
    std::string name;        ///< Имя сотрудника
    std::string position;    ///< Должность сотрудника
    std::string department;  ///< Отдел, в котором работает сотрудник
    double salary;           ///< Заработная плата сотрудника

    // Перегрузка операторов сравнения
    bool operator>(const Employee& other) const {
        if (department != other.department)
            return department > other.department;
        if (name != other.name)
            return name > other.name;
        return salary > other.salary;
    }

    bool operator<(const Employee& other) const {
        if (department != other.department)
            return department < other.department;
        if (name != other.name)
            return name < other.name;
        return salary < other.salary;
    }

    bool operator>=(const Employee& other) const {
        return !(*this < other);
    }

    bool operator<=(const Employee& other) const {
        return !(*this > other);
    }

    bool operator==(const Employee& other) const {
    return name == other.name && position == other.position && department == other.department && salary == other.salary;
}
};