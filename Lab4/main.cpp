#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

// Інтерфейс Notification
class Notification {
public:
    virtual void send(string title, string message) = 0;
    virtual ~Notification() {}
};

// Існуючий клас EmailNotification
class EmailNotification : public Notification {
private:
    string adminEmail;

public:
    EmailNotification(string email) : adminEmail(email) {}

    void send(string title, string message) override {
        cout << "[Email] Надіслано лист на " << adminEmail
             << " | Тема: " << title
             << " | Повідомлення: " << message << endl;
    }
};

// Класи сторонніх сервісів (які ми хочемо адаптувати)

// Slack API
class SlackService {
private:
    string login;
    string apiKey;
    string chatId;

public:
    SlackService(string login, string apiKey, string chatId)
        : login(login), apiKey(apiKey), chatId(chatId) {}

    void auth() {
        cout << "[Slack] Авторизація користувача " << login << "..." << endl;
    }

    void sendToChat(string message) {
        cout << "[Slack] Відправлено у чат " << chatId << ": " << message << endl;
    }
};

// SMS API
class SmsService {
private:
    string phone;
    string sender;

public:
    SmsService(string phone, string sender)
        : phone(phone), sender(sender) {}

    void connect() {
        cout << "[SMS] Підключення до сервера для відправки SMS..." << endl;
    }

    void sendSMS(string text) {
        cout << "[SMS] Від " << sender << " до " << phone << ": " << text << endl;
    }
};

// Адаптери (реалізують Notification, але працюють через інші сервіси)

// Slack Adapter
class SlackNotificationAdapter : public Notification {
private:
    SlackService* slack;

public:
    SlackNotificationAdapter(SlackService* service) : slack(service) {}

    void send(string title, string message) override {
        slack->auth();
        slack->sendToChat(title + ": " + message);
    }
};

// SMS Adapter
class SmsNotificationAdapter : public Notification {
private:
    SmsService* sms;

public:
    SmsNotificationAdapter(SmsService* service) : sms(service) {}

    void send(string title, string message) override {
        sms->connect();
        sms->sendSMS(title + " — " + message);
    }
};

// Клієнтський код
int main() {
    SetConsoleOutputCP(65001);
    // Email
    Notification* email = new EmailNotification("admin@mail.com");
    email->send("Вітання", "Ваш лист доставлено успішно!");

    cout << "---------------------------" << endl;

    // Slack
    SlackService* slackService = new SlackService("user123", "ABC-KEY-999", "dev-chat");
    Notification* slack = new SlackNotificationAdapter(slackService);
    slack->send("Нове повідомлення", "Код виконано без помилок.");

    cout << "---------------------------" << endl;

    // SMS
    SmsService* smsService = new SmsService("+380123456789", "System");
    Notification* sms = new SmsNotificationAdapter(smsService);
    sms->send("Попередження", "Закінчується місце на диску.");

    // Прибирання
    delete email;
    delete slack;
    delete sms;
    delete slackService;
    delete smsService;

    return 0;
}
