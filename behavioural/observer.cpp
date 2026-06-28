#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

// Forward declaration
class StockObservable;

// ================= Observer Interface =================

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(int stockCount) = 0;
};

// ================= Observable Interface =================

class StockObservable {
public:
    virtual ~StockObservable() = default;

    virtual void addObserver(Observer* observer) = 0;
    virtual void removeObserver(Observer* observer) = 0;
    virtual void notifyObservers() = 0;

    virtual void addStock(int quantity) = 0;
    virtual void sellItem() = 0;
    virtual int getStockCount() const = 0;
};

// ================= Email Observer =================

class EmailNotification : public Observer {
private:
    std::string email;

public:
    EmailNotification(const std::string& email)
        : email(email) {}

    void update(int stockCount) override {
        std::cout << "[Email] Sent notification to "
                  << email
                  << " | Current Stock = "
                  << stockCount << '\n';
    }
};

// ================= Mobile Observer =================

class MobileNotification : public Observer {
private:
    std::string mobileNumber;

public:
    MobileNotification(const std::string& mobile)
        : mobileNumber(mobile) {}

    void update(int stockCount) override {
        std::cout << "[SMS] Sent notification to "
                  << mobileNumber
                  << " | Current Stock = "
                  << stockCount << '\n';
    }
};

// ================= Concrete Observable =================

class IphoneObservable : public StockObservable {
private:
    std::unordered_set<Observer*> observers;
    int stockCount = 0;

public:
    void addObserver(Observer* observer) override {
        observers.insert(observer);
    }

    void removeObserver(Observer* observer) override {
        observers.erase(observer);
    }

    void notifyObservers() override {
        for (Observer* observer : observers) {
            observer->update(stockCount);
        }
    }

    void addStock(int quantity) override {
        bool wasOutOfStock = (stockCount == 0);

        stockCount += quantity;

        std::cout << "\nAdded " << quantity
                  << " item(s). Stock = "
                  << stockCount << '\n';

        if (wasOutOfStock) {
            notifyObservers();
        }
    }

    void sellItem() override {
        if (stockCount == 0) {
            std::cout << "Out of stock!\n";
            return;
        }

        stockCount--;

        std::cout << "Sold one iPhone. Remaining Stock = "
                  << stockCount << '\n';
    }

    int getStockCount() const override {
        return stockCount;
    }
};

// ================= Driver =================

int main() {

    auto iphoneStock = std::make_unique<IphoneObservable>();

    auto email1 = std::make_unique<EmailNotification>(
        "abc@gmail.com");

    auto email2 = std::make_unique<EmailNotification>(
        "xyz@gmail.com");

    auto sms1 = std::make_unique<MobileNotification>(
        "9876543210");

    iphoneStock->addObserver(email1.get());
    iphoneStock->addObserver(email2.get());
    iphoneStock->addObserver(sms1.get());

    iphoneStock->addStock(5);

    std::cout << "\n";

    iphoneStock->sellItem();
    iphoneStock->sellItem();
    iphoneStock->sellItem();
    iphoneStock->sellItem();
    iphoneStock->sellItem();

    std::cout << "\n";

    iphoneStock->addStock(2);

    return 0;
}