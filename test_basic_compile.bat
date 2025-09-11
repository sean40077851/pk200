@echo off
echo === 基本編譯測試 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 清理建置目錄
echo 清理建置目錄...
if exist build (
    rmdir /s /q build
    echo 建置目錄已清理
)

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32

REM 開始基本編譯測試
echo 開始基本編譯測試（僅包含基本組件）...
idf.py build

if %errorlevel% equ 0 (
    echo.
    echo ✓ 基本編譯測試成功！
    echo 現在可以逐步添加其他組件
) else (
    echo.
    echo ✗ 基本編譯測試失敗
    echo 請檢查 ESP-IDF 環境和基本配置
)

echo.
echo === 測試完成 ===
pause
