#include <bits/stdc++.h>
using namespace std;

enum Loglevel{
    INFO,
    DEBUG,
    ERROR
};

class Logger{
    protected:
        std::unique_ptr<Logger> nextLogger;
        virtual bool canHandle(Loglevel level) = 0;
        virtual void write(const string &message) = 0;
    public:
        virtual ~Logger() = default;

        void setNext(std::unique_ptr<Logger> nxt){
            nextLogger = std::move(nxt);
        }

        void log(string const& message, Loglevel level){
            if(canHandle(level)){
                write(message);
            }
            else{
                nextLogger->log(message,level);
            }
        }
};

class InfoLogger: public Logger{
    protected:
        bool canHandle(Loglevel level) override{
            return level == Loglevel::INFO;
        }
        void write(const string& message) override{
            cout<<"[INFO] "<<message<<endl;
        }
};

class DebugLogger: public Logger{
    protected:
        bool canHandle(Loglevel level) override{
            return level == Loglevel::DEBUG;
        }
        void write(const string& message) override{
            cout<<"[DEBUG] "<<message<<endl;
        }
};

class ErrorLogger: public Logger{
    protected:
        bool canHandle(Loglevel level) override{
            return level == Loglevel::ERROR;
        }
        void write(const string& message) override{
            cout<<"[ERROR] "<<message<<endl;
        }
};

int main(){
    auto info = make_unique<InfoLogger>();
    auto debug = make_unique<DebugLogger>();
    auto error = make_unique<ErrorLogger>();

    info->setNext(std::move(debug));
    error->setNext(std::move(info));

    error->log("App created", Loglevel::INFO);
    error->log("App started", DEBUG);
    error->log("App crashed", ERROR);
    
    return 0;
}