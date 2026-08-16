#include <bits/stdc++.h>

using namespace std;

class ArithmeticExpression{
    public:
        virtual int evaluate() = 0;
        virtual ~ArithmeticExpression() = default;
};

class Number : public ArithmeticExpression{
    public:
        Number(int val) : value(val) {}
        int evaluate() override{
            return value;
        }
    private:
        int value;
};

enum class Operation{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};

class Expression : public ArithmeticExpression{
    public:
        Expression(ArithmeticExpression* left, ArithmeticExpression* right, Operation op) :
            leftEx(left) , rightEx(right), op(op) {}
        
        int evaluate() override{
            int ans;
            switch(op){
                case Operation::ADD:
                    ans = leftEx->evaluate() + rightEx->evaluate();
                    break;
                case Operation::SUBTRACT:
                    ans = leftEx->evaluate() - rightEx->evaluate();
                    break;
                case Operation::MULTIPLY: 
                    ans = leftEx->evaluate() * rightEx->evaluate();
                    break;
                case Operation::DIVIDE:
                    ans = leftEx->evaluate() / rightEx->evaluate();
                    break;
            }
            return ans;
        }
    private:
        ArithmeticExpression* leftEx;
        ArithmeticExpression* rightEx;
        Operation op;
};

int main(){
    // 2 + (7*5)
    ArithmeticExpression* two = new Number(2);
    ArithmeticExpression* seven = new Number(7);
    ArithmeticExpression* five = new Number(5);
    ArithmeticExpression* innerExpr = new Expression(seven, five, Operation::MULTIPLY);
    ArithmeticExpression* outerExpr = new Expression(two, innerExpr, Operation::ADD);
    cout<<"Result is "<< outerExpr->evaluate()<<endl;
    return 0;
}