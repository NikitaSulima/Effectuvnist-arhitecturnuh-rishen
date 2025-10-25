#include <iostream>
#include <string>
#include <map>
#include <windows.h>
using namespace std;

// Інтерфейс завантажувача
class Downloader {
public:
    virtual string download(const string& url) = 0;
    virtual ~Downloader() {}
};

// Реальна реалізація завантажувача
class SimpleDownloader : public Downloader {
public:
    string download(const string& url) override {

        cout << "[SimpleDownloader] Завантаження файлу з: " << url << endl;
        // Повертаємо "завантажені дані" у вигляді рядка
        return "Дані_файлу_з_" + url;
    }
};

// Проксі з кешуванням
class CachedDownloader : public Downloader {
private:
    Downloader* realDownloader;
    map<string, string> cache;

public:
    // Проксі приймає вказівник на реальний завантажувач
    CachedDownloader(Downloader* downloader)
        : realDownloader(downloader) {}

    ~CachedDownloader() {
    }

    string download(const string& url) override {
        auto it = cache.find(url);
        if (it != cache.end()) {
            cout << "[Proxy] Отримано з кешу: " << url << endl;
            return it->second;
        }

        cout << "[Proxy] Кеш відсутній. Завантажуємо: " << url << endl;
        string data = realDownloader->download(url);
        cache[url] = data;
        return data;
    }

    void clearCache() {
        cache.clear();
    }

    bool hasInCache(const string& url) const {
        return cache.find(url) != cache.end();
    }
};

// Демонстрація використання
int main() {
    SetConsoleOutputCP(65001);
    // Створимо реальний завантажувач
    SimpleDownloader* real = new SimpleDownloader();

    // Створимо проксі, який використовуватиме реальний завантажувач
    CachedDownloader* proxy = new CachedDownloader(real);

    cout << "=== Виклик напряму через SimpleDownloader ===" << endl;
    string r1 = real->download("http://example.com/file1.txt");

    cout << "\n=== Виклики через CachedDownloader (Proxy) ===" << endl;
    string p1 = proxy->download("http://example.com/file1.txt"); // завантажить і збереже в кеш
    string p2 = proxy->download("http://example.com/file1.txt"); // візьме з кешу
    string p3 = proxy->download("http://example.com/file2.txt"); // завантажить інший файл

    cout << "\n=== Результати (плохоформатований вивід даних) ===" << endl;
    cout << "real: " << r1 << endl;
    cout << "proxy p1: " << p1 << endl;
    cout << "proxy p2: " << p2 << endl;
    cout << "proxy p3: " << p3 << endl;

    // Очищення пам'яті
    delete proxy;
    delete real;

    return 0;
}
