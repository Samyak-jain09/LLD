
classDiagram
    class Splitwise {
        +Splitwise()
        +demo()
        +setupUsersAndGroups()
        +addUsersToSplitwise()
    }

    class UserController {
        +addUser(User user)
        +getUser(string id) User&
        +getAllUsers() vector~User&~
    }

    class GroupController {
        +createGroup(string id, string name, User& createdBy)
        +getGroup(string id) Group&
        +getAllGroups() vector~Group&~
    }

    class BalanceSheetController {
        +updateUserExpenseBalanceSheet(User& paidBy, vector~Split&~ splits, double expenseAmount)
        +displayUserBalanceSheet(User& user)
    }

    class User {
        -string id
        -string name
        -UserExpenseBalanceSheet userExpenseBalanceSheet
        +User(string id, string name)
        +getId() string
        +getName() string
        +getBalanceSheet() UserExpenseBalanceSheet&
    }

    class UserExpenseBalanceSheet {
        -unordered_map~string, Balance~ balanceSheet
        -double totalExpense
        -double totalPaid
        -double totalOwed
        -double totalGetBack
        +getBalanceSheet() unordered_map~string, Balance&~
        +getTotalExpense() double
        +getTotalPaid() double
        +getTotalOwed() double
        +getTotalGetBack() double
        +setBalanceSheet(...)
        +setTotalExpense(double)
        +setTotalPaid(double)
        +setTotalOwed(double)
        +setTotalGetBack(double)
    }

    class Balance {
        -double amountOwe
        -double amountGetBack
        +Balance()
        +Balance(double amountOwe, double amountGetBack)
        +getAmountOwe() double
        +getAmountGetBack() double
    }

    class Group {
        -string id
        -string name
        -vector~User*~ users
        -vector~Expense~ expenses
        -ExpenseController expenseController
        +Group(string id, string name, BalanceSheetController& bsc)
        +addUser(User& user)
        +createExpense(string id, string description, double expenseAmount, User& paidBy, ExpenseSplitType type, vector~Split~ splits) Expense
        +getUsers() vector~User*~
        +getExpenses() vector~Expense~
        +getId() string
    }

    class ExpenseController {
        -BalanceSheetController& balanceSheetController
        +ExpenseController(BalanceSheetController& balanceSheetController)
        +createExpense(string id, string description, double expenseAmount, User& paidBy, ExpenseSplitType type, vector~Split~ splits) Expense
    }

    class Expense {
        -string id
        -string description
        -double expenseAmount
        -User& paidBy
        -ExpenseSplitType expenseSplitType
        -vector~Split~ splits
        +Expense(...)
    }

    class Split {
        -User& user
        -double amountOwe
        +Split(User& u, double amount)
        +getAmountOwe() double
        +getUser() User&
    }

    class ExpenseSplit {
        <<abstract>>
        +validateExpenseSplit(double expenseAmount, vector~Split~ splits)
    }

    class EqualExpenseSplit {
        +validateExpenseSplit(double expenseAmount, vector~Split~ splits)
    }

    class UnequalExpenseSplit {
        +validateExpenseSplit(double expenseAmount, vector~Split~ splits)
    }

    class PercentExpenseSplit {
        +validateExpenseSplit(double expenseAmount, vector~Split~ splits)
    }

    class SplitFactory {
        +getExpenseSplit(ExpenseSplitType expenseSplitType) ExpenseSplit*
    }

    class ExpenseSplitType {
        <<enumeration>>
        EQUAL
        UNEQUAL
        PERCENT
    }

    Splitwise --> UserController
    Splitwise --> GroupController
    Splitwise --> BalanceSheetController

    UserController --> User
    GroupController --> Group
    Group --> User
    Group --> Expense
    Group --> ExpenseController
    ExpenseController --> BalanceSheetController
    ExpenseController --> SplitFactory
    SplitFactory --> ExpenseSplit
    ExpenseSplit <|-- EqualExpenseSplit
    ExpenseSplit <|-- UnequalExpenseSplit
    ExpenseSplit <|-- PercentExpenseSplit

    Expense --> Split
    Expense --> User
    Expense --> ExpenseSplitType
    Split --> User
    User --> UserExpenseBalanceSheet
    UserExpenseBalanceSheet --> Balance
    BalanceSheetController --> User
    BalanceSheetController --> Split

