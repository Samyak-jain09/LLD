#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

// Subject
class DatabaseService {
public:
    virtual string query(const string& sql) = 0;
    virtual ~DatabaseService() = default;
};

// Real Subject
class RealDatabaseService : public DatabaseService {
public:
    string query(const string& sql) override {
        cout << "Executing query on Real Database...\n";

        // Simulate expensive database operation
        return "Result for: " + sql;
    }
};

// Proxy
class CachingDatabaseProxy : public DatabaseService {
private:
    unique_ptr<DatabaseService> realDatabase;
    unordered_map<string, string> cache;

public:
    CachingDatabaseProxy(unique_ptr<DatabaseService> db)
        : realDatabase(move(db)) {}

    string query(const string& sql) override {

        auto it = cache.find(sql);

        if (it != cache.end()) {
            cout << "Cache Hit\n";
            return it->second;
        }

        cout << "Cache Miss\n";

        string result = realDatabase->query(sql);

        cache[sql] = result;

        return result;
    }
};

int main() {

    unique_ptr<DatabaseService> realDB =
        make_unique<RealDatabaseService>();

    CachingDatabaseProxy proxy(move(realDB));

    string q1 = "SELECT * FROM USERS";
    string q2 = "SELECT * FROM PRODUCTS";

    cout << proxy.query(q1) << "\n\n";

    cout << proxy.query(q1) << "\n\n";

    cout << proxy.query(q2) << "\n\n";

    cout << proxy.query(q2) << "\n\n";

    cout << proxy.query(q1) << "\n";

    return 0;
}