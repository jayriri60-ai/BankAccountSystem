#include "bankaccount.h"

// Constructeur par défaut
BankAccount::BankAccount()
    : depositorName(""), accountNumber(0), accountType(""), balance(0.0)
{
}

// Configuration des détails du compte
void BankAccount::createAccount(QString name, int accNum, QString type, double initialBalance)
{
    depositorName = name;
    accountNumber = accNum;
    accountType = type;
    balance = (initialBalance >= 0.0) ? initialBalance : 0.0;
}

// Effectuer un dépôt
bool BankAccount::deposit(double amount)
{
    if (amount > 0) {
        balance += amount;
        return true;
    }
    return false;
}

// Effectuer un retrait
bool BankAccount::withdraw(double amount)
{
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

// Getters
QString BankAccount::getDepositorName() const
{
    return depositorName;
}

int BankAccount::getAccountNumber() const
{
    return accountNumber;
}

QString BankAccount::getAccountType() const
{
    return accountType;
}

double BankAccount::getBalance() const
{
    return balance;
}

// Formate le solde avec deux décimales
QString BankAccount::getBalanceString() const
{
    return QString::number(balance, 'f', 2) + " €";
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