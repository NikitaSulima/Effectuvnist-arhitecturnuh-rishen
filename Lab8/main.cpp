#include <iostream>
#include <string>
#include <map>
#include <windows.h>

using namespace std;

// === Абстрактний базовий клас ===
class EntityUpdater {
public:
    virtual ~EntityUpdater() = default;

    // --- Шаблонний метод ---
    void update(int entityId, map<string, string> newData) {
        auto entity = getEntity(entityId);

        if (!validateData(entity, newData)) {
            onValidationFailed(entity, newData);
            cout << "Validation failed. Update aborted.\n";
            return;
        }

        auto request = buildSaveRequest(entity, newData);
        auto response = sendRequest(request);
        auto finalResponse = formatResponse(response, entity);

        afterUpdateHook(entity, finalResponse);
        cout << "Update complete.\n\n";
    }

protected:
    // === Абстрактні методи (кроки алгоритму) ===
    virtual map<string, string> getEntity(int entityId) = 0;
    virtual bool validateData(map<string, string>& entity, map<string, string>& newData) = 0;
    virtual map<string, string> buildSaveRequest(map<string, string>& entity, map<string, string>& newData) = 0;
    virtual map<string, string> sendRequest(map<string, string>& request) = 0;

    // === Методи за замовчуванням ===
    virtual map<string, string> formatResponse(map<string, string>& response, map<string, string>& entity) {
        return {{"status", "success"}, {"code", "200"}};
    }

    // === Хуки (точки розширення) ===
    virtual void onValidationFailed(map<string, string>& entity, map<string, string>& newData) {}
    virtual void afterUpdateHook(map<string, string>& entity, map<string, string>& response) {}
};


// === Реалізація для Product ===
class ProductUpdater : public EntityUpdater {
protected:
    map<string, string> getEntity(int entityId) override {
        cout << "[Product] Отримання продукту ID: " << entityId << endl;
        return {{"id", to_string(entityId)}, {"name", "Laptop"}};
    }

    bool validateData(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[Product] Перевірка валідності даних...\n";
        if (newData["price"].empty() || stoi(newData["price"]) <= 0)
            return false;
        return true;
    }

    void onValidationFailed(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[Product] Валідація не пройдена — надсилаємо сповіщення адміністратору в месенджер.\n";
    }

    map<string, string> buildSaveRequest(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[Product] Формування запиту для збереження...\n";
        return {{"product_id", entity["id"]}, {"data", newData["price"]}};
    }

    map<string, string> sendRequest(map<string, string>& request) override {
        cout << "[Product] Надсилання запиту...\n";
        return {{"status", "ok"}};
    }
};


// === Реалізація для User ===
class UserUpdater : public EntityUpdater {
protected:
    map<string, string> getEntity(int entityId) override {
        cout << "[User] Отримання користувача ID: " << entityId << endl;
        return {{"id", to_string(entityId)}, {"name", "Danylo"}, {"email", "user@mail.com"}};
    }

    bool validateData(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[User] Перевірка даних...\n";
        if (newData.find("email") != newData.end()) {
            cout << "[User] Поле 'email' заборонено змінювати! Видаляємо його.\n";
            newData.erase("email");
        }
        return true;
    }

    map<string, string> buildSaveRequest(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[User] Формування запиту на оновлення користувача...\n";
        return {{"user_id", entity["id"]}, {"update_name", newData["name"]}};
    }

    map<string, string> sendRequest(map<string, string>& request) override {
        cout << "[User] Надсилання запиту...\n";
        return {{"status", "updated"}};
    }
};


// === Реалізація для Order ===
class OrderUpdater : public EntityUpdater {
protected:
    map<string, string> getEntity(int entityId) override {
        cout << "[Order] Отримання замовлення ID: " << entityId << endl;
        return {{"id", to_string(entityId)}, {"item", "Laptop"}, {"total", "1500"}};
    }

    bool validateData(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[Order] Валідація даних...\n";
        return true;
    }

    map<string, string> buildSaveRequest(map<string, string>& entity, map<string, string>& newData) override {
        cout << "[Order] Формування запиту для оновлення замовлення...\n";
        return {{"order_id", entity["id"]}, {"status", newData["status"]}};
    }

    map<string, string> sendRequest(map<string, string>& request) override {
        cout << "[Order] Надсилання запиту...\n";
        return {{"status", "success"}};
    }

    map<string, string> formatResponse(map<string, string>& response, map<string, string>& entity) override {
        cout << "[Order] Формування розширеної відповіді...\n";
        string orderInfo = "{id: " + entity["id"] + ", item: " + entity["item"] + ", total: " + entity["total"] + "}";
        return {{"status", "success"}, {"code", "200"}, {"order_data", orderInfo}};
    }
};


// === Клієнтський код ===
int main() {
    SetConsoleOutputCP(65001);
    cout << "\n---Оновлення продукту ---\n";
    ProductUpdater product;
    product.update(1, {{"price", "-10"}});

    cout << "\n---Оновлення користувача ---\n";
    UserUpdater user;
    user.update(2, {{"name", "Ivan"}, {"email", "new@mail.com"}});

    cout << "\n---Оновлення замовлення ---\n";
    OrderUpdater order;
    order.update(3, {{"status", "shipped"}});
}
