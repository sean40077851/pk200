@echo off
echo === 詳細編譯除錯腳本 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 檢查環境變數
echo 檢查環境變數...
echo IDF_PATH: %IDF_PATH%
echo PATH: %PATH%

REM 檢查 Python
echo 檢查 Python...
python --version
if %errorlevel% neq 0 (
    echo Python 檢查失敗
    pause
    exit /b 1
)

REM 檢查 idf.py
echo 檢查 idf.py...
idf.py --version
if %errorlevel% neq 0 (
    echo idf.py 檢查失敗
    pause
    exit /b 1
)

REM 清理建置目錄
echo 清理建置目錄...
if exist build (
    rmdir /s /q build
    echo 建置目錄已清理
)

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32
if %errorlevel% neq 0 (
    echo 目標設定失敗
    pause
    exit /b 1
)

REM 開始建置並顯示詳細輸出
echo 開始建置專案（詳細模式）...
idf.py -v build

echo.
echo === 建置完成 ===
if %errorlevel% equ 0 (
    echo ✓ 建置成功！
) else (
    echo ✗ 建置失敗，錯誤代碼: %errorlevel%
)

pause
