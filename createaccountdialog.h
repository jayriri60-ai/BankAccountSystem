#ifndef CREATEACCOUNTDIALOG_H
#define CREATEACCOUNTDIALOG_H

#include <QDialog>
#include "bankaccount.h"

namespace Ui {
class CreateAccountDialog;
}

class CreateAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAccountDialog(QWidget *parent = nullptr);
    ~CreateAccountDialog();

    // Permet à MainWindow de récupérer le compte une fois créé
    BankAccount getCreatedAccount() const;

private slots:
    // Slots déclenchés par les clics sur les boutons
    void onCreateClicked();
    void onCancelClicked();

private:
    Ui::CreateAccountDialog *ui;
    BankAccount createdAccount; // Stocke l'objet compte créé
};

#endif // CREATEACCOUNTDIALOG_H