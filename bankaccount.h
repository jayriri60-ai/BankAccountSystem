#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>
#include <QList>
#include <QDateTime>

// Structure pour stocker l'historique des transactions
struct Transaction {
    QString timestamp;
    QString type;       // "Dépôt", "Retrait", "Intérêts", etc.
    double amount;
    double newBalance;
};

class BankAccount
{
private:
    QString depositorName;
    int accountNumber;
    QString accountType;
    double balance;
    QString pinCode;                   // 🔑 Code PIN à 4 chiffres
    QList<Transaction> transactionHistory; // 📜 Historique

public:
    BankAccount();

    // Méthode de création (avec 5 arguments, PIN par défaut à "0000")
    void createAccount(QString name, int accNum, QString type, double initialBalance, QString pin = "0000");

    bool deposit(double amount);
    bool withdraw(double amount);
    bool applyInterest(double ratePercent);

    // Authentification par PIN
    bool verifyPin(const QString &inputPin) const;
    void setPin(const QString &newPin);
    QString getPin() const;

    // Getters
    QString getDepositorName() const;
    int getAccountNumber() const;
    QString getAccountType() const;
    double getBalance() const;
    QString getBalanceString() const;

    // Historique des transactions
    QList<Transaction> getTransactionHistory() const;
    void addTransaction(const QString &type, double amount);

    QString getAccountDetails() const;
};

#endif // BANKACCOUNT_H