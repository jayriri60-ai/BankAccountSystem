#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountdialog.h"

#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentAccountIndex(-1)
{
    ui->setupUi(this);
    setWindowTitle("Système de Gestion de Comptes Bancaires");

    // ------------------------------------------------------------------
    // CONNEXION DES SIGNAUX ET SLOTS
    // ------------------------------------------------------------------
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::onCreateAccount);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchAccount);
    connect(ui->depositButton, &QPushButton::clicked, this, &MainWindow::onDeposit);
    connect(ui->withdrawButton, &QPushButton::clicked, this, &MainWindow::onWithdraw);
    connect(ui->checkBalanceButton, &QPushButton::clicked, this, &MainWindow::onCheckBalance);
    connect(ui->displayButton, &QPushButton::clicked, this, &MainWindow::onDisplayDetails);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::onExit);

    // Valider le dépôt directement avec la touche "Entrée" dans le champ montant
    connect(ui->amountLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onDeposit);

    statusBar()->showMessage("Bienvenue ! Veuillez créer ou rechercher un compte pour commencer.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Recherche de l'index d'un compte dans la liste via son numéro
int MainWindow::findAccountIndex(int accNum) const
{
    for (int i = 0; i < accountList.size(); ++i) {
        if (accountList[i].getAccountNumber() == accNum) {
            return i;
        }
    }
    return -1;
}

// 1. Création d'un nouveau compte
void MainWindow::onCreateAccount()
{
    CreateAccountDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        BankAccount newAccount = dialog.getCreatedAccount();

        // Contrôle des doublons
        if (findAccountIndex(newAccount.getAccountNumber()) != -1) {
            QMessageBox::warning(this, "Erreur", "Un compte avec ce numéro existe déjà !");
            return;
        }

        accountList.append(newAccount);
        currentAccountIndex = accountList.size() - 1;

        updateAccountDisplay();
        statusBar()->showMessage("Nouveau compte créé avec succès !");
    }
}

// 2. Recherche d'un compte
void MainWindow::onSearchAccount()
{
    QString input = ui->searchAccLineEdit->text().trimmed();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un numéro de compte à chercher.");
        return;
    }

    bool ok = false;
    int accNum = input.toInt(&ok);
    if (!ok || accNum <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de compte valide !");
        return;
    }

    int index = findAccountIndex(accNum);
    if (index != -1) {
        currentAccountIndex = index;
        updateAccountDisplay();
        statusBar()->showMessage("Compte N° " + QString::number(accNum) + " sélectionné.");
    } else {
        QMessageBox::critical(this, "Introuvable", "Aucun compte ne porte ce numéro !");
        statusBar()->showMessage("Erreur : Compte non trouvé.");
    }
}

// 3. Effectuer un dépôt
void MainWindow::onDeposit()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord créer ou rechercher un compte.");
        return;
    }

    QString text = ui->amountLineEdit->text().trimmed();
    bool ok = false;
    double amount = text.toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un montant numérique valide supérieur à 0.");
        return;
    }

    if (accountList[currentAccountIndex].deposit(amount)) {
        updateAccountDisplay();
        QMessageBox::information(this, "Dépôt réussi",
                                 QString("Dépôt de %1 € effectué !\nNouveau solde : %2")
                                     .arg(amount)
                                     .arg(accountList[currentAccountIndex].getBalanceString()));

        ui->amountLineEdit->clear();
        statusBar()->showMessage("Dépôt de " + QString::number(amount) + " € effectué.");
    }
}

// 4. Effectuer un retrait
void MainWindow::onWithdraw()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord créer ou rechercher un compte.");
        return;
    }

    QString text = ui->amountLineEdit->text().trimmed();
    bool ok = false;
    double amount = text.toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un montant numérique valide supérieur à 0.");
        return;
    }

    if (accountList[currentAccountIndex].withdraw(amount)) {
        updateAccountDisplay();
        QMessageBox::information(this, "Retrait réussi",
                                 QString("Retrait de %1 € effectué !\nNouveau solde : %2")
                                     .arg(amount)
                                     .arg(accountList[currentAccountIndex].getBalanceString()));

        ui->amountLineEdit->clear();
        statusBar()->showMessage("Retrait de " + QString::number(amount) + " € effectué.");
    } else {
        QMessageBox::critical(this, "Échec du retrait", "Solde insuffisant pour effectuer ce retrait !");
        statusBar()->showMessage("Échec : Solde insuffisant.");
    }
}

// 5. Afficher le solde actuel
void MainWindow::onCheckBalance()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Aucun compte sélectionné.");
        return;
    }

    QString msg = QString("Titulaire : %1\nSolde actuel : %2")
                      .arg(accountList[currentAccountIndex].getDepositorName())
                      .arg(accountList[currentAccountIndex].getBalanceString());

    QMessageBox::information(this, "Consultation du Solde", msg);
    statusBar()->showMessage("Solde consulté.");
}

// 6. Réafficher les détails du compte
void MainWindow::onDisplayDetails()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Aucun compte sélectionné.");
        return;
    }

    updateAccountDisplay();
    statusBar()->showMessage("Détails du compte affichés.");
}

// 7. Réinitialiser la vue
void MainWindow::onReset()
{
    ui->searchAccLineEdit->clear();
    ui->amountLineEdit->clear();
    ui->detailsTextEdit->clear();
    currentAccountIndex = -1;
    statusBar()->showMessage("Interface réinitialisée.");
}

// 8. Quitter l'application
void MainWindow::onExit()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Êtes-vous sûr de vouloir quitter l'application ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }
}

// Mise à jour du texte des détails
void MainWindow::updateAccountDisplay()
{
    if (currentAccountIndex != -1) {
        ui->detailsTextEdit->setText(accountList[currentAccountIndex].getAccountDetails());
    } else {
        ui->detailsTextEdit->clear();
    }
}