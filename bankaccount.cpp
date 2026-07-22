#include "bankaccount.h"

// Constructeur par défaut
BankAccount::BankAccount()
    : depositorName(""), accountNumber(0), accountType(""), balance(0.0), pinCode("0000")
{
}

// Configuration des détails du compte
void BankAccount::createAccount(QString name, int accNum, QString type, double initialBalance, QString pin)
{
    depositorName = name;
    accountNumber = accNum;
    accountType = type;
    balance = (initialBalance >= 0.0) ? initialBalance : 0.0;
    pinCode = pin.isEmpty() ? "0000" : pin;

    if (initialBalance > 0) {
        addTransaction("Dépôt Initial", initialBalance);
    }
}

// Ajoute une ligne à l'historique des transactions
void BankAccount::addTransaction(const QString &type, double amount)
{
    Transaction t;
    t.timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    t.type = type;
    t.amount = amount;
    t.newBalance = balance;
    transactionHistory.append(t);
}

// Effectuer un dépôt
bool BankAccount::deposit(double amount)
{
    if (amount > 0) {
        balance += amount;
        addTransaction("Dépôt", amount);
        return true;
    }
    return false;
}

// Effectuer un retrait
bool BankAccount::withdraw(double amount)
{
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        addTransaction("Retrait", amount);
        return true;
    }
    return false;
}

// Application d'intérêts sur le compte d'épargne
bool BankAccount::applyInterest(double ratePercent)
{
    if (accountType.contains("Savings", Qt::CaseInsensitive) ||
        accountType.contains("Épargne", Qt::CaseInsensitive)) {
        if (ratePercent > 0) {
            double interest = balance * (ratePercent / 100.0);
            balance += interest;
            addTransaction(QString("Intérêts (+%1%)").arg(ratePercent), interest);
            return true;
        }
    }
    return false;
}

// Vérification du code PIN
bool BankAccount::verifyPin(const QString &inputPin) const
{
    return pinCode == inputPin;
}

void BankAccount::setPin(const QString &newPin)
{
    pinCode = newPin;
}

QString BankAccount::getPin() const
{
    return pinCode;
}

// Getters
QString BankAccount::getDepositorName() const { return depositorName; }
int BankAccount::getAccountNumber() const { return accountNumber; }
QString BankAccount::getAccountType() const { return accountType; }
double BankAccount::getBalance() const { return balance; }

// Formate le solde avec deux décimales
QString BankAccount::getBalanceString() const
{
    return QString::number(balance, 'f', 2) + " €";
}

QList<Transaction> BankAccount::getTransactionHistory() const
{
    return transactionHistory;
}

// Génère une présentation claire et alignée du compte
QString BankAccount::getAccountDetails() const
{
    return QString("--- Détails du compte ---\n"
                   "Titulaire : %1\n"
                   "Numéro de compte : %2\n"
                   "Type de compte : %3\n"
                   "Solde actuel : %4")
        .arg(depositorName)
        .arg(accountNumber)
        .arg(accountType)
        .arg(getBalanceString());
}