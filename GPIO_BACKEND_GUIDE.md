# Руководство по GPIO Backend'ам

## 🔍 Проблема

На Raspberry Pi может отсутствовать библиотека `libgpiod`, что вызывает ошибку:
```bash
/usr/bin/ld: cannot find -lgpiod: No such file or directory
```

## 📚 Доступные GPIO библиотеки

### 1. **libgpiod** (современный, рекомендуемый)
- **Описание:** Современная библиотека для работы с GPIO через character device
- **Установка:** `sudo apt install libgpiod-dev`
- **Преимущества:** Безопасность, производительность, поддержка новых ядер
- **Недостатки:** Может отсутствовать на старых системах

### 2. **pigpio** (мощная, популярная)
- **Описание:** Мощная библиотека с поддержкой PWM, сервоприводов, I2C, SPI
- **Установка:** `sudo apt install libpigpio-dev` или уже установлена
- **Преимущества:** Высокая производительность, много функций, стабильность
- **Недостатки:** Требует демон pigpiod

### 3. **WiringPi** (популярная)
- **Описание:** Популярная библиотека для Raspberry Pi
- **Установка:** `sudo apt install wiringpi`
- **Преимущества:** Широкая поддержка, много примеров
- **Недостатки:** Устаревшая, может не работать на новых ядрах

### 4. **sysfs** (встроенная)
- **Описание:** Встроенный интерфейс через файловую систему
- **Установка:** Не требуется (встроен в ядро)
- **Преимущества:** Всегда доступен, не требует библиотек
- **Недостатки:** Устаревший, медленный, небезопасный

## 🛠️ Решение проблемы

### Шаг 1: Проверка доступных библиотек

Запустите скрипт проверки:
```bash
chmod +x check_gpio_libs.sh
./check_gpio_libs.sh
```

### Шаг 2: Установка нужной библиотеки

#### Для libgpiod:
```bash
sudo apt update
sudo apt install libgpiod-dev
```

#### Для pigpio (уже установлена на вашей системе):
```bash
# Проверить, что pigpiod демон запущен
sudo systemctl status pigpiod

# Если не запущен, запустить
sudo systemctl start pigpiod
sudo systemctl enable pigpiod
```

#### Для WiringPi:
```bash
sudo apt install wiringpi
```

#### sysfs не требует установки (встроен)

### Шаг 3: Компиляция с нужным backend'ом

#### Используя гибкий Makefile:
```bash
# Для pigpio (рекомендуется для вашей системы)
make -f Makefile_flexible GPIO_BACKEND=pigpio test-flexible

# Для libgpiod
make -f Makefile_flexible GPIO_BACKEND=libgpiod test-flexible

# Для WiringPi
make -f Makefile_flexible GPIO_BACKEND=wiringpi test-flexible

# Для sysfs (не требует библиотек)
make -f Makefile_flexible GPIO_BACKEND=sysfs test-flexible
```

#### Прямая компиляция:
```bash
# pigpio (рекомендуется)
gcc -Wall -Wextra -g -DGPIO_BACKEND=3 -o test-pigpio test_gpio_enhanced_flexible.c -lpigpio

# libgpiod
gcc -Wall -Wextra -g -DGPIO_BACKEND=1 -o test-libgpiod test_gpio_enhanced_flexible.c -lgpiod

# WiringPi
gcc -Wall -Wextra -g -DGPIO_BACKEND=2 -o test-wiringpi test_gpio_enhanced_flexible.c -lwiringPi

# sysfs
gcc -Wall -Wextra -g -DGPIO_BACKEND=4 -o test-sysfs test_gpio_enhanced_flexible.c
```

## 🚀 Рекомендуемое решение для вашей системы

Поскольку у вас установлена **pigpio**, используйте её:

```bash
# Компиляция с pigpio
gcc -Wall -Wextra -g -DGPIO_BACKEND=3 -o test-pigpio test_gpio_enhanced_flexible.c -lpigpio

# Запуск
sudo ./test-pigpio
```

Или с Makefile:
```bash
make -f Makefile_flexible GPIO_BACKEND=pigpio test-flexible
sudo ./test-flexible-pigpio
```

## 📊 Сравнение backend'ов

| Backend | Установка | Производительность | Безопасность | Совместимость | Функции |
|---------|-----------|-------------------|--------------|---------------|---------|
| **pigpio** | Встроен | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **libgpiod** | `sudo apt install libgpiod-dev` | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| **WiringPi** | `sudo apt install wiringpi` | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐ |
| **sysfs** | Встроен | ⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |

## 🎯 Рекомендации для вашей системы

### Лучший выбор для вашей Raspberry Pi:
1. **pigpio** - уже установлена, высокая производительность
2. **sysfs** - встроен, всегда работает
3. **libgpiod** - современный, но может потребовать установки
4. **WiringPi** - устаревшая, но может работать

### Для быстрого тестирования:
```bash
# Использовать pigpio (уже установлена)
make -f Makefile_flexible GPIO_BACKEND=pigpio test-flexible
sudo ./test-flexible-pigpio
```

## 🔧 Примеры использования

### Тестирование всех backend'ов:
```bash
# Собрать все версии
make -f Makefile_flexible test-all

# Запустить каждую
sudo ./test-flexible-pigpio
sudo ./test-flexible-libgpiod
sudo ./test-flexible-wiringpi  
sudo ./test-flexible-sysfs
```

### Отладка:
```bash
# Собрать с отладкой
make -f Makefile_flexible GPIO_BACKEND=pigpio DEBUG=1 test-flexible

# Запустить
sudo ./test-flexible-pigpio
```

## ⚠️ Важные замечания

1. **Всегда используйте sudo** для GPIO операций
2. **pigpio требует демон** - проверьте `sudo systemctl status pigpiod`
3. **pigpio** - отличный выбор для вашей системы
4. **sysfs** - самый надежный fallback

## 🆘 Если pigpio не работает

1. **Проверьте демон pigpiod:**
   ```bash
   sudo systemctl status pigpiod
   sudo systemctl start pigpiod
   ```

2. **Проверьте права доступа:**
   ```bash
   sudo usermod -a -G gpio $USER
   ```

3. **Используйте sysfs** (всегда работает):
   ```bash
   gcc -Wall -Wextra -g -DGPIO_BACKEND=4 -o test-sysfs test_gpio_enhanced_flexible.c
   sudo ./test-sysfs
   ```

## 🎉 Заключение

У вас установлена **отличная библиотека pigpio**! Используйте её для лучшей производительности:

```bash
make -f Makefile_flexible GPIO_BACKEND=pigpio test-flexible
sudo ./test-flexible-pigpio
```

Это даст вам **максимальную производительность** и **стабильность** для тестирования GPIO! 🚀 