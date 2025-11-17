#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

// === Інтерфейс Посередника ===
class Mediator {
public:
    virtual void notify(string sender, string event) = 0;
};

// === Базовий клас Компонента ===
class Component {
protected:
    Mediator* mediator;

public:
    void setMediator(Mediator* m) { mediator = m; }
    virtual ~Component() = default;
};

// === Конкретні елементи форми ===

// 1️Вибір дати доставки
class DeliveryDatePicker : public Component {
    string date;
public:
    void selectDate(string newDate) {
        date = newDate;
        cout << "[Дата доставки] Обрано: " << date << endl;
        mediator->notify("DeliveryDate", "DateChanged");
    }
    string getDate() const { return date; }
};

// 2️Вибір проміжку часу
class TimeSlotSelector : public Component {
    vector<string> availableSlots;
public:
    void updateAvailableSlots(vector<string> slots) {
        availableSlots = slots;
        cout << "[Час доставки] Доступні слоти оновлено: ";
        for (auto& s : availableSlots) cout << s << " ";
        cout << endl;
    }
};

// 3️Чекбокс “Отримувач інша особа”
class OtherPersonCheckbox : public Component {
    bool checked = false;
public:
    void toggle(bool state) {
        checked = state;
        cout << "[Отримувач інша особа] Стан: " << (checked ? "Так" : "Ні") << endl;
        mediator->notify("OtherPersonCheckbox", "Toggled");
    }
    bool isChecked() const { return checked; }
};

// 4️Поля Ім’я та Телефон
class RecipientNameField : public Component {
public:
    void setVisible(bool visible) {
        cout << "[Поле Ім’я] " << (visible ? "Показано" : "Сховано") << endl;
    }
};

class RecipientPhoneField : public Component {
public:
    void setVisible(bool visible) {
        cout << "[Поле Телефон] " << (visible ? "Показано" : "Сховано") << endl;
    }
};

// 5️Чекбокс “Самовивіз”
class PickupCheckbox : public Component {
    bool checked = false;
public:
    void toggle(bool state) {
        checked = state;
        cout << "[Самовивіз] Стан: " << (checked ? "Так" : "Ні") << endl;
        mediator->notify("PickupCheckbox", "Toggled");
    }
    bool isChecked() const { return checked; }
};

// === Конкретний Посередник ===
class OrderFormMediator : public Mediator {
private:
    DeliveryDatePicker* datePicker;
    TimeSlotSelector* timeSelector;
    OtherPersonCheckbox* otherCheckbox;
    RecipientNameField* nameField;
    RecipientPhoneField* phoneField;
    PickupCheckbox* pickupCheckbox;

public:
    OrderFormMediator(DeliveryDatePicker* d, TimeSlotSelector* t,
                      OtherPersonCheckbox* o, RecipientNameField* n,
                      RecipientPhoneField* p, PickupCheckbox* pick) :
                      datePicker(d), timeSelector(t),
                      otherCheckbox(o), nameField(n),
                      phoneField(p), pickupCheckbox(pick) {
        d->setMediator(this);
        t->setMediator(this);
        o->setMediator(this);
        n->setMediator(this);
        p->setMediator(this);
        pick->setMediator(this);
    }

    void notify(string sender, string event) override {
        if (sender == "DeliveryDate" && event == "DateChanged") {
            // При зміні дати — оновлюємо доступні часові слоти
            cout << "[Посередник] Оновлюємо часові слоти відповідно до дати.\n";
            if (datePicker->getDate() == "Сьогодні")
                timeSelector->updateAvailableSlots({"12:00", "14:00", "16:00"});
            else
                timeSelector->updateAvailableSlots({"10:00", "12:00", "15:00", "18:00"});
        }

        else if (sender == "OtherPersonCheckbox" && event == "Toggled") {
            // Якщо “отримувач інша особа” — показуємо додаткові поля
            bool state = otherCheckbox->isChecked();
            nameField->setVisible(state);
            phoneField->setVisible(state);
        }

        else if (sender == "PickupCheckbox" && event == "Toggled") {
            // Якщо “самовивіз” — блокуємо поля доставки
            bool state = pickupCheckbox->isChecked();
            cout << "[Посередник] Самовивіз: " << (state ? "Вимикаємо доставку." : "Увімкнено доставку.") << endl;
            if (state) {
                timeSelector->updateAvailableSlots({});
                nameField->setVisible(false);
                phoneField->setVisible(false);
            }
        }
    }
};

// === Клієнтський код ===
int main() {
    SetConsoleOutputCP(65001);

    // Створюємо елементи форми
    DeliveryDatePicker date;
    TimeSlotSelector timeSlots;
    OtherPersonCheckbox otherPerson;
    RecipientNameField nameField;
    RecipientPhoneField phoneField;
    PickupCheckbox pickup;

    // Створюємо посередника
    OrderFormMediator mediator(&date, &timeSlots, &otherPerson, &nameField, &phoneField, &pickup);

    cout << "\n=== Демонстрація роботи патерну Посередник ===\n\n";

    // 1. Користувач обирає дату доставки
    date.selectDate("Сьогодні");

    // 2. Користувач вказує, що отримувач інша особа
    otherPerson.toggle(true);

    // 3. Користувач передумав — сам забере квіти
    pickup.toggle(true);

    // 4. Потім знову передумав — хоче доставку
    pickup.toggle(false);

    return 0;
}
