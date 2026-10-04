#include <bits/stdc++.h>
using namespace std;

enum class LogLevel
{
    VERBOSE,
    ERROR,
    DEBUG
};

class Logging
{
protected:
    unique_ptr<Logging> nextHandler;
    virtual void write(const string &s) = 0;
    virtual bool canHandle(LogLevel level) = 0;

public:
    void setNext(unique_ptr<Logging> next)
    {
        nextHandler = std::move(next);
    }
    void log(LogLevel level, const string &s)
    {
        if (canHandle(level))
        {
            write(s);
        }
        else
        {
            nextHandler->log(level, s);
        }
    }
};

class VerboseLogging : public Logging
{
private:
    void write(const string &s) override
    {
        cout << "[VERBOSE] " << s << endl;
    }
    bool canHandle(LogLevel level)
    {
        if (level == LogLevel::VERBOSE)
            return true;
        return false;
    }
};

class ErrorLogging : public Logging
{
private:
    void write(const string &s) override
    {
        cout << "[ERROR] " << s << endl;
    }
    bool canHandle(LogLevel level)
    {
        if (level == LogLevel::ERROR)
            return true;
        return false;
    }
};

class DebugLogging : public Logging
{
private:
    void write(const string &s) override
    {
        cout << "[DEBUG] " << s << endl;
    }
    bool canHandle(LogLevel level)
    {
        if (level == LogLevel::DEBUG)
            return true;
        return false;
    }
};

int main()
{
    auto verboseLog = make_unique<VerboseLogging>();
    auto errorLog = make_unique<ErrorLogging>();
    auto debugLog = make_unique<DebugLogging>();

    errorLog->setNext(std::move(debugLog));
    verboseLog->setNext(std::move(errorLog));
    verboseLog->log(LogLevel::VERBOSE, "This is a verbose message.");
    verboseLog->log(LogLevel::ERROR, "This is an error message.");
    verboseLog->log(LogLevel::DEBUG, "This is a debug message.");
    return 0;
}