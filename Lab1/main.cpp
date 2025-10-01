#include <iostream>
#include <string>

using namespace std;

// Базовий інтерфейс сховища
class IStorage {
public:
    virtual void connect() = 0;
    virtual void uploadFile(string filePath) = 0;
    virtual void downloadFile(string fileName) = 0;
    virtual ~IStorage() {}
};

// Реалізація: Локальний диск
class LocalDiskStorage : public IStorage {
public:
    void connect() override {
        cout << "[LocalDisk] Підключення до локального диску..." << endl;
    }
    void uploadFile(std::string filePath) override {
        cout << "[LocalDisk] Завантаження файлу: " << filePath << endl;
    }
    void downloadFile(std::string fileName) override {
        cout << "[LocalDisk] Завантаження файлу на ПК: " << fileName << endl;
    }
};

// Реалізація: Amazon S3
class AmazonS3Storage : public IStorage {
public:
    void connect() override {
        cout << "[AmazonS3] Підключення до Amazon S3..." << endl;
    }
    void uploadFile(std::string filePath) override {
        cout << "[AmazonS3] Завантаження файлу у S3: " << filePath << endl;
    }
    void downloadFile(std::string fileName) override {
        cout << "[AmazonS3] Завантаження файлу з S3: " << fileName << endl;
    }
};

// Singleton: Менеджер сховищ
class StorageManager {
private:
    static StorageManager* instance;  // єдиний екземпляр
    IStorage* storage;                // вибране сховище

    StorageManager() : storage(nullptr) {}

public:

    StorageManager(const StorageManager&) = delete;
    StorageManager& operator=(const StorageManager&) = delete;

    static StorageManager* getInstance() {
        if (instance == nullptr) {
            instance = new StorageManager();
        }
        return instance;
    }

    // Вибір сховища
    void setStorage(IStorage* s) {
        storage = s;
        storage->connect();
    }

    // Методи роботи з файлами
    void upload(string filePath) {
        if (storage) storage->uploadFile(filePath);
        else cout << "Сховище не вибране!" << endl;
    }

    void download(string fileName) {
        if (storage) storage->downloadFile(fileName);
        else cout << "Сховище не вибране!" << endl;
    }
};

// Ініціалізація статичного члена класу
StorageManager* StorageManager::instance = nullptr;

// Клієнтський код
int main() {
    // Отримуємо єдиний екземпляр Singleton

    StorageManager* manager = StorageManager::getInstance();

    // 1) Використання локального диску
    IStorage* local = new LocalDiskStorage();
    manager->setStorage(local);
    manager->upload("document.txt");
    manager->download("presentation.pptx");

    cout << "---------------------------" << endl;

    // 2) Використання Amazon S3
    IStorage* s3 = new AmazonS3Storage();
    manager->setStorage(s3);
    manager->upload("report.pdf");
    manager->download("backup.zip");

    return 0;
}
