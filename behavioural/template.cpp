#include <bits/stdc++.h>
using namespace std;

class PaymentGateway{
public:
    virtual ~PaymentGateway() = default;
    virtual void initiatePayment() = 0;
    virtual void debitMoney() = 0;
    virtual void creditMoney() = 0;
    virtual void validatePayment() = 0;
    virtual void sendMoney() final{
        initiatePayment();
        debitMoney();
        creditMoney();
        validatePayment();
    }
};

class UpiPayment : public PaymentGateway{
    public:
    void initiatePayment() override{
        cout<<"UPI initiate payment\n"; 
    }
    void debitMoney() override{
        cout<<"UPI debit money\n";
    }
    void creditMoney() override{
        cout<<"UPI credit money\n";
    }
    void validatePayment() override{
        cout<<"UPI validate payment\n";
    }
};

class NetBankingPayment : public PaymentGateway{
    public:
    void initiatePayment() override{
        cout<<"NetBanking initiate payment\n"; 
    }
    void debitMoney() override{
        cout<<"NetBanking debit money\n";
    }
    void creditMoney() override{
        cout<<"NetBanking credit money\n";
    }
    void validatePayment() override{
        cout<<"NetBanking validate payment\n";
    }
};

class CreditCardPayment : public PaymentGateway{
    public:
    void initiatePayment() override{
        cout<<"CreditCard initiate payment\n"; 
    }
    void debitMoney() override{
        cout<<"CreditCard debit money\n";
    }
    void creditMoney() override{
        cout<<"CreditCard credit money\n";
    }
    void validatePayment() override{
        cout<<"CreditCard validate payment\n";
    }
};

int main(){
    PaymentGateway* upigateway = new UpiPayment();
    upigateway->sendMoney();
    delete upigateway;
    PaymentGateway* netbankinggateway = new NetBankingPayment();
    netbankinggateway->sendMoney();
    delete netbankinggateway;
    PaymentGateway* creditcardgateway = new CreditCardPayment();
    creditcardgateway->sendMoney();
    delete creditcardgateway;
    return 0;
}