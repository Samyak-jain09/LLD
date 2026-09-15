#include <bits/stdc++.h>
using namespace std;

class Momento{
    private:
        int height;
        int width;

    public:
        Momento(int h, int w) : height(h), width(w) {}
        int getHeight() const{
            return this->height;
        }

        int getWidth() const{
            return this->width;
        }
};

class ConfigurationOriginator{
    private:
        int height;
        int width;
    public:
        ConfigurationOriginator(int h, int w) : height(h), width(w) {}
        
        void setHeight(int h){
            this->height = h;
        }

        void setWidth(int w){
            this->width = w;
        }
        int getheight(){
            return this->height;
        }
        int getwidth(){
            return this->width;
        }
        Momento* createMomento(){
            return new Momento(this->height, this->width);
        }

        void restore(Momento* obj){
            this->height = obj->getHeight();
            this->width = obj->getWidth();
        }
};

class CareTaker{
    private:
        vector<Momento* > snapshots;
    public:
        void addSnapshot(Momento* m){
            snapshots.push_back(m);
        }
        Momento* undo(){
            if(snapshots.size() == 0){
                cout<<"No snapshots to undo\n";
                return nullptr;
            }
            Momento* m = snapshots.back();
            snapshots.pop_back();
            return m;
        }
};

int main(){
    ConfigurationOriginator* conf1 = new ConfigurationOriginator(10,12);
    Momento* snapshot = conf1->createMomento();
    CareTaker ck = CareTaker();
    ck.addSnapshot(snapshot);
    conf1->setHeight(2);
    conf1->setWidth(5);
    Momento* snapshot2 = conf1->createMomento();
    ck.addSnapshot(snapshot2);
    conf1->setHeight(5);
    conf1->setWidth(3);
    Momento* restoredSnapshot = ck.undo();
    conf1->restore(restoredSnapshot);
    cout<< "Height and width are "<< conf1->getheight() << " and " << conf1->getwidth() << endl;
    return 0;
}
