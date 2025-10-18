#include <iostream>
#include <string>
using namespace std;

// ===== Renderer Interface =====
class Renderer {
public:
    virtual string renderText(string text) = 0;
    virtual string renderImage(string url) = 0;
    virtual string renderLink(string url, string title) = 0;
    virtual ~Renderer() {}
};

// ===== HTML Renderer =====
class HTMLRenderer : public Renderer {
public:
    string renderText(string text) override {
        return "<p>" + text + "</p>";
    }
    string renderImage(string url) override {
        return "<img src='" + url + "' />";
    }
    string renderLink(string url, string title) override {
        return "<a href='" + url + "'>" + title + "</a>";
    }
};

// ===== JSON Renderer =====
class JsonRenderer : public Renderer {
public:
    string renderText(string text) override {
        return "{ \"text\": \"" + text + "\" }";
    }
    string renderImage(string url) override {
        return "{ \"image\": \"" + url + "\" }";
    }
    string renderLink(string url, string title) override {
        return "{ \"link\": \"" + url + "\", \"title\": \"" + title + "\" }";
    }
};

// ===== XML Renderer =====
class XmlRenderer : public Renderer {
public:
    string renderText(string text) override {
        return "<text>" + text + "</text>";
    }
    string renderImage(string url) override {
        return "<image>" + url + "</image>";
    }
    string renderLink(string url, string title) override {
        return "<link url='" + url + "'>" + title + "</link>";
    }
};

// ===== Абстракція Page =====
class Page {
protected:
    Renderer* renderer;
public:
    Page(Renderer* r) : renderer(r) {}
    virtual string view() = 0;
    virtual ~Page() {}
};

// ===== Simple Page =====
class SimplePage : public Page {
private:
    string title;
    string content;
public:
    SimplePage(Renderer* r, string t, string c)
        : Page(r), title(t), content(c) {}

    string view() override {
        return renderer->renderText(title) + "\n" +
               renderer->renderText(content);
    }
};

// ===== Product =====
class Product {
public:
    string id;
    string name;
    string description;
    string image;

    Product(string i, string n, string d, string img)
        : id(i), name(n), description(d), image(img) {}
};

// ===== Product Page =====
class ProductPage : public Page {
private:
    Product* product;
public:
    ProductPage(Renderer* r, Product* p)
        : Page(r), product(p) {}

    string view() override {
        return renderer->renderText(product->name) + "\n" +
               renderer->renderText(product->description) + "\n" +
               renderer->renderImage(product->image) + "\n" +
               renderer->renderLink("/product/" + product->id, "View Product");
    }
};

// ===== Клієнтський код =====
int main() {
    Renderer* html = new HTMLRenderer();
    Renderer* json = new JsonRenderer();
    Renderer* xml = new XmlRenderer();

    Page* simple = new SimplePage(html, "About Us", "Welcome to our site!");
    cout << "HTML Simple Page:\n" << simple->view() << "\n\n";

    Page* simpleJson = new SimplePage(json, "About Us", "Welcome to our site!");
    cout << "JSON Simple Page:\n" << simpleJson->view() << "\n\n";

    Product* product = new Product("101", "Laptop", "Powerful gaming laptop", "laptop.jpg");
    Page* productPageHtml = new ProductPage(html, product);
    cout << "HTML Product Page:\n" << productPageHtml->view() << "\n\n";

    Page* productPageXml = new ProductPage(xml, product);
    cout << "XML Product Page:\n" << productPageXml->view() << "\n\n";

    // прибирання
    delete html;
    delete json;
    delete xml;
    delete simple;
    delete simpleJson;
    delete product;
    delete productPageHtml;
    delete productPageXml;

    return 0;
}
