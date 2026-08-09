#include <bits/stdc++.h>

using namespace std;

enum class TransactionType{
    CASH_WITHDRAWAL,
    BALANCE_CHECK
};

void showAllTransactions() {
    std::cout << "Available Transaction Types:\n";
    std::cout << "- Cash Withdrawal\n";
    std::cout << "- Balance Check\n";
}

class UserBankAccount{
    private:
        int balance;
    public:
        UserBankAccount(int initialBalance = 0) : balance(initialBalance) {}

        void withdrawalBalance(int amount){
            balance = balance - amount;
        }
        void depositBalance(int amount){
            balance += amount;
        }
        void setBalance(int amount){
            balance = amount;
        }
        int getBalance() const{
            return balance;
        }
};

class Card{
    private: 
        int cardNumber;
        int cvv;
        int expirydate;
        int holderName;
        static const int PIN_NUMBER = 112211;
        UserBankAccount bankAccount;
    public:
        bool isCorrectPINEntered(int pin){
            if(pin == PIN_NUMBER){
                return true;
            }
            return false;
        }
        int getBankBalance() const{
            return bankAccount.getBalance();
        }
        void deductBankBalance(int amount){
            bankAccount.withdrawalBalance(amount);
        }
        void setBankAccount(UserBankAccount ba){
            this->bankAccount = ba;
        }
};

class User{
    private: 
        Card card;
        UserBankAccount bankAccount;
    public:
        Card getCard() const{
            return card;
        }
        void setCard(Card card){
            this->card = card;
        }
};

class ATM;

class AtmState{
    public:
        virtual void insertCard(ATM* atm, Card* card){
            cout<<"Something went wrong"<<endl;
        };
        virtual void authenticatePin(ATM* atm, Card* card, int pin){
            cout<<"Something went wrong"<<endl;
        }
        virtual void selectOperation(ATM* atm, Card* card, TransactionType type){
           cout<<"Something went wrong"<<endl; 
        }
        virtual void cashWithdrawal(ATM* atm, Card* card, int amount){
            cout<<"Something went wrong"<<endl;
        }
        virtual void displayBalance(ATM* atm, Card* card){
            cout<<"Something went wrong"<<endl;
        }
        virtual void returnCard(){
            cout<<"Something went wrong"<<endl;
        }
        virtual void exit(ATM* atm){
            cout<<"Something went wrong"<<endl;
        }
};

class IdleState : public AtmState{
    public:
        IdleState(){
            cout<<"ATM is in idle state"<<endl;
        }
        void insertCard(ATM* atm, Card* card) override;
};

class HasCard : public AtmState{
    public:
        HasCard(){
            cout<<"enter your card pin number"<<endl;
        }
        void authenticatePin(ATM* atm, Card* card, int pin) override;
        void exit(ATM* atm) override;
        void returnCard() override{
            cout<<"Please collect your card"<<endl;
        }
};

class SelectOperationState : public AtmState{
    public:
        SelectOperationState(){
            showOperations();
        }
        void selectOperation(ATM* atm, Card* card, TransactionType type) override;
        void exit(ATM* atm) override;
        void returnCard() override{
            cout<<"Please collect your card"<<endl;
        }
    private:
        void showOperations(){
            cout<<"Select the operations"<<endl;
            showAllTransactions();
        }
};

class CashWithdrawalState : public AtmState{
    public:
        CashWithdrawalState(){
            cout<<"Enter the withdrawal amount"<<endl;
        }
        void cashWithdrawal(ATM* atm, Card* card, int amount) override;
        void exit (ATM* atm) override;
        void returnCard() override{
            cout<<"Please collect your card"<<endl;
        }
};

class CheckBalanceState : public AtmState{
    public:
        CheckBalanceState(){
            cout<<"Printing your balance amount"<<endl;
        }

        void displayBalance(ATM* atm, Card* card) override{
            cout<<"Your balance is: "<<card->getBankBalance()<<endl;
            exit(atm);
        }

        void exit(ATM* atm) override;

        void returnCard() override{
            cout<<"Please collect your card"<<endl;
        }
};

class ATM{
    private:
        AtmState* currentATMState;
        int atmBalance;
        int noOfTwoThousandNotes;
        int noOfFiveHundredNotes;
        int noOfOneHundredNotes;
        ATM(){
            cout<<"ATM instance is created"<<endl;
        }
        ATM(const ATM&) = delete;
        ATM& operator=(const ATM&) = delete;
    public :
        static ATM* getATMObject(){
            static ATM atmObject;
            return &atmObject;
        }
        void setCurrentATMState(AtmState* state){
            delete(currentATMState);
            this->currentATMState = state;
        }
        AtmState* getCurrentATMState() const{
            return currentATMState;
        }
        int getAtmBalance(){
            return this->atmBalance;
        }
        void setAtmBalance(int atmBalance, int no2k, int no5, int no100){
            this->atmBalance = atmBalance;
            this->noOfFiveHundredNotes = no5;
            this->noOfOneHundredNotes = no100;
        }
        int getNoOfTwoThousandNotes() {
            return noOfTwoThousandNotes;
        }
        int getNoOfFiveHundredNotes() {
            return noOfFiveHundredNotes;
        }
        int getNoOfOneHundredNotes() {
            return noOfOneHundredNotes;
        }
        void deductATMBalance(int amount) {
            atmBalance = atmBalance - amount;
        }
        void deductTwoThousandNotes(int number) {
            noOfTwoThousandNotes = noOfTwoThousandNotes - number;
        }
        void deductFiveHundredNotes(int number) {
            noOfFiveHundredNotes = noOfFiveHundredNotes - number;
        }
        void deductOneHundredNotes(int number) {
            noOfOneHundredNotes = noOfOneHundredNotes - number;
        }
        void printCurrentATMStatus(){
            cout<<"Balance : " << atmBalance <<endl;
            cout<<"2kNotes : " << noOfTwoThousandNotes <<endl;
            cout<<"500Notes : " <<noOfFiveHundredNotes <<endl;
            cout<<"100Notes : " << noOfOneHundredNotes <<endl;
        }

};

void IdleState::insertCard(ATM* atm, Card* card){
    cout<<"Card is inserted"<<endl;
    atm->setCurrentATMState(new HasCard());
}

void HasCard::authenticatePin(ATM* atm, Card* card, int pin){
    if(card->isCorrectPINEntered(pin)){
        cout<<"Correct Pin"<<endl;
        atm->setCurrentATMState(new SelectOperationState());
    }
    else{
        cout<<"Entered pin is not correct"<<endl;
        exit(atm);
    }
}

void HasCard::exit(ATM* atm){
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

void SelectOperationState::selectOperation(ATM* atm, Card* card, TransactionType type){
    if(type == TransactionType::BALANCE_CHECK){
        cout<<"checking balance"<<endl;
        atm->setCurrentATMState(new CheckBalanceState());
    }
    else if(type == TransactionType::CASH_WITHDRAWAL){
        cout<<"Withdrawing cash"<<endl;
        atm->setCurrentATMState(new CashWithdrawalState());
    }
    else{
        cout<<"Invalid option"<<endl;
        exit(atm);
    }
}

void SelectOperationState::exit(ATM* atm){
    returnCard();
    atm->setCurrentATMState(new IdleState());
}



void CashWithdrawalState::exit (ATM* atm){
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

void CheckBalanceState::exit(ATM* atm){
    returnCard();
    atm->setCurrentATMState(new IdleState());
}

class CashWithdrawalProcessor{
    public:
        CashWithdrawalProcessor(CashWithdrawalProcessor* next){
            this->nextProcessor = next;
        }
        virtual ~CashWithdrawalProcessor() = default;

        virtual void withdraw(ATM* atm, int amount){
            if(nextProcessor!=nullptr){
                nextProcessor->withdraw(atm,amount);
            }
            else{
                cout<<"Cannot process the request";
            }
        }
    private:
        CashWithdrawalProcessor* nextProcessor;
};

class TwoThousandProcessor : public CashWithdrawalProcessor{
    public:
        TwoThousandProcessor(CashWithdrawalProcessor* processor) : CashWithdrawalProcessor(processor) {}
        void withdraw(ATM* atm, int amount) override {
            int notes = amount/2000;
            int remAmount = amount%2000;
            int notesInATM = atm->getNoOfTwoThousandNotes();
            if(notes<=notesInATM){
                if(notes>0)
                    atm->deductTwoThousandNotes(notes);
            }
            else if(notes>notesInATM){
                atm->deductTwoThousandNotes(notesInATM);
                remAmount+= (notes-notesInATM)*2000;
            }
            if(remAmount!=0){
                CashWithdrawalProcessor::withdraw(atm,remAmount);
            }
        }
};

class FiveHundredWithdrawProcessor : public CashWithdrawalProcessor{
    public:
        FiveHundredWithdrawProcessor(CashWithdrawalProcessor* processor) : CashWithdrawalProcessor(processor) {}
        void withdraw(ATM* atm, int amount) override {
            int notes = amount/500;
            int remAmount = amount%500;
            int notesInATM = atm->getNoOfFiveHundredNotes();
            if(notes<=notesInATM){
                atm->deductFiveHundredNotes(notes);
            }
            else if(notes>notesInATM){
                atm->deductFiveHundredNotes(notesInATM);
                remAmount+= (notes-notesInATM)*500;
            }
            if(remAmount!=0){
                CashWithdrawalProcessor::withdraw(atm,remAmount);
            }
        }
};

class OneHundredWithdrawProcessor : public CashWithdrawalProcessor{
    public:
        OneHundredWithdrawProcessor(CashWithdrawalProcessor* processor) : CashWithdrawalProcessor(processor) {}
        void withdraw(ATM* atm, int amount) override {
            int notes = amount/100;
            int remAmount = amount%100;
            int notesInATM = atm->getNoOfOneHundredNotes();
            if(notes<=notesInATM){
                atm->deductOneHundredNotes(notes);
            }
            else if(notes>notesInATM){
                atm->deductOneHundredNotes(notesInATM);
                remAmount+= (notes-notesInATM)*100;
            }
            if(remAmount!=0){
                CashWithdrawalProcessor::withdraw(atm,remAmount);
            }
        }
};

void CashWithdrawalState::cashWithdrawal(ATM* atm, Card* card, int amount){
    if(atm->getAtmBalance()<amount){
        cout<<"ATM balance insufficient"<<endl;
        exit(atm);
    }
    else if(card->getBankBalance()<amount){
        cout<<"Bank balance insufficient"<<endl;
        exit(atm);
    }
    else{
        card->deductBankBalance(amount);
        atm->deductATMBalance(amount);
        CashWithdrawalProcessor* cashWithdrawalProcessor = new TwoThousandProcessor(new FiveHundredWithdrawProcessor(new OneHundredWithdrawProcessor(nullptr)));
        cashWithdrawalProcessor->withdraw(atm,amount);
        exit(atm);
    }  
}

class ATMRoom {
private:
    ATM* atm;
    User user;
public:
    ATMRoom() {
        atm = ATM::getATMObject();
    }

    void initialize() {
        // Initialize ATM with some cash
        atm->setAtmBalance(10000, 2, 5, 10); // total, 2x2000, 5x500, 10x100

        // Create a bank account with balance
        UserBankAccount account(5000); // set balance to 5000 

        // Create card and link account
        Card card;
        card.setBankAccount(account);

        // Assign card to user
        user.setCard(card);

        // Set ATM initial state
        atm->setCurrentATMState(new IdleState());
    }

    ATM* getATM() { return atm; }
    User getUser() const { return user; }
};

int main() {
    ATMRoom room;
    room.initialize();

    ATM* atm = room.getATM();
    Card card = room.getUser().getCard();

    // Happy path: Insert card → Authenticate → Cash withdrawal → Exit
    atm->getCurrentATMState()->insertCard(atm, &card);
    atm->getCurrentATMState()->authenticatePin(atm, &card, 112211);
    atm->getCurrentATMState()->selectOperation(atm, &card, TransactionType::CASH_WITHDRAWAL);
    atm->getCurrentATMState()->cashWithdrawal(atm, &card, 1700); // withdraw 1700

    // Print ATM status at the end
    atm->printCurrentATMStatus();

    return 0;
}

