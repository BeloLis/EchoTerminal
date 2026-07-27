# EchoTerminal

**Ретро-терминал с эффектом старого CRT-монитора**, написанный на C++ с использованием Qt Framework.

![Qt](https://img.shields.io/badge/Qt-5.15%2B-blue?logo=qt)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-purple?logo=cmake)
![License](https://img.shields.io/badge/License-MIT-green)

## Особенности

- **Аутентичный ретро-стиль** - чёрный фон, ярко-зелёный текст, моноширинный шрифт
- **CRT-эффекты** - реалистичное мигание экрана и периодическое "дрожание" как у старых мониторов
- **Автосохранение истории** - все ваши мысли сохраняются между сессиями
- **Кроссплатформенность** - работает на Linux, Windows и macOS
- **QSS стилизация**- профессиональное разделение логики и внешнего вида

## Быстрый старт

### Требования

- **Qt 5.15** или выше (Qt 6 поддерживается)
- **CMake 3.16** или выше
- **C++17** совместимый компилятор

### Сборка и запуск

```bash
# Клонируйте репозиторий
git clone https://github.com/yourusername/EchoTerminal.git
cd EchoTerminal

# Создайте директорию сборки
mkdir build && cd build

# Запустите CMake
cmake ..

# Соберите проект
make

# Запустите приложение
./EchoTerminal
```

### Сборка через Qt Creator

1. Откройте `CMakeLists.txt` в Qt Creator
2. Нажмите Ctrl+B (сборка)
3. Нажмите Ctrl+R (запуск)

## Использование

После запуска вы можете:

* Вводить текст и нажимать Enter — терминал сохранит вашу мысль
* `help` — показать список команд
* `clear` — очистить экран
* `history` — показать последние 10 записей
* `exit` — выйти из приложения

### Пример работы

[14:30:22] === ECHO TERMINAL v1.0 ===

[14:30:22] System initialized successfully.

[14:30:22] Type your thoughts and press Enter to save.

[14:30:45] > Сегодня изучаю Qt Framework

[14:30:45] Echo: Thought recorded.

## Кастомизация

Все стили находятся в файле `style.qss`. Вы можете легко изменить:

* Цвета текста и фона
* Размер шрифта
* Интенсивность эффектов мигания
* Границы и отступы

## Архитектура проекта

EchoTerminal/
├── CMakeLists.txt # Конфигурация сборки
├── main.cpp # Точка входа
├── echoterminalwindow.h # Заголовок основного окна
├── echoterminalwindow.cpp # Логика приложения
├── echoterminalwindow.ui # Интерфейс (Qt Designer)
├── style.qss # Стили (QSS)
├── resources.qrc # Ресурсы Qt
└── README.md # Документация


## Технологии

* Qt Widgets — GUI фреймворк
* QSS (Qt Style Sheets) — стилизация интерфейса
* CMake — система сборки
* C++17 — современный стандарт C++

## Хранение данных

История сохраняется в кроссплатформенном формате:

* Linux: `~/.local/share/EchoTerminal/echo_history.log`
* Windows: `%APPDATA%/EchoTerminal/echo_history.log`
* macOS: `~/Library/Application Support/EchoTerminal/echo_history.log`

## Вклад

Pull Request'ы приветствуются! Не стесняйтесь создавать Issues для предложений улучшений.
EOF
