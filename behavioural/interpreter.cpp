#include <bits/stdc++.h>
using namespace std;

class Context{
    private:
        unordered_map<string,int> contextMap;
    public:
        void setValue(string s, int i){
            contextMap[s] = i;
        }
        int get(string s){
            return contextMap[s];
        }
};

class AbstractExpression{
public:
    virtual ~AbstractExpression() = default;
    virtual int interpret(Context* context) = 0;
};

class NumberTerminalExpression : public AbstractExpression{
    private:
        string stringValue;
    public:
        NumberTerminalExpression(string s) : stringValue(s) {}
        int interpret(Context* context) override{
            return context->get(stringValue);
        }
};

class MultiplyNonTerminalExpression : public AbstractExpression{
    private:
        AbstractExpression* left;
        AbstractExpression* right;
    public:
        MultiplyNonTerminalExpression(AbstractExpression* l, AbstractExpression* r) : left(l), right(r) {}
        int interpret(Context* context){
            return (left->interpret(context) * right->interpret(context));
        }
};

int main(){
    Context* c = new Context();
    c->setValue("a",5);
    c->setValue("b",2);
    AbstractExpression* expression = new MultiplyNonTerminalExpression(new NumberTerminalExpression("a"), new MultiplyNonTerminalExpression( 
                                        new NumberTerminalExpression("b"), new NumberTerminalExpression("a")));
    cout<< expression->interpret(c)<<endl;
    return 0;
}