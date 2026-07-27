#ifndef ECHOTERMINALWINDOW_H
#define ECHOTERMINALWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class EchoTerminalWindow; }
QT_END_NAMESPACE

class EchoTerminalWindow : public QMainWindow
{
    Q_OBJECT

public:
    EchoTerminalWindow(QWidget *parent = nullptr);
    ~EchoTerminalWindow();

private slots:
    void processCommand();
    void flickerScreen();

private:
    Ui::EchoTerminalWindow *ui;
    QString logFilePath;
    QTimer *flickerTimer;

    void loadHistory();
    void saveToHistory(const QString &text);
    void printToScreen(const QString &text, bool isSystem = false);
};

#endif // ECHOTERMINALWINDOW_H
