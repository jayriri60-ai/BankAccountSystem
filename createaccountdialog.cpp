#include "createaccountdialog.h"
#include "ui_createaccountdialog.h"
#include <QMessageBox>
#include <QPushButton>

CreateAccountDialog::CreateAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateAccountDialog)
{
    ui->setupUi(this);
    setWindowTitle("Créer un nouveau compte bancaire");

    // Connexion des boutons aux slots de validation/annulation
    connect(ui->btnCreate, &QPushButton::clicked, this, &CreateAccountDialog::onCreateClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &CreateAccountDialog::onCancelClicked);
}

CreateAccountDialog::~CreateAccountDialog()
{
    delete ui;
}

// ------------------------------------------------------------------
// VALIDATION DES DONNÉES ET CRÉATION DU COMPTE
// ------------------------------------------------------------------
void CreateAccountDialog::onCreateClicked()
{
    // 1. Récupération des saisies de l'utilisateur
    QString name = ui->nameLineEdit->text().trimmed();
    QString accNumStr = ui->accountNumberLineEdit->text().trimmed();
    QString accountType = ui->typeComboBox->currentText();
    QString depositStr = ui->depositLineEdit->text().trimmed();

    // 2. Validation exigée par le cahier des charges (Part 5)

    // Règle 1 : Le nom ne doit pas être vide
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom du titulaire ne peut pas être vide !");
        return;
    }

    // Règle 2 : Le numéro de compte doit être un entier positif
    bool okNum = false;
    int accNum = accNumStr.toInt(&okNum);
    if (!okNum || accNum <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un numéro de compte valide (entier positif) !");
        return;
    }

    // Règle 3 : Le dépôt initial doit être un nombre >= 0
    bool okDeposit = false;
    double initialDeposit = depositStr.toDouble(&okDeposit);
    if (!okDeposit || initialDeposit < 0.0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le dépôt initial doit être un montant valide supérieur ou égal à 0 !");
        return;
    }

    // 3. Tout est valide ! On crée l'objet BankAccount
    createdAccount.createAccount(name, accNum, accountType, initialDeposit);

    // 4. Message de confirmation
    QMessageBox::information(this, "Succès", "Le compte a été créé avec succès !");

    // 5. Ferme le dialogue en renvoyant QDialog::Accepted
    accept();
}

void CreateAccountDialog::onCancelClicked()
{
    // Ferme le dialogue sans enregistrer
    reject();
}

BankAccount CreateAccountDialog::getCreatedAccount() const
{
    return createdAccount;
}