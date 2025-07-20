#!/bin/bash

echo "=== Проверка доступных GPIO библиотек на Raspberry Pi ==="
echo

echo "1. Проверка установленных пакетов GPIO:"
dpkg -l | grep -i gpio
echo

echo "2. Проверка библиотек в /usr/lib:"
ls /usr/lib/ | grep -i gpio
echo

echo "3. Проверка заголовочных файлов GPIO:"
find /usr/include -name "*gpio*" 2>/dev/null
echo

echo "4. Проверка libgpiod:"
pkg-config --exists libgpiod && echo "✅ libgpiod доступна" || echo "❌ libgpiod не найдена"
echo

echo "5. Проверка WiringPi:"
pkg-config --exists wiringpi && echo "✅ WiringPi доступна" || echo "❌ WiringPi не найдена"
echo

echo "6. Проверка sysfs GPIO:"
if [ -d "/sys/class/gpio" ]; then
    echo "✅ sysfs GPIO доступен"
    ls /sys/class/gpio/
else
    echo "❌ sysfs GPIO недоступен"
fi
echo

echo "7. Проверка /dev/gpiochip*:"
ls /dev/gpiochip* 2>/dev/null || echo "❌ gpiochip не найден"
echo

echo "8. Проверка доступных компиляторов:"
which gcc
which clang
echo

echo "9. Проверка версии ядра:"
uname -r
echo

echo "10. Проверка модели Raspberry Pi:"
cat /proc/device-tree/model 2>/dev/null || echo "Не Raspberry Pi"
echo

echo "=== Рекомендации ==="
echo "Если libgpiod не установлена, выполните:"
echo "sudo apt update"
echo "sudo apt install libgpiod-dev"
echo
echo "Если WiringPi не установлена, выполните:"
echo "sudo apt install wiringpi"
echo
echo "Для использования sysfs GPIO (устаревший метод):"
echo "Не рекомендуется, но может работать без дополнительных библиотек" 