#include <bits/stdc++.h>
using namespace std;

class Payment{
    public:
        void makePayment(){
            //IMPL
        }
};

class ProductDao{
    public:
        void saveProduct(){
            //IMPL
        }
};

class InvoiceDao{
    public:
        void saveInvoice(){
            //IMPL
        }
};

class sendNotification{
    public:
        void sendEmail(){
            //IMPL
        }
        void sendSMS(){
            //IMPL
        }
};

class OrderFacade{
    private:
        Payment payment;
        ProductDao productDao;
        InvoiceDao invoiceDao;
        sendNotification notification;
    public:
        void placeOrder(){
            payment.makePayment();
            productDao.saveProduct();
            invoiceDao.saveInvoice();
            notification.sendEmail();
            notification.sendSMS();
        }
};

class Client{
    public:
        void placeOrder(){
            OrderFacade orderFacade;
            orderFacade.placeOrder();
        }
};

int main(){
    Client client;
    client.placeOrder();
    return 0;
}