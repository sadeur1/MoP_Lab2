#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <chrono>
#include <algorithm>
#include <unordered_map>
#include <iomanip>


// Структура данных сотрудника
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

// Генерация случайной строки
std::string randomString(unsigned int l = 15, std::string charIndex = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890")
{ 
    unsigned int length = rand() % l + 1; 

    unsigned int ri[15]; 

    for (unsigned int i = 0; i < length; ++i) 
        ri[i] = rand() % charIndex.length();

    std::string rs = ""; 

    for (unsigned int i = 0; i < length; ++i) 
        rs += charIndex[ri[i]];

    if (rs.empty()) randomString(l, charIndex); 

    else return rs;
}

// Генерация случайных данных и запись в файл
void generateRandomData(const std::string& filename, int size) {
    std::ofstream file(filename);

    if (file.is_open()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> salaryDistribution(1000.0, 10000.0);

        for (int i = 0; i < size; ++i) {
            Employee employee;
            employee.name = randomString();
            employee.position = randomString();
            employee.department = randomString();
            employee.salary = salaryDistribution(gen);

            file << employee.name << " " << employee.position << " " << employee.department << " " << employee.salary << "\n";
        }

        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// Считывание данных из файла
std::vector<Employee> readDataFromFile(const std::string& filename) {
    std::vector<Employee> data;
    std::ifstream file(filename);

    if (file.is_open()) {
        while (!file.eof()) {
            Employee employee;
            file >> employee.name >> employee.position >> employee.department >> employee.salary;
            data.push_back(employee);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return data;
}

// Вывод результатов в файл CSV
void writeResultsToCSV(const std::vector<int>& sizes, const std::vector<double>& binarySearchTreeTimes, const std::vector<double>& redBlackTreeTimes, const std::vector<double>& hashTableTimes, const std::vector<double>& multimapTimes, const std::vector<size_t>& hashTableCollisions, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << std::fixed << std::setprecision(10);
        file << "Array Size,Binary Search Tree Time,Red-Black Tree Time,Hash Table Time,Hash Table Collisions,Multimap Time\n";
        for (size_t i = 0; i < sizes.size(); ++i) {
            file << sizes[i] << "," << binarySearchTreeTimes[i] << "," << redBlackTreeTimes[i] << "," << hashTableTimes[i] << "," << hashTableCollisions[i] << "," << multimapTimes[i] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// Бинарное дерево поиска
class BinarySearchTree {
private:
    struct Node {
        Employee data;
        Node* left;
        Node* right;
        Node(const Employee& item) : data(item), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Вставка элемента в дерево
    void insert(Node*& node, const Employee& item) {
        if (node == nullptr)
            node = new Node(item);
        else if (item < node->data)
            insert(node->left, item);
        else
            insert(node->right, item);
    }

    // Поиск элемента в дереве
    bool search(Node* node, const Employee& item) {
        if (node == nullptr)
            return false;
        else if (node->data == item)
            return true;
        else if (item < node->data)
            return search(node->left, item);
        else
            return search(node->right, item);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(const Employee& item) {
        insert(root, item);
    }

    bool search(const Employee& item) {
        return search(root, item);
    }
};

// Класс узла красно-черного дерева
template<typename T>
class RBTreeNode {
public:
    T data;
    RBTreeNode<T>* parent;
    RBTreeNode<T>* left;
    RBTreeNode<T>* right;
    bool isRed;

    // Конструктор узла
    RBTreeNode(const T& value) : data(value), parent(nullptr), left(nullptr), right(nullptr), isRed(true) {}
};

// Красно-черное дерево
template<typename T>
class RedBlackTree {
private:
    RBTreeNode<T>* root;

    // Вспомогательные функции для вставки
    void rotateLeft(RBTreeNode<T>* node);
    void rotateRight(RBTreeNode<T>* node);
    void insertFixup(RBTreeNode<T>* node);

public:
    RedBlackTree() : root(nullptr) {}

    // Вставка элемента
    void insert(const T& value);

    // Поиск элемента
    bool search(const T& value) const;
};

// Вращение влево
template<typename T>
void RedBlackTree<T>::rotateLeft(RBTreeNode<T>* node) {
    RBTreeNode<T>* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
}

// Вращение вправо
template<typename T>
void RedBlackTree<T>::rotateRight(RBTreeNode<T>* node) {
    RBTreeNode<T>* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = leftChild;
    } else if (node == node->parent->right) {
        node->parent->right = leftChild;
    } else {
        node->parent->left = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
}

// Вставка узла и исправление нарушений свойств красно-черного дерева
template<typename T>
void RedBlackTree<T>::insert(const T& value) {
    RBTreeNode<T>* newNode = new RBTreeNode<T>(value);
    RBTreeNode<T>* parent = nullptr;
    RBTreeNode<T>* current = root;

    // Находим место для вставки
    while (current != nullptr) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Устанавливаем родителя новому узлу
    newNode->parent = parent;

    if (parent == nullptr) {
        root = newNode;
    } else if (value < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // Исправляем возможные нарушения свойств красно-черного дерева
    insertFixup(newNode);
}

// Исправление нарушений свойств красно-черного дерева после вставки
template<typename T>
void RedBlackTree<T>::insertFixup(RBTreeNode<T>* node) {
    while (node != root && node->parent->isRed) {
        if (node->parent == node->parent->parent->left) {
            RBTreeNode<T>* uncle = node->parent->parent->right;
            if (uncle != nullptr && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                rotateRight(node->parent->parent);
            }
        } else {
            RBTreeNode<T>* uncle = node->parent->parent->left;
            if (uncle != nullptr && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->isRed = false;
}

// Поиск элемента в дереве
template<typename T>
bool RedBlackTree<T>::search(const T& value) const {
    RBTreeNode<T>* current = root;
    while (current != nullptr) {
        if (value == current->data) {
            return true;
        } else if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

// Хэш-таблица
class HashTable {
private:
    std::vector<std::vector<Employee>> table;
    std::hash<std::string> hashFunction;

    // Хэш функция
    size_t hash(const std::string& key) {
        // Пример простой хэш-функции: сумма кодов символов строки
        size_t hashValue = 0;
        for (char c : key) {
            hashValue += c;
        }
        return hashValue % table.size();
    }

public:
    HashTable(size_t size) : table(size) {}

    // Вставка элемента в таблицу
    void insert(const Employee& item) {
        size_t index = hash(item.department);
        table[index].push_back(item);
    }

    // Поиск элемента в таблице
    bool search(const Employee& item) {
        size_t index = hash(item.department);
        for (const auto& e : table[index]) {
            if (e == item)
                return true;
        }
        return false;
    }

    // Подсчет коллизий
    size_t countCollisions() {
        size_t collisions = 0;
        for (const auto& bucket : table) {
            if (bucket.size() > 1) {
                collisions += bucket.size() - 1;
            }
        }
        return collisions;
    }
};

// Генерация случайного сотрудника для поиска
Employee generateRandomEmployee(const std::vector<Employee>& data) {
    int index = rand() % data.size();
    return data[index];
}

// Функция для измерения времени выполнения операции поиска
template<typename Func>
double measureExecutionTime(Func function, const std::vector<Employee>& data, const Employee& target) {
    auto start = std::chrono::high_resolution_clock::now();
    function(data, target);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}


// Поиск в массиве с использованием бинарного дерева поиска
void binarySearchTreeSearch(const std::vector<Employee>& data, const Employee& target) {
    BinarySearchTree tree;
    for (const auto& employee : data) {
        tree.insert(employee);
    }
    tree.search(target);
}

// Поиск в красно-черном дереве
void redBlackTreeSearch(const std::vector<Employee>& data, const Employee& target) {
    RedBlackTree<Employee> tree;
    for (const auto& employee : data) {
        tree.insert(employee);
    }
    tree.search(target);
}

// Поиск в массиве с использованием хэш-таблицы
void hashTableSearch(const std::vector<Employee>& data, const Employee& target) {
    HashTable table(data.size()); // Размер таблицы равен размеру данных
    for (const auto& employee : data) {
        table.insert(employee);
    }
    table.search(target);
}

// Поиск в multimap
void multimapSearch(const std::vector<Employee>& data, const Employee& target) {
    std::multimap<std::string, Employee> mmap;
    for (const auto& employee : data) {
        mmap.insert({employee.department, employee});
    }
    mmap.find(target.department);
}

// Выполнение поиска и измерение времени выполнения
void executeSearchAndMeasure(const std::vector<Employee>& data, int iterations) {
    std::vector<double> binarySearchTreeTimes;
    std::vector<double> redBlackTreeTimes; // Время поиска с использованием красно-черного дерева
    std::vector<double> hashTableTimes;
    std::vector<double> multimapTimes;
    std::vector<size_t> hashTableCollisions;
    std::vector<int> sizes;

    for (int size = 100; size <= 100000; size += 11100) {
        sizes.push_back(size);
        double binarySearchTreeTotalTime = 0.0;
        double redBlackTreeTotalTime = 0.0; // Общее время поиска в красно-черном дереве
        double hashTableTotalTime = 0.0;
        double multimapTotalTime = 0.0;
        size_t hashTableTotalCollisions = 0;

        for (int i = 0; i < iterations; ++i) {
            std::vector<Employee> subset(data.begin(), data.begin() + size);
            Employee target = generateRandomEmployee(subset);

            // Бинарное дерево поиска
            binarySearchTreeTotalTime += measureExecutionTime(binarySearchTreeSearch, subset, target);

            // Красно-черное дерево
            redBlackTreeTotalTime += measureExecutionTime(redBlackTreeSearch, subset, target);

            // Хэш-таблица
            HashTable table(size);
            for (const auto& employee : subset) {
                table.insert(employee);
            }
            hashTableTotalTime += measureExecutionTime(hashTableSearch, subset, target);
            hashTableTotalCollisions += table.countCollisions();

            // Multimap
            multimapTotalTime += measureExecutionTime(multimapSearch, subset, target);
        }

        binarySearchTreeTimes.push_back(binarySearchTreeTotalTime / iterations);
        redBlackTreeTimes.push_back(redBlackTreeTotalTime / iterations);
        hashTableTimes.push_back(hashTableTotalTime / iterations);
        multimapTimes.push_back(multimapTotalTime / iterations);
        hashTableCollisions.push_back(hashTableTotalCollisions / iterations);
    }

    // Запись результатов в файл CSV
    writeResultsToCSV(sizes, binarySearchTreeTimes, redBlackTreeTimes, hashTableTimes, multimapTimes, hashTableCollisions, "search_results.csv");
}
int main() {
    srand(time(0));

    const std::string filename = "employees.txt";
    const int dataSize = 100000;
    const int iterations = 10;

    // Генерация данных
    generateRandomData(filename, dataSize);

    // Считывание данных
    std::vector<Employee> data = readDataFromFile(filename);

    // Выполнение поиска и измерение времени
    executeSearchAndMeasure(data, iterations);

    return 0;
}