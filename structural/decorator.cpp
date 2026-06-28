#include <iostream>
#include <memory>

using namespace std;

class BasePizza {
public:
    virtual ~BasePizza() = default;
    virtual int cost() const = 0;
};



// Concrete Components

class Margherita : public BasePizza {
public:
    int cost() const override {
        return 100;
    }
};

class VegDelight : public BasePizza {
public:
    int cost() const override {
        return 150;
    }
};



// Base Decorator

class PizzaDecorator : public BasePizza {
protected:
    unique_ptr<BasePizza> pizza;

public:
    PizzaDecorator(unique_ptr<BasePizza> pizza)
        : pizza(move(pizza)) {}
};



// Concrete Decorators

class ExtraCheese : public PizzaDecorator {
public:
    ExtraCheese(unique_ptr<BasePizza> pizza)
        : PizzaDecorator(move(pizza)) {}

    int cost() const override {
        return pizza->cost() + 10;
    }
};

class Mushroom : public PizzaDecorator {
public:
    Mushroom(unique_ptr<BasePizza> pizza)
        : PizzaDecorator(move(pizza)) {}

    int cost() const override {
        return pizza->cost() + 20;
    }
};

class Olives : public PizzaDecorator {
public:
    Olives(unique_ptr<BasePizza> pizza)
        : PizzaDecorator(move(pizza)) {}

    int cost() const override {
        return pizza->cost() + 15;
    }
};



int main() {

    unique_ptr<BasePizza> pizza =
        make_unique<ExtraCheese>(
            make_unique<Mushroom>(
                make_unique<VegDelight>()));

    cout << "Cost = " << pizza->cost() << endl;

    return 0;
}