#include <bits/stdc++.h>
using namespace std;

class User;
class Split;

class Balance {
public:
    Balance() : amountOwe(0), amountGetBack(0) {}
    Balance(double amountOwe, double amountGetBack) : amountOwe(amountOwe), amountGetBack(amountGetBack) {}
    double getAmountOwe() { return amountOwe; }
    double getAmountGetBack() { return amountGetBack; }

private:
    double amountOwe;
    double amountGetBack;
};

class UserExpenseBalanceSheet {
public:
    UserExpenseBalanceSheet() : totalExpense(0), totalPaid(0), totalOwed(0), totalGetBack(0) {}

    unordered_map<string, Balance>& getBalanceSheet() { return balanceSheet; }
    double getTotalExpense() { return totalExpense; }
    double getTotalPaid() { return totalPaid; }
    double getTotalOwed() { return totalOwed; }
    double getTotalGetBack() { return totalGetBack; }

    void setBalanceSheet(unordered_map<string, Balance> balanceSheet) { this->balanceSheet = balanceSheet; }
    void setTotalExpense(double totalExpense) { this->totalExpense = totalExpense; }
    void setTotalPaid(double totalPaid) { this->totalPaid = totalPaid; }
    void setTotalOwed(double totalOwed) { this->totalOwed = totalOwed; }
    void setTotalGetBack(double totalGetBack) { this->totalGetBack = totalGetBack; }

private:
    unordered_map<string, Balance> balanceSheet;
    double totalExpense;
    double totalPaid;
    double totalOwed;
    double totalGetBack;
};

class User {
public:
    User(string id, string name) : id(id), name(name) {}

    string getId() { return id; }
    string getName() { return name; }
    UserExpenseBalanceSheet& getBalanceSheet() { return userExpenseBalanceSheet; }

private:
    string id;
    string name;
    UserExpenseBalanceSheet userExpenseBalanceSheet;
};

class UserController {
public:
    void addUser(User user) { users.push_back(user); }

    User& getUser(string id) {
        for (auto& user : users) {
            if (user.getId() == id) return user;
        }
        throw invalid_argument("User not found");
    }

    vector<User>& getAllUsers() { return users; }

private:
    vector<User> users;
};

class Split {
public:
    Split(User& u, double amount) : user(u), amountOwe(amount) {}
    double getAmountOwe() { return amountOwe; }
    User& getUser() { return user; }

private:
    User& user;
    double amountOwe;
};

enum class ExpenseSplitType { EQUAL, UNEQUAL, PERCENT };

class Expense {
public:
    Expense(string id, string description, double expenseAmount, User& paidBy, ExpenseSplitType expenseSplitType, vector<Split> splits)
        : id(id), description(description), expenseAmount(expenseAmount), paidBy(paidBy), expenseSplitType(expenseSplitType), splits(splits) {}

private:
    string id;
    string description;
    double expenseAmount;
    User& paidBy;
    ExpenseSplitType expenseSplitType;
    vector<Split> splits;
};

class ExpenseSplit {
public:
    virtual void validateExpenseSplit(double expenseAmount, vector<Split> splits) = 0;
};

class EqualExpenseSplit : public ExpenseSplit {
public:
    void validateExpenseSplit(double expenseAmount, vector<Split> splits) override {
        double totalAmount = 0;
        double amountPerUser = expenseAmount / splits.size();
        for (auto split : splits) {
            if (split.getAmountOwe() != amountPerUser) {
                throw invalid_argument("Amount owed by each user is not equal");
            }
            totalAmount += split.getAmountOwe();
        }
        if (totalAmount != expenseAmount) {
            throw invalid_argument("Total amount of splits does not match the expense amount");
        }
    }
};

class UnequalExpenseSplit : public ExpenseSplit {
public:
    void validateExpenseSplit(double expenseAmount, vector<Split> splits) override {
        double totalAmount = 0;
        for (auto split : splits) totalAmount += split.getAmountOwe();
        if (totalAmount != expenseAmount) throw invalid_argument("Total amount of splits does not match the expense amount");
    }
};

class PercentExpenseSplit : public ExpenseSplit {
public:
    void validateExpenseSplit(double expenseAmount, vector<Split> splits) override {
        double totalPercentage = 0;
        for (auto split : splits) totalPercentage += split.getAmountOwe();
        if (totalPercentage != 100) throw invalid_argument("Total percentage of splits does not equal 100%");
    }
};

class SplitFactory {
public:
    static ExpenseSplit* getExpenseSplit(ExpenseSplitType expenseSplitType) {
        switch (expenseSplitType) {
        case ExpenseSplitType::EQUAL: return new EqualExpenseSplit();
        case ExpenseSplitType::UNEQUAL: return new UnequalExpenseSplit();
        case ExpenseSplitType::PERCENT: return new PercentExpenseSplit();
        default: throw invalid_argument("Invalid expense split type");
        }
    }
};

class BalanceSheetController {
public:
    void updateUserExpenseBalanceSheet(User& paidBy, vector<Split>& splits, double expenseAmount) {
        UserExpenseBalanceSheet& paidByUserBalanceSheet = paidBy.getBalanceSheet();
        paidByUserBalanceSheet.setTotalExpense(paidByUserBalanceSheet.getTotalExpense() + expenseAmount);

        for (auto& split : splits) {
            User& userOwe = split.getUser();
            UserExpenseBalanceSheet& userOweBalanceSheet = userOwe.getBalanceSheet();
            double amountOwe = split.getAmountOwe();

            if (userOwe.getId() != paidBy.getId()) {
                paidByUserBalanceSheet.setTotalGetBack(paidByUserBalanceSheet.getTotalGetBack() + amountOwe);

                Balance& oweBalance = paidByUserBalanceSheet.getBalanceSheet()[userOwe.getId()];
                oweBalance = Balance(oweBalance.getAmountOwe(), oweBalance.getAmountGetBack() + amountOwe);

                userOweBalanceSheet.setTotalOwed(userOweBalanceSheet.getTotalOwed() + amountOwe);
                userOweBalanceSheet.setTotalExpense(userOweBalanceSheet.getTotalExpense() + amountOwe);

                Balance& userPaidBalance = userOweBalanceSheet.getBalanceSheet()[paidBy.getId()];
                userPaidBalance = Balance(userPaidBalance.getAmountOwe() + amountOwe, userPaidBalance.getAmountGetBack());
            }
        }
    }

    void displayUserBalanceSheet(User& user) {
        cout << "Balance Sheet for User: " << user.getName() << endl;
        UserExpenseBalanceSheet& userBalanceSheet = user.getBalanceSheet();
        cout << "Total Expense: " << userBalanceSheet.getTotalExpense() << endl;
        cout << "Total Paid: " << userBalanceSheet.getTotalPaid() << endl;
        cout << "Total Owed: " << userBalanceSheet.getTotalOwed() << endl;
        cout << "Total Get Back: " << userBalanceSheet.getTotalGetBack() << endl;
        cout << "Balance Sheet Details: " << endl;
        for (auto& balanceEntry : userBalanceSheet.getBalanceSheet()) {
            cout << "User ID: " << balanceEntry.first << ", Amount Owe: " << balanceEntry.second.getAmountOwe()
                 << ", Amount Get Back: " << balanceEntry.second.getAmountGetBack() << endl;
        }
    }
};

class ExpenseController {
public:
    ExpenseController(BalanceSheetController& balanceSheetController) : balanceSheetController(balanceSheetController) {}
    Expense createExpense(string id, string description, double expenseAmount, User& paidBy, ExpenseSplitType expenseSplitType, vector<Split> splits) {
        ExpenseSplit* expenseSplit = SplitFactory::getExpenseSplit(expenseSplitType);
        expenseSplit->validateExpenseSplit(expenseAmount, splits);
        Expense expense(id, description, expenseAmount, paidBy, expenseSplitType, splits);
        balanceSheetController.updateUserExpenseBalanceSheet(paidBy, splits, expenseAmount);
        return expense;
    }

private:
    BalanceSheetController& balanceSheetController;
};

class Group {
public:
    Group(string id, string name, BalanceSheetController& bsc) : id(id), name(name), expenseController(bsc) {}

    void addUser(User& user) { users.push_back(&user); }
    Expense createExpense(string id, string description, double expenseAmount, User& paidBy, ExpenseSplitType expenseSplitType, vector<Split> splits) {
        Expense expense = expenseController.createExpense(id, description, expenseAmount, paidBy, expenseSplitType, splits);
        expenses.push_back(expense);
        return expense;
    }
    vector<User*> getUsers() { return users; }
    vector<Expense> getExpenses() {
                return expenses;
    }
    string getId() { return id; }

private:
    string id;
    string name;
    vector<User*> users;
    vector<Expense> expenses;
    ExpenseController expenseController;
};

class GroupController {
public:
    GroupController(BalanceSheetController& bsc) : balanceSheetController(bsc) {}

    void createGroup(string id, string name, User& createdBy) {
        Group group(id, name, balanceSheetController);
        group.addUser(createdBy);
        groups.push_back(group);
    }

    Group& getGroup(string id) {
        for (auto& group : groups) {
            if (group.getId() == id) return group;
        }
        throw invalid_argument("Group not found");
    }

    vector<Group>& getAllGroups() { return groups; }

private:
    vector<Group> groups;
    BalanceSheetController& balanceSheetController;
};

class Splitwise {
public:
    Splitwise() : userController(), balanceSheetController(), groupController(balanceSheetController) {}

    void demo() {
        setupUsersAndGroups();
        Group& group = groupController.getGroup("1");
        group.addUser(userController.getUser("2"));
        group.addUser(userController.getUser("3"));

        vector<Split> splits;
        splits.emplace_back(userController.getUser("1"), 100);
        splits.emplace_back(userController.getUser("2"), 100);
        splits.emplace_back(userController.getUser("3"), 100);
        group.createExpense("1", "Hotel", 300, userController.getUser("1"), ExpenseSplitType::EQUAL, splits);

        vector<Split> splits2;
        splits2.emplace_back(userController.getUser("1"), 400);
        splits2.emplace_back(userController.getUser("2"), 200);
        group.createExpense("2", "Dinner", 600, userController.getUser("2"), ExpenseSplitType::UNEQUAL, splits2);

        for (auto user : group.getUsers()) {
            balanceSheetController.displayUserBalanceSheet(*user);
        }
    }

    void setupUsersAndGroups() {
        addUsersToSplitwise();
        User& user1 = userController.getUser("1");
        groupController.createGroup("1", "Trip to Paris", user1);
    }

    void addUsersToSplitwise() {
        User user1("1", "Alice");
        User user2("2", "Bob");
        User user3("3", "Charlie");
        userController.addUser(user1);
        userController.addUser(user2);
        userController.addUser(user3);
    }

private:
    UserController userController;
    BalanceSheetController balanceSheetController;
    GroupController groupController;
};

int main() {
    Splitwise splitwise;
    splitwise.demo();
    return 0;
}
