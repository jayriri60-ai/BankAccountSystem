#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <QString>

class BankAccount
{
private:
    QString depositorName;
    int accountNumber;
    QString accountType;  // "Savings" ou "Current"
    double balance;

public:
    // Constructeur par défaut
    BankAccount();

    // Initialiser/Créer les détails du compte
    void createAccount(QString name, int accNum, QString type, double initialBalance);

    // Fonctions de transaction
    bool deposit(double amount);
    bool withdraw(double amount);

    // Getters
    QString getDepositorName() const;
    int getAccountNumber() const;
    QString getAccountType() const;
    double getBalance() const;
    QString getBalanceString() const;  // Formate le solde en texte

    // Fonction d'affichage
    QString getAccountDetails() const;  // Retourne les détails formatés
};

#endif // BANKACCOUNT_H