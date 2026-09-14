#include <bits/stdc++.h>
using namespace std;

class SingleRoomElement;
class DoubleRoomElement;
class DeluxeRoomElement;
class RoomVisitor;
class RoomElement{
    public:
        virtual ~RoomElement() = default;
        virtual void accept(RoomVisitor* visitor) = 0;
};

class RoomVisitor{
    public:
        virtual ~RoomVisitor() = default;
        virtual void visit(SingleRoomElement* element) = 0;
        virtual void visit(DoubleRoomElement* element) = 0;
        virtual void visit(DeluxeRoomElement* element) = 0;
};

class SingleRoomElement : public RoomElement{
    private:
        int roomPrice;
    public:
        SingleRoomElement() : roomPrice(0) {}
        void accept(RoomVisitor* visitor) override{
            visitor->visit(this);
        }
        int getPrice() const{
            return this->roomPrice;
        }
        void setPrice(int price){
            this->roomPrice = price;
        }
};

class DeluxeRoomElement : public RoomElement{
    private:
        int roomPrice;
    public:
        DeluxeRoomElement() : roomPrice(0) {}
        void accept(RoomVisitor* visitor) override{
            visitor->visit(this);
        }
        int getPrice() const{
            return this->roomPrice;
        }
        void setPrice(int price){
            this->roomPrice = price;
        }
};

class DoubleRoomElement : public RoomElement{
    private:
        int roomPrice;
    public:
        DoubleRoomElement() : roomPrice(0) {}
        void accept(RoomVisitor* visitor) override{
            visitor->visit(this);
        }
        int getPrice() const{
            return this->roomPrice;
        }
        void setPrice(int price){
            this->roomPrice = price;
        }
};

class RoomPriceVisitor : public RoomVisitor{
    public:
        RoomPriceVisitor() = default;
        void visit(SingleRoomElement* element){
            element->setPrice(element->getPrice()+10); 
        }
        void visit(DoubleRoomElement* element){
            element->setPrice(element->getPrice()+20); 
        }
        void visit(DeluxeRoomElement* element){
            element->setPrice(element->getPrice()+30); 
        }
};

class RoomMaintainenceVisitor : public RoomVisitor{
    public:
        RoomMaintainenceVisitor() = default;
        void visit(SingleRoomElement* element){
            element->setPrice(element->getPrice()+50); 
            cout<<"Maintainence of single room"<<endl;
        }
        void visit(DoubleRoomElement* element){
            element->setPrice(element->getPrice()+60);
            cout<<"Maintainence of double room"<<endl;
        }
        void visit(DeluxeRoomElement* element){
            element->setPrice(element->getPrice()+370);
            cout<<"Maintainence of deluxe room"<<endl;
        }
};

int main(){
    RoomElement* singleRoom = new SingleRoomElement();
    RoomElement* doubleRoom = new DoubleRoomElement();
    RoomElement* deluxeRoom = new DeluxeRoomElement();
    singleRoom->accept(new RoomMaintainenceVisitor());
    doubleRoom->accept(new RoomMaintainenceVisitor());
    return 0;
}

