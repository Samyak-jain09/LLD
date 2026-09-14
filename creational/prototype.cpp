#include <bits/stdc++.h>
using namespace std;

class Shape {
    public:
        virtual ~Shape() = default;
        virtual std::unique_ptr<Shape> clone() const = 0;
        virtual void draw() = 0;
};

class Rectangle : public Shape{
    private:
        int length;
        int breadth;
    public:
        Rectangle(int len, int bre) : length(len), breadth(bre) {}
        std::unique_ptr<Shape> clone() const{
            return std::make_unique<Rectangle> (*this);
        }
        void draw(){
            cout << "length " << length << " breadth " << breadth << " rectangle drawn\n";
        }
};

class Circle : public Shape{
    private:
        int radius;
    public:
        Circle(int r) : radius(r) {};
        std::unique_ptr<Shape> clone() const{
            return make_unique<Circle> (*this);
        }
        void draw(){
            cout<<"Circle of radius "<< radius << " drawn\n";
        }
};

int main(){
    vector<unique_ptr<Shape>> prototypes;
    prototypes.push_back(make_unique<Rectangle> (1,2));
    prototypes.push_back(make_unique<Circle> (5));
    vector<unique_ptr<Shape>> cloned;
    for(auto &prototype : prototypes){
        auto shape = prototype->clone();
        cloned.push_back(std::move(shape));
    }
    for(auto &clone : cloned){
        clone->draw();
    }
    return 0;
}