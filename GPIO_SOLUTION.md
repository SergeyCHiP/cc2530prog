# Решение проблем с GPIO

## 🚨 Проблема с pigpio

При запуске pigpio на Raspberry Pi возникла ошибка:
```
Sorry, this system does not appear to be a raspberry pi.
aborting.
```

## 🔍 Диагностика

### Проверка системы:
```bash
# Проверка модели Raspberry Pi
cat /proc/cpuinfo | grep Model

# Проверка версии pigpio
pigpiod -v

# Проверка демона pigpiod
sudo systemctl status pigpiod
```

## 🛠️ Решения

### 1. **Mock-версия для разработки** (рекомендуется)

Используйте mock-версию для тестирования логики GPIO без реального оборудования:

```bash
# Сборка mock-версии
make -f Makefile_mock

# Запуск
./test-mock

# Или одной командой
make -f Makefile_mock run
```

**Преимущества:**
- ✅ Работает на любой системе (macOS, Linux, Windows)
- ✅ Не требует реального GPIO оборудования
- ✅ Быстрое тестирование логики
- ✅ Безопасно для разработки

### 2. **Исправление pigpio**

Если нужно использовать реальный pigpio:

```bash
# Перезапуск демона pigpiod
sudo systemctl stop pigpiod
sudo systemctl start pigpiod
sudo systemctl enable pigpiod

# Проверка статуса
sudo systemctl status pigpiod

# Ручной запуск демона
sudo pigpiod

# Проверка версии
pigpiod -v
```

### 3. **Альтернативные библиотеки**

Если pigpio не работает, используйте другие библиотеки:

#### **WiringPi** (если установлена):
```bash
gcc -Wall -Wextra -g -o test-wiringpi test_gpio_wiringpi.c -lwiringPi
sudo ./test-wiringpi
```

#### **sysfs** (встроенная):
```bash
gcc -Wall -Wextra -g -o test-sysfs test_gpio_sysfs.c
sudo ./test-sysfs
```

## 📊 Сравнение решений

| Решение | Простота | Совместимость | Производительность | Требования |
|---------|----------|---------------|-------------------|------------|
| **Mock** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | Нет |
| pigpio | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | Raspberry Pi |
| WiringPi | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ | Raspberry Pi |
| sysfs | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | Linux |

## 🎯 Рекомендации

### Для разработки:
1. **Используйте mock-версию** для тестирования логики
2. **Отлаживайте алгоритмы** без реального оборудования
3. **Быстрое тестирование** изменений

### Для продакшена:
1. **Исправьте pigpio** или используйте альтернативу
2. **Протестируйте на реальном оборудовании**
3. **Настройте права доступа**

## 🚀 Быстрый старт

### Mock-версия (рекомендуется):
```bash
# Сборка и запуск
make -f Makefile_mock run

# Результат:
# ✅ All tests passed! Mock GPIO backend is working correctly.
# 💡 This validates the GPIO logic without real hardware.
```

### Если pigpio работает:
```bash
# Сборка и запуск
make -f Makefile_pigpio run

# Результат:
# 🎉 All tests passed! pigpio GPIO backend is working correctly.
```

## 🔧 Отладка pigpio

### Проверка системы:
```bash
# Проверка модели
cat /proc/cpuinfo | grep Model

# Проверка версии
pigpiod -v

# Проверка демона
sudo systemctl status pigpiod
```

### Переустановка pigpio:
```bash
# Удаление
sudo apt remove pigpio pigpiod

# Установка
sudo apt update
sudo apt install pigpio pigpiod

# Запуск
sudo systemctl start pigpiod
sudo systemctl enable pigpiod
```

## 📋 Команды для копирования

### Mock-версия:
```bash
make -f Makefile_mock run
```

### pigpio (если работает):
```bash
make -f Makefile_pigpio run
```

### Проверка системы:
```bash
cat /proc/cpuinfo | grep Model
sudo systemctl status pigpiod
```

## 🎉 Заключение

**Mock-версия** - лучшее решение для разработки и тестирования GPIO логики без реального оборудования. Она позволяет:

- ✅ Быстро тестировать изменения
- ✅ Работать на любой системе
- ✅ Безопасно отлаживать код
- ✅ Валидировать алгоритмы

**Используйте mock для разработки, pigpio для продакшена!** 🚀 