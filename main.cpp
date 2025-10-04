#include <iostream>
#include <string>
using namespace std;

// Абстрактний продук
class SocialNetwork {
public:
    virtual void connect(string id, string password) = 0;
    virtual void publishMessage(string message) = 0;
    virtual ~SocialNetwork() {}
};

// Конкретні продукти
class Facebook : public SocialNetwork {
private:
    string login;
    string password;
public:
    void connect(string login, string password) override {
        this->login = login;
        this->password = password;
        cout << "[Facebook] Connecting user " << login << endl;
    }
    void publishMessage(string message) override {
        cout << "[Facebook] Publishing: " << message << endl;
    }
};

class LinkedIn : public SocialNetwork {
private:
    string email;
    string password;
public:
    void connect(string email, string password) override {
        this->email = email;
        this->password = password;
        cout << "[LinkedIn] Connecting user " << email << endl;
    }
    void publishMessage(string message) override {
        cout << "[LinkedIn] Publiching: " << message << endl;
    }
};

// Абстрактна фабрика
class SocialNetworkCreator {
public:
    virtual SocialNetwork* createNetwork(string id, string password) = 0;
    virtual ~SocialNetworkCreator() {}
};

// Конкретні фабрики
class FacebookCreator : public SocialNetworkCreator {
public:
    SocialNetwork* createNetwork(string login, string password) override {
        Facebook* fb = new Facebook();
        fb->connect(login, password);
        return fb;
    }
};

class LinkedInCreator : public SocialNetworkCreator {
public:
    SocialNetwork* createNetwork(string email, string password) override {
        LinkedIn* li = new LinkedIn();
        li->connect(email, password);
        return li;
    }
};

// Клієнтський код
int main() {
    // Facebook
    SocialNetworkCreator* fbCreator = new FacebookCreator();
    SocialNetwork* fb = fbCreator->createNetwork("nikita_user", "12345");
    fb->publishMessage("Hello, it`s my first post on Facebook!");

    cout << "--------------------------" << endl;

    // LinkedIn
    SocialNetworkCreator* liCreator = new LinkedInCreator();
    SocialNetwork* li = liCreator->createNetwork("kate@mail.com", "qwerty");
    li->publishMessage("It`s my post on LinkedIn!");

    // прибирання
    delete fb;
    delete li;
    delete fbCreator;
    delete liCreator;

    return 0;
}
