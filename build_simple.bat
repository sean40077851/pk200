@echo off
echo Setting up ESP-IDF environment...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

echo Setting target to ESP32...
idf.py set-target esp32

echo Building project...
idf.py build

if %errorlevel% equ 0 (
    echo Build successful!
) else (
    echo Build failed!
)

pause
