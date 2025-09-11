@echo off
echo === 快速編譯測試 ===

REM 設定 ESP-IDF 環境
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 清理建置目錄
if exist build (
    echo 清理建置目錄...
    rmdir /s /q build
)

REM 設定目標並建置
echo 設定目標為 ESP32...
idf.py set-target esp32

echo 開始建置...
idf.py build

echo 測試完成
pause
