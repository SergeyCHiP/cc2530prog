# Руководство по pigpio GPIO

## 🎯 Упрощенное решение

Используем только **системную библиотеку pigpio**, которая уже установлена на вашем Raspberry Pi.

## 📚 pigpio - системная библиотека

### ✅ **Преимущества:**
- **Уже установлена** на вашей системе
- **Высокая производительность** (⭐⭐⭐⭐⭐)
- **Отличная стабильность**
- **Много функций** (PWM, I2C, SPI, сервоприводы)
- **Хорошая документация**

### 📦 **Установленные файлы:**
```
libpigpio.so
libpigpio.so.1
libpigpiod_if.so
libpigpiod_if.so.1
libpigpiod_if2.so
libpigpiod_if2.so.1
```

## 🚀 Быстрое использование

### Компиляция:
```bash
# Прямая компиляция
gcc -Wall -Wextra -g -o test-pigpio test_gpio_pigpio.c -lpigpio

# Или с Makefile
make -f Makefile_pigpio
```

### Запуск:
```bash
sudo ./test-pigpio
```

### Или одной командой:
```bash
make -f Makefile_pigpio run
```

## 📊 Тесты

Программа выполняет **6 типов тестов**:

1. **Enhanced GPIO Export** - тест экспорта пинов
2. **Enhanced GPIO Direction** - тест направлений (IN, OUT, HIGH)
3. **Enhanced GPIO Value** - тест чтения/записи значений
4. **GPIO Performance** - тест производительности (1000 операций)
5. **GPIO Stress** - стресс-тест (5 циклов по 100 операций)
6. **GPIO Cleanup** - тест очистки ресурсов

## 🔧 Отладка

### Сборка с отладкой:
```bash
make -f Makefile_pigpio DEBUG=1
```

### Проверка демона pigpiod:
```bash
sudo systemctl status pigpiod
sudo systemctl start pigpiod
```

## ⚠️ Важные замечания

1. **Всегда используйте sudo** для GPIO операций
2. **pigpio требует демон** - проверьте `sudo systemctl status pigpiod`
3. **Тестируемые пины:** 17, 27, 22
4. **Автоматическая очистка** при завершении программы

## 🆘 Если не работает

### 1. Проверьте демон pigpiod:
```bash
sudo systemctl status pigpiod
sudo systemctl start pigpiod
sudo systemctl enable pigpiod
```

### 2. Проверьте права доступа:
```bash
sudo usermod -a -G gpio $USER
```

### 3. Проверьте библиотеку:
```bash
ls /usr/lib/ | grep pigpio
```

## 📈 Ожидаемый результат

```
=== pigpio GPIO Test Program ===
Using system pigpio library
Testing GPIO: 17, 27, 22

=== Enhanced GPIO Export Tests (pigpio) ===
[PIGPIO] gpio_export(17)
✅ pigpio initialized successfully
✅ GPIO 17 exported successfully
...

=== Test Summary (pigpio) ===
Total tests: 6
Passed: 6
Failed: 0
Success rate: 100.0%
Total time: 150.25 ms
Average time per test: 25.04 ms

🎉 All tests passed! pigpio GPIO backend is working correctly.
✅ pigpio terminated
```

## 🎉 Преимущества упрощенного подхода

- ✅ **Простота** - только одна библиотека
- ✅ **Надежность** - системная библиотека
- ✅ **Производительность** - высокая скорость
- ✅ **Стабильность** - проверенная библиотека
- ✅ **Функциональность** - много возможностей

## 📋 Команды для копирования

```bash
# Сборка
make -f Makefile_pigpio

# Запуск
sudo ./test-pigpio

# Сборка и запуск одной командой
make -f Makefile_pigpio run

# Отладка
make -f Makefile_pigpio DEBUG=1
sudo ./test-pigpio
```

**Теперь у вас есть простое и надежное решение для тестирования GPIO!** 🚀 