#include <iostream>
#include <string>
using namespace std;

// Інтерфейс
class IQueryBuilder {
public:
    virtual IQueryBuilder* select(string fields) = 0;
    virtual IQueryBuilder* where(string condition) = 0;
    virtual IQueryBuilder* limit(int n) = 0;
    virtual string getSQL() = 0;
    virtual ~IQueryBuilder() {}
};

// PostgreSQL Builder
class PostgreSQLQueryBuilder : public IQueryBuilder {
private:
    string query;
public:
    IQueryBuilder* select(string fields) override {
        query = "SELECT " + fields;
        return this;
    }
    IQueryBuilder* where(string condition) override {
        query += " WHERE " + condition;
        return this;
    }
    IQueryBuilder* limit(int n) override {
        query += " LIMIT " + to_string(n);
        return this;
    }
    string getSQL() override {
        return query + ";";
    }
};

// MySQL Builder
class MySQLQueryBuilder : public IQueryBuilder {
private:
    string query;
public:
    IQueryBuilder* select(string fields) override {
        query = "SELECT " + fields;
        return this;
    }
    IQueryBuilder* where(string condition) override {
        query += " WHERE " + condition;
        return this;
    }
    IQueryBuilder* limit(int n) override {
        query += " LIMIT " + to_string(n);
        return this;
    }
    string getSQL() override {
        return query + ";";
    }
};

// Клієнтський код
int main() {
    // PostgreSQL
    IQueryBuilder* pgBuilder = new PostgreSQLQueryBuilder();
    string pgSQL = pgBuilder->select("*")
                             ->where("age > 18")
                             ->limit(10)
                             ->getSQL();
    cout << "[PostgreSQL] " << pgSQL << endl;

    // MySQL
    IQueryBuilder* myBuilder = new MySQLQueryBuilder();
    string mySQL = myBuilder->select("id, name")
                             ->where("status = 'active'")
                             ->limit(5)
                             ->getSQL();
    cout << "[MySQL] " << mySQL << endl;

    delete pgBuilder;
    delete myBuilder;
    return 0;
}
