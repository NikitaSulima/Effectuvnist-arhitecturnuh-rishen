#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

// ===== Інтерфейс Стратегії =====
class DeliveryStrategy {
public:
    virtual double calculateCost(double distance, double weight) = 0;
    virtual string getName() = 0;
    virtual ~DeliveryStrategy() {}
};

// ===== Конкретна стратегія 1: Самовивіз =====
class SelfPickupStrategy : public DeliveryStrategy {
public:
    double calculateCost(double distance, double weight) override {
        // Самовивіз — безкоштовна доставка
        return 0.0;
    }
    string getName() override {
        return "Самовивіз";
    }
};

// ===== Конкретна стратегія 2: Зовнішня служба =====
class ExternalDeliveryStrategy : public DeliveryStrategy {
public:
    double calculateCost(double distance, double weight) override {
        // Формула може бути складною, але ми просто імітуємо:
        return 5.0 + distance * 0.5 + weight * 0.2;
    }
    string getName() override {
        return "Зовнішня служба доставки";
    }
};

// ===== Конкретна стратегія 3: Власна служба =====
class InternalDeliveryStrategy : public DeliveryStrategy {
public:
    double calculateCost(double distance, double weight) override {
        // Власна служба може мати знижки або іншу логіку
        return 3.0 + distance * 0.3 + weight * 0.1;
    }
    string getName() override {
        return "Власна служба доставки";
    }
};

// ===== Контекст, який використовує стратегію =====
class DeliveryContext {
private:
    DeliveryStrategy* strategy;  // поточна стратегія доставки
public:
    DeliveryContext(DeliveryStrategy* s = nullptr)
        : strategy(s) {}

    void setStrategy(DeliveryStrategy* s) {
        strategy = s;
    }

    void calculate(double distance, double weight) {
        if (!strategy) {
            cout << "❌ Стратегію не вибрано!\n";
            return;
        }
        double cost = strategy->calculateCost(distance, weight);
        cout << "Обрана стратегія: " << strategy->getName() << endl;
        cout << "Відстань: " << distance << " км, Вага: " << weight << " кг" << endl;
        cout << "Вартість доставки: " << cost << " грн\n\n";
    }
};

// ===== Клієнтський код =====
int main() {
    SetConsoleOutputCP(65001);
    // Стратегії
    SelfPickupStrategy selfPickup;
    ExternalDeliveryStrategy externalDelivery;
    InternalDeliveryStrategy internalDelivery;

    // Контекст
    DeliveryContext context;

    cout << "=== Розрахунок вартості доставки ===\n\n";

    // 1️Самовивіз
    context.setStrategy(&selfPickup);
    context.calculate(10, 5);

    // 2️Зовнішня служба
    context.setStrategy(&externalDelivery);
    context.calculate(10, 5);

    // 3️Власна служба
    context.setStrategy(&internalDelivery);
    context.calculate(10, 5);

    return 0;
}
