#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "bankaccount.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots connectés aux événements des boutons
    void onCreateAccount();
    void onSearchAccount();
    void onDeposit();
    void onWithdraw();
    void onCheckBalance();
    void onDisplayDetails();
    void onReset();
    void onExit();

private:
    Ui::MainWindow *ui;

    // Liste pour gérer tous les comptes enregistrés
    QList<BankAccount> accountList;

    // Indice du compte sélectionné (-1 si aucun)
    int currentAccountIndex;

    // Fonctions utilitaires
    int findAccountIndex(int accNum) const;
    void updateAccountDisplay();
};

#endif // MAINWINDOW_H