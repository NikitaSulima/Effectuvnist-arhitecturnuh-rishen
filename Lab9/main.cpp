#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

// === Інтерфейс "Відвідувач" ===
class Department;
class Employee;
class Company;

class Visitor {
public:
    virtual void visit(Company* company) = 0;
    virtual void visit(Department* department) = 0;
    virtual void visit(Employee* employee) = 0;
    virtual ~Visitor() = default;
};

// === Інтерфейс елементів, які приймають відвідувача ===
class Element {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Element() = default;
};

// === Клас "Співробітник" ===
class Employee : public Element {
    string position;
    double salary;

public:
    Employee(string pos, double sal) : position(pos), salary(sal) {}

    string getPosition() const { return position; }
    double getSalary() const { return salary; }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

// === Клас "Департамент" ===
class Department : public Element {
    string name;
    vector<Employee*> employees;

public:
    Department(string n, vector<Employee*> emps) : name(n), employees(emps) {}

    string getName() const { return name; }
    vector<Employee*> getEmployees() const { return employees; }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

// === Клас "Компанія" ===
class Company : public Element {
    string name;
    vector<Department*> departments;

public:
    Company(string n, vector<Department*> deps) : name(n), departments(deps) {}

    string getName() const { return name; }
    vector<Department*> getDepartments() const { return departments; }

    void accept(Visitor* visitor) override {
        visitor->visit(this);
    }
};

// === Конкретний відвідувач — "Зарплатна відомість" ===
class SalaryReportVisitor : public Visitor {
    double totalCompanySalary = 0;
    double currentDeptSalary = 0;
    string currentDeptName;

public:
    void visit(Company* company) override {
        cout << "\n===== Зарплатна відомість для компанії: " << company->getName() << " =====\n";

        for (auto dep : company->getDepartments()) {
            dep->accept(this);
            totalCompanySalary += currentDeptSalary;
            cout << "  -> Загальна зарплата у департаменті [" << currentDeptName << "]: "
                 << currentDeptSalary << " грн\n";
            currentDeptSalary = 0;
        }

        cout << "\n💰 Загальний фонд оплати праці компанії: " << totalCompanySalary << " грн\n";
    }

    void visit(Department* department) override {
        currentDeptName = department->getName();
        cout << "\n--- Департамент: " << currentDeptName << " ---\n";

        for (auto emp : department->getEmployees()) {
            emp->accept(this);
        }
    }

    void visit(Employee* employee) override {
        cout << "Посада: " << employee->getPosition()
             << " | Зарплата: " << employee->getSalary() << " грн\n";
        currentDeptSalary += employee->getSalary();
    }
};

// === Клієнтський код ===
int main() {
    SetConsoleOutputCP(65001);

    // Створюємо співробітників
    Employee* e1 = new Employee("Менеджер", 25000);
    Employee* e2 = new Employee("Аналітик", 20000);
    Employee* e3 = new Employee("Програміст", 30000);
    Employee* e4 = new Employee("Тестувальник", 22000);

    // Створюємо департаменти
    Department* d1 = new Department("Відділ Продажів", {e1, e2});
    Department* d2 = new Department("IT Відділ", {e3, e4});

    // Створюємо компанію
    Company* company = new Company("TechCorp", {d1, d2});

    // Створюємо відвідувача
    SalaryReportVisitor* reportVisitor = new SalaryReportVisitor();

    // Формуємо звіт для всієї компанії
    company->accept(reportVisitor);

    // Формуємо звіт тільки для одного департаменту
    cout << "\n\n===== Звіт тільки для одного департаменту =====\n";
    d2->accept(reportVisitor);

    // Прибираємо пам’ять
    delete e1; delete e2; delete e3; delete e4;
    delete d1; delete d2;
    delete company;
    delete reportVisitor;

    return 0;
}
