#include <bits/stdc++.h>
using namespace std;

class Collegue{
public:
    virtual ~Collegue() = default;
    virtual void placeBid(int amount) = 0;
    virtual void receiveNotification(int amount) = 0;
    virtual string getName() = 0;
};

class AuctionMediator{
public:
    virtual ~AuctionMediator() = default;
    virtual void addBidder(Collegue* bidder) = 0;
    virtual void placeBid(Collegue* bidder , int amount) = 0;
};

class Auction : public AuctionMediator{
    private:
        vector<Collegue* > bidders;
    public:
        Auction() = default;
        void addBidder(Collegue* bidder){
            bidders.push_back(bidder);
        }
        void placeBid(Collegue* bidder, int amount){
            for(auto &bid : bidders){
                if(bid!=bidder){
                    bid->receiveNotification(amount);
                }
            }
        }
};

class Bidder : public Collegue{
    private:
        string name;
        AuctionMediator* mediator;
    public:
        Bidder(string name, AuctionMediator* mediator) : name(name), mediator(mediator) {
            mediator->addBidder(this);
        }
        void placeBid(int amount){
            mediator->placeBid(this,amount);
        }
        void receiveNotification(int amount){
            cout<<"Bidder "<< name << " receive a notification of a bid place of " << amount<< endl;
        }
        string getName() override{
            return this->name;
        }
};

int main(){
    AuctionMediator *am = new Auction();
    Collegue* b1 = new Bidder("bidder1", am);
    Collegue* b2 = new Bidder("bidder2", am);
    Collegue* b3 = new Bidder("bidder3", am);

    b3->placeBid(50);
    return 0;
}