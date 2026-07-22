#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🎨 Feuille de style QSS - Thème Sombre Professionnel
    QString darkTheme = R"(
        /* Style général de la fenêtre et des dialogues */
        QMainWindow, QDialog {
            background-color: #1e1e2e;
            color: #cdd6f4;
        }

        /* Cadres et groupes (QGroupBox) */
        QGroupBox {
            color: #89b4fa;
            border: 1px solid #45475a;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 12px;
            font-weight: bold;
        }

        /* Libellés (QLabel) */
        QLabel {
            color: #cdd6f4;
            font-size: 13px;
        }

        /* Champs de saisie (QLineEdit & QTextEdit) */
        QLineEdit, QTextEdit {
            background-color: #313244;
            color: #cdd6f4;
            border: 1px solid #45475a;
            border-radius: 6px;
            padding: 6px;
            selection-background-color: #89b4fa;
        }
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid #89b4fa;
        }

        /* Style par défaut des boutons (QPushButton) */
        QPushButton {
            background-color: #89b4fa;
            color: #11111b;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #b4befe;
        }
        QPushButton:pressed {
            background-color: #74c7ec;
        }

        /* Styles ciblés par objectName (Boutons spécifiques) */
        QPushButton#depositButton {
            background-color: #a6e3a1; /* Vert */
            color: #11111b;
        }
        QPushButton#depositButton:hover {
            background-color: #94e2d5;
        }

        QPushButton#withdrawButton {
            background-color: #f38ba8; /* Rouge */
            color: #11111b;
        }
        QPushButton#withdrawButton:hover {
            background-color: #f5e0dc;
        }

        QPushButton#resetButton, QPushButton#exitButton {
            background-color: #45475a;
            color: #cdd6f4;
        }
        QPushButton#resetButton:hover, QPushButton#exitButton:hover {
            background-color: #585b70;
        }

        /* Barre d'état (QStatusBar) */
        QStatusBar {
            background-color: #181825;
            color: #a6adc8;
        }
    )";

    // Application du thème à toute l'application
    a.setStyleSheet(darkTheme);

    // Gestion de la traduction (Code d'origine)
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "BankAccountSystem_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return QApplication::exec();
}