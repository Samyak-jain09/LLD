#include <bits/stdc++.h>
using namespace std;

enum class Coin : int { PENNY=1, NICKEL=5, DIME=10, QUARTER=25 };
enum class ItemType : int { COKE, PEPSI, SODA, WATER };

class Item {
public:
    Item(ItemType type, int price) : type(type), price(price) {}
    ItemType getType() const { return type; }
    int getPrice() const { return price; }
private:
    ItemType type;
    int price;
};

class Inventory {
public:
    void setItem(Item item) { this->item = item; }
    Item getItem() const { return item; }
private:
    Item item{ItemType::COKE,25};
};

// Forward declarations
class State;
class IdleState;
class HasMoneyState;
class SelectionState;
class DispenseState;

class VendingMachine {
public:
    void setVendingMachineState(State* state) { vendingMachineState = state; }
    State* getState() { return vendingMachineState; }
    void addCoin(Coin coin) { coinList.push_back(coin); }
    int getTotalMoney() {
        int sum=0; for(auto c:coinList) sum+=static_cast<int>(c);
        return sum;
    }
    void resetCoins() { coinList.clear(); }
private:
    State* vendingMachineState=nullptr;
    Inventory inventory;
    vector<Coin> coinList;
};

class State {
public:
    virtual void clickOnInsertCoinButton(VendingMachine&)=0;
    virtual void clickOnStartProductSelectionButton(VendingMachine&)=0;
    virtual void insertCoin(VendingMachine&,Coin)=0;
    virtual void chooseProduct(VendingMachine&,int)=0;
    virtual int getChange(int)=0;
    virtual Item dispenseProduct(VendingMachine&,int)=0;
    virtual vector<Coin> refundFullMoney(VendingMachine&)=0;
    virtual ~State() {}
};

// ------------------ IdleState ------------------
class IdleState:public State{
public:
    IdleState(){ cout<<"Machine is in IdleState\n"; }
    void clickOnInsertCoinButton(VendingMachine& m) override; // declared only
    void clickOnStartProductSelectionButton(VendingMachine&) override {
        cout<<"Cannot start product selection in IdleState\n";
    }
    void insertCoin(VendingMachine&,Coin) override { cout<<"Click insert coin first\n"; }
    void chooseProduct(VendingMachine&,int) override { cout<<"Cannot choose product in IdleState\n"; }
    int getChange(int) override { return 0; }
    Item dispenseProduct(VendingMachine&,int) override { return Item(ItemType::COKE,0); }
    vector<Coin> refundFullMoney(VendingMachine&) override { return {}; }
};

// ------------------ HasMoneyState ------------------
class HasMoneyState:public State{
public:
    HasMoneyState(){ cout<<"Machine is in HasMoneyState\n"; }
    void clickOnInsertCoinButton(VendingMachine&) override { cout<<"Already in coin insertion mode\n"; }
    void clickOnStartProductSelectionButton(VendingMachine& m) override; // declared only
    void insertCoin(VendingMachine& m,Coin coin) override {
        m.addCoin(coin);
        cout<<"Coin inserted: "<<static_cast<int>(coin)<<"\n";
    }
    void chooseProduct(VendingMachine&,int) override { cout<<"Click product selection first\n"; }
    int getChange(int r) override { return r; }
    Item dispenseProduct(VendingMachine&,int) override { return Item(ItemType::COKE,0); }
    vector<Coin> refundFullMoney(VendingMachine& m) override { m.resetCoins(); return {}; }
};

// ------------------ SelectionState ------------------
class SelectionState:public State{
public:
    SelectionState(){ cout<<"Machine is in SelectionState\n"; }
    void clickOnInsertCoinButton(VendingMachine&) override { cout<<"Already inserted coins\n"; }
    void clickOnStartProductSelectionButton(VendingMachine&) override { cout<<"Already in selection mode\n"; }
    void insertCoin(VendingMachine&,Coin) override { cout<<"Cannot insert coin during selection\n"; }
    void chooseProduct(VendingMachine& m,int code) override; // declared only
    int getChange(int r) override { return r; }
    Item dispenseProduct(VendingMachine&,int) override { return Item(ItemType::COKE,0); }
    vector<Coin> refundFullMoney(VendingMachine& m) override { m.resetCoins(); return {}; }
};

// ------------------ DispenseState ------------------
class DispenseState:public State{
public:
    DispenseState(){ cout<<"Machine is in DispenseState\n"; }
    void clickOnInsertCoinButton(VendingMachine&) override { cout<<"Wait, dispensing\n"; }
    void clickOnStartProductSelectionButton(VendingMachine&) override { cout<<"Wait, dispensing\n"; }
    void insertCoin(VendingMachine&,Coin) override { cout<<"Cannot insert coin while dispensing\n"; }
    void chooseProduct(VendingMachine&,int) override { cout<<"Already dispensing\n"; }
    int getChange(int r) override { cout<<"Returning change "<<r<<"\n"; return r; }
    Item dispenseProduct(VendingMachine& m,int code) override; // declared only
    vector<Coin> refundFullMoney(VendingMachine&) override { return {}; }
};

// ------------------ Transition Implementations ------------------
void IdleState::clickOnInsertCoinButton(VendingMachine& m) {
    cout<<"Insert coin button clicked. Moving to HasMoneyState.\n";
    m.setVendingMachineState(new HasMoneyState());
}

void HasMoneyState::clickOnStartProductSelectionButton(VendingMachine& m) {
    cout<<"Moving to SelectionState\n";
    m.setVendingMachineState(new SelectionState());
}

void SelectionState::chooseProduct(VendingMachine& m,int code) {
    cout<<"Product chosen with code "<<code<<"\n";
    m.setVendingMachineState(new DispenseState());
}

Item DispenseState::dispenseProduct(VendingMachine& m,int code) {
    cout<<"Dispensed product with code "<<code<<"\n";
    m.resetCoins();
    m.setVendingMachineState(new IdleState());
    return Item(ItemType::COKE,25);
}

// ------------------ Main Demo ------------------
int main(){
    VendingMachine vm;
    vm.setVendingMachineState(new IdleState());

    vm.getState()->clickOnInsertCoinButton(vm);
    vm.getState()->insertCoin(vm,Coin::QUARTER);
    vm.getState()->clickOnStartProductSelectionButton(vm);
    vm.getState()->chooseProduct(vm,101);
    Item item=vm.getState()->dispenseProduct(vm,101);

    cout<<"Got item with price: "<<item.getPrice()<<endl;
    return 0;
}
