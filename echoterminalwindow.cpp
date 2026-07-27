#include "echoterminalwindow.h"
#include "ui_echoterminalwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QRandomGenerator>
#include <QScrollBar>
#include <QApplication>
#include <QDebug>
#include <QStyle>

namespace {
const int FLICKER_INTERVAL_MS = 2000; // Проверка мигания каждые 2 секунды
}

EchoTerminalWindow::EchoTerminalWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EchoTerminalWindow)
{
    ui->setupUi(this);

    // ==========================================
    // 1. ЗАГРУЗКА И ПРИМЕНЕНИЕ QSS СТИЛЕЙ
    // ==========================================
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        qApp->setStyleSheet(style);
        styleFile.close();
    } else {
        qWarning() << "Warning: Could not load style.qss";
    }

    // ==========================================
    // 2. НАСТРОЙКА ИНТЕРФЕЙСА И ПУТЕЙ
    // ==========================================
    // Сразу ставим фокус на поле ввода, чтобы можно было печатать
    ui->commandInput->setFocus();

    // Кроссплатформенный путь для сохранения истории
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    logFilePath = dataPath + "/echo_history.log";

    // ==========================================
    // 3. ПОДКЛЮЧЕНИЕ СИГНАЛОВ И ЗАГРУЗКА ДАННЫХ
    // ==========================================
    connect(ui->commandInput, &QLineEdit::returnPressed,
            this, &EchoTerminalWindow::processCommand);

    loadHistory();

    // Приветственные сообщения
    printToScreen("=== ECHO TERMINAL v1.0 ===", true);
    printToScreen("System initialized successfully.", true);
    printToScreen("Type your thoughts and press Enter to save.", true);
    printToScreen("Commands: help, clear, exit, history", true);
    printToScreen("==========================", true);

    // ==========================================
    // 4. ЗАПУСК ТАЙМЕРА МИГАНИЯ
    // ==========================================
    flickerTimer = new QTimer(this);
    connect(flickerTimer, &QTimer::timeout,
            this, &EchoTerminalWindow::flickerScreen);
    flickerTimer->start(FLICKER_INTERVAL_MS);
}

EchoTerminalWindow::~EchoTerminalWindow()
{
    delete ui;
}

void EchoTerminalWindow::processCommand()
{
    QString input = ui->commandInput->text().trimmed();

    if (input.isEmpty()) {
        return;
    }

    QString lowerInput = input.toLower();

    if (lowerInput == "help" || lowerInput == "помощь") {
        printToScreen("Available commands:", true);
        printToScreen("  help    - Show this help message", true);
        printToScreen("  clear   - Clear screen", true);
        printToScreen("  exit    - Close application", true);
        printToScreen("  history - Show last 10 entries", true);
    }
    else if (lowerInput == "clear" || lowerInput == "очистить") {
        ui->screenOutput->clear();
        printToScreen("Screen cleared.", true);
        ui->commandInput->setFocus(); // Возвращаем фокус
    }
    else if (lowerInput == "exit" || lowerInput == "выход") {
        printToScreen("Goodbye! Your thoughts are saved.", true);
        QApplication::quit();
        return;
    }
    else if (lowerInput == "history" || lowerInput == "история") {
        printToScreen("Last 10 entries:", true);
        QFile file(logFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList lines;
            while (!in.atEnd()) {
                lines.append(in.readLine());
            }
            file.close();

            int startIdx = qMax(0, lines.size() - 10);
            for (int i = startIdx; i < lines.size(); ++i) {
                printToScreen("  " + lines[i], true);
            }
        } else {
            printToScreen("No history found.", true);
        }
    }
    else {
        // Обычный ввод
        printToScreen("> " + input);
        saveToHistory(input);

        // Эхо-ответ (время добавляется автоматически внутри printToScreen)
        printToScreen("Echo: Thought recorded.", true);
    }

    // Очистка поля и возврат фокуса для следующего ввода
    ui->commandInput->clear();
    ui->commandInput->setFocus();
}

void EchoTerminalWindow::printToScreen(const QString &text, bool isSystem)
{
    QString color = isSystem ? "#00AA00" : "#33FF33";
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

    QString formattedText;
    if (isSystem) {
        formattedText = QString("<span style='color: %1;'>[%2] %3</span><br>")
        .arg(color, timestamp, text);
    } else {
        formattedText = QString("<span style='color: %1; font-weight: bold;'>[%2] %3</span><br>")
        .arg(color, timestamp, text);
    }

    ui->screenOutput->append(formattedText);

    // Автоматическая прокрутка вниз
    QScrollBar *scrollBar = ui->screenOutput->verticalScrollBar();
    if (scrollBar) {
        scrollBar->setValue(scrollBar->maximum());
    }
}

void EchoTerminalWindow::saveToHistory(const QString &text)
{
    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        out << timestamp << " | " << text << "\n";
        file.close();
    } else {
        printToScreen("Error: Could not save to history file.", true);
    }
}

void EchoTerminalWindow::loadHistory()
{
    QFile file(logFilePath);
    if (!file.exists()) {
        return;
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        int count = 0;

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" | ");
            if (parts.size() > 1) {
                printToScreen("[RESTORED] " + parts.last());
                count++;
            }
        }
        file.close();

        if (count > 0) {
            printToScreen(QString("Restored %1 previous thoughts.").arg(count), true);
        }
    }
}

void EchoTerminalWindow::flickerScreen()
{
    static int flickerCount = 0;
    flickerCount++;

    // 1. Лёгкое мигание (30% шанс каждые 2 секунды)
    if (QRandomGenerator::global()->bounded(100) < 30) {
        ui->screenOutput->setProperty("flickerState", "light");
        ui->screenOutput->style()->unpolish(ui->screenOutput);
        ui->screenOutput->style()->polish(ui->screenOutput);

        QTimer::singleShot(50, this, [this]() {
            ui->screenOutput->setProperty("flickerState", "");
            ui->screenOutput->style()->unpolish(ui->screenOutput);
            ui->screenOutput->style()->polish(ui->screenOutput);
        });
    }

    // 2. Сильное "дрожание" раз в 1 минуту (30 тиков * 2 сек = 60 сек)
    if (flickerCount % 30 == 0) {
        ui->screenOutput->setProperty("flickerState", "strong");
        ui->screenOutput->style()->unpolish(ui->screenOutput);
        ui->screenOutput->style()->polish(ui->screenOutput);

        QTimer::singleShot(100, this, [this]() {
            ui->screenOutput->setProperty("flickerState", "");
            ui->screenOutput->style()->unpolish(ui->screenOutput);
            ui->screenOutput->style()->polish(ui->screenOutput);
        });

        // Защита от переполнения int
        if (flickerCount > 1000) {
            flickerCount = 0;
        }
    }
}
