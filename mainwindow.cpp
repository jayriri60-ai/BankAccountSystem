#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountdialog.h"

#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>

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

    // Les 4 fonctionnalités bonus
    connect(ui->historyButton, &QPushButton::clicked, this, &MainWindow::onShowHistory);
    connect(ui->interestButton, &QPushButton::clicked, this, &MainWindow::onCalculateInterest);
    connect(ui->currencyButton, &QPushButton::clicked, this, &MainWindow::onConvertCurrency);
    connect(ui->printButton, &QPushButton::clicked, this, &MainWindow::onPrintStatement);

    // Valider le dépôt directement avec la touche "Entrée"
    connect(ui->amountLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onDeposit);

    // 💾 Chargement automatique des comptes
    loadAccountsFromFile();

    statusBar()->showMessage(QString("Bienvenue ! %1 compte(s) chargé(s).").arg(accountList.size()));
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

// 🔒 Authentification sécurisée par Code PIN
bool MainWindow::authenticateUser()
{
    if (currentAccountIndex == -1) return false;

    bool ok;
    QString enteredPin = QInputDialog::getText(
        this,
        "Authentification requise",
        "Veuillez entrer le code PIN à 4 chiffres du compte :",
        QLineEdit::Password,
        "",
        &ok
        );

    if (ok && accountList[currentAccountIndex].verifyPin(enteredPin)) {
        return true;
    } else if (ok) {
        QMessageBox::critical(this, "Erreur", "Code PIN incorrect ! Accès refusé.");
    }
    return false;
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

        // 💾 Sauvegarde automatique
        saveAccountsToFile();

        updateAccountDisplay();
        statusBar()->showMessage("Nouveau compte créé et enregistré avec succès !");
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
        // 💾 Sauvegarde des modifications
        saveAccountsToFile();

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
        // 💾 Sauvegarde des modifications
        saveAccountsToFile();

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

// 📜 7. Historique des transactions
void MainWindow::onShowHistory()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un compte.");
        return;
    }

    if (!authenticateUser()) return;

    const BankAccount &acc = accountList[currentAccountIndex];
    QList<Transaction> history = acc.getTransactionHistory();

    if (history.isEmpty()) {
        QMessageBox::information(this, "Historique", "Aucune transaction enregistrée pour ce compte.");
        return;
    }

    QString report = QString("=== Historique des transactions : %1 (N° %2) ===\n\n")
                         .arg(acc.getDepositorName())
                         .arg(acc.getAccountNumber());

    for (const Transaction &t : history) {
        report += QString("[%1] %2 : %3 € (Nouveau solde : %4 €)\n")
                      .arg(t.timestamp)
                      .arg(t.type, -18)
                      .arg(t.amount, 8, 'f', 2)
                      .arg(t.newBalance, 8, 'f', 2);
    }

    ui->detailsTextEdit->setText(report);
    statusBar()->showMessage("Historique des transactions affiché.");
}

// 📈 8. Calculateur d'intérêts (Comptes Épargne)
void MainWindow::onCalculateInterest()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un compte.");
        return;
    }

    BankAccount &acc = accountList[currentAccountIndex];
    if (!acc.getAccountType().contains("Savings", Qt::CaseInsensitive) &&
        !acc.getAccountType().contains("Épargne", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Type de compte", "Le calcul d'intérêts s'applique uniquement aux comptes d'épargne (Savings).");
        return;
    }

    bool ok;
    double rate = QInputDialog::getDouble(this, "Calculateur d'intérêts",
                                          "Taux d'intérêt annuel (%) :", 3.0, 0.1, 20.0, 2, &ok);
    if (ok) {
        if (acc.applyInterest(rate)) {
            saveAccountsToFile();
            updateAccountDisplay();
            QMessageBox::information(this, "Succès", QString("Intérêts de %1% appliqués avec succès !").arg(rate));
            statusBar()->showMessage("Intérêts calculés et ajoutés au solde.");
        }
    }
}

// 💱 9. Convertisseur de devises (EUR, USD, GBP, CAD, BIF, TZS, KES)
void MainWindow::onConvertCurrency()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un compte.");
        return;
    }

    double balanceEUR = accountList[currentAccountIndex].getBalance();

    double usd = balanceEUR * 1.08;
    double gbp = balanceEUR * 0.85;
    double cad = balanceEUR * 1.47;
    double bif = balanceEUR * 3150.0;
    double tzs = balanceEUR * 2850.0;
    double kes = balanceEUR * 140.0;

    QString msg = QString("=== Conversion du Solde (%1) ===\n\n"
                          "💶 EUR : %2 €\n"
                          "💵 USD : %3 $\n"
                          "💷 GBP : %4 £\n"
                          "🇨🇦 CAD : %5 $\n"
                          "🇧🇮 BIF : %6 FBu\n"
                          "🇹🇿 TZS : %7 TSh\n"
                          "🇰🇪 KES : %8 KSh")
                      .arg(accountList[currentAccountIndex].getDepositorName())
                      .arg(balanceEUR, 0, 'f', 2)
                      .arg(usd, 0, 'f', 2)
                      .arg(gbp, 0, 'f', 2)
                      .arg(cad, 0, 'f', 2)
                      .arg(bif, 0, 'f', 0)
                      .arg(tzs, 0, 'f', 0)
                      .arg(kes, 0, 'f', 2);

    QMessageBox::information(this, "Convertisseur de Devises", msg);
}

// 🖨️ 10. Relevé de compte / Export PDF
void MainWindow::onPrintStatement()
{
    if (currentAccountIndex == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un compte.");
        return;
    }

    if (!authenticateUser()) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le relevé en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    const BankAccount &acc = accountList[currentAccountIndex];

    QString html = QString(
                       "<html><head><style>"
                       "body { font-family: Arial; padding: 20px; }"
                       "h1 { color: #0056b3; }"
                       "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                       "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
                       "th { background-color: #f2f2f2; }"
                       "</style></head><body>"
                       "<h1>RELEVÉ DE COMPTE BANCAIRE</h1>"
                       "<p><b>Titulaire :</b> %1</p>"
                       "<p><b>Numéro de Compte :</b> %2</p>"
                       "<p><b>Type :</b> %3</p>"
                       "<p><b>Solde Actuel :</b> %4 €</p>"
                       "<h2>Historique des Opérations</h2>"
                       "<table><tr><th>Date / Heure</th><th>Type</th><th>Montant</th><th>Solde</th></tr>"
                       ).arg(acc.getDepositorName()).arg(acc.getAccountNumber()).arg(acc.getAccountType()).arg(acc.getBalance(), 0, 'f', 2);

    for (const Transaction &t : acc.getTransactionHistory()) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3 €</td><td>%4 €</td></tr>")
                    .arg(t.timestamp).arg(t.type).arg(t.amount, 0, 'f', 2).arg(t.newBalance, 0, 'f', 2);
    }

    html += "</table></body></html>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    doc.print(&printer);

    QMessageBox::information(this, "Succès", "Relevé de compte exporté en PDF avec succès !");
}

// 11. Réinitialiser la vue
void MainWindow::onReset()
{
    ui->searchAccLineEdit->clear();
    ui->amountLineEdit->clear();
    ui->detailsTextEdit->clear();
    currentAccountIndex = -1;
    statusBar()->showMessage("Interface réinitialisée.");
}

// 12. Quitter l'application
void MainWindow::onExit()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Êtes-vous sûr de vouloir quitter l'application ?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // 💾 Sauvegarde finale avant fermeture
        saveAccountsToFile();
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

// ------------------------------------------------------------------
// 💾 GESTION DE LA PERSISTANCE DES DONNÉES (FILE I/O)
// ------------------------------------------------------------------

// Écrit la liste des comptes dans le fichier accounts.txt (Format CSV)
void MainWindow::saveAccountsToFile()
{
    QFile file("accounts.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    for (const BankAccount &acc : accountList) {
        out << acc.getAccountNumber() << ";"
            << acc.getDepositorName() << ";"
            << acc.getAccountType() << ";"
            << acc.getBalance() << ";"
            << acc.getPin() << "\n";
    }

    file.close();
}

// Reconstruit la liste des comptes à partir du fichier accounts.txt
void MainWindow::loadAccountsFromFile()
{
    QFile file("accounts.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return; // Le fichier n'existe pas encore au premier lancement
    }

    accountList.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() >= 4) {
            int accNum = parts[0].toInt();
            QString name = parts[1];
            QString type = parts[2];
            double balance = parts[3].toDouble();
            QString pin = (parts.size() >= 5) ? parts[4] : "0000";

            BankAccount acc;
            acc.createAccount(name, accNum, type, balance, pin);
            accountList.append(acc);
        }
    }

    file.close();
}