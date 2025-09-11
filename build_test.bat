@echo off
echo === PK200 LCD ESP-IDF 專案建置測試 ===

REM 設定 ESP-IDF 環境
echo 設定 ESP-IDF 環境...
call D:\Espressif\frameworks\esp-idf-v5.4.2\export.bat

REM 檢查必要檔案
echo 檢查專案檔案...

set "files=CMakeLists.txt sdkconfig.defaults partitions.csv main\CMakeLists.txt main\main.c"
for %%f in (%files%) do (
    if exist "%%f" (
        echo ✓ %%f
    ) else (
        echo ✗ %%f ^(缺失^)
        exit /b 1
    )
)

REM 檢查組件檔案
set "components=components\lvgl\CMakeLists.txt components\display_driver\CMakeLists.txt components\touch_driver\CMakeLists.txt components\ui_components\CMakeLists.txt"
for %%f in (%components%) do (
    if exist "%%f" (
        echo ✓ %%f
    ) else (
        echo ✗ %%f ^(缺失^)
        exit /b 1
    )
)

REM 檢查 LVGL 庫
if exist "libraries\lvgl-release-v8.4" (
    echo ✓ LVGL 庫存在
) else (
    echo ✗ LVGL 庫缺失
    exit /b 1
)

REM 檢查 UI 檔案
set "ui_files=src\ui.c src\ui.h src\ui_Screen1.c src\ui_helpers.c"
for %%f in (%ui_files%) do (
    if exist "%%f" (
        echo ✓ %%f
    ) else (
        echo ✗ %%f ^(缺失^)
        exit /b 1
    )
)

echo 所有必要檔案檢查完成！

REM 設定目標
echo 設定目標為 ESP32...
idf.py set-target esp32
if %errorlevel% neq 0 (
    echo ✗ 目標設定失敗
    exit /b 1
)
echo ✓ 目標設定成功

REM 建置專案
echo 開始建置專案...
idf.py build
if %errorlevel% equ 0 (
    echo ✓ 專案建置成功！
    echo 可以使用以下命令燒錄：
    echo idf.py -p COM_PORT flash monitor
) else (
    echo ✗ 專案建置失敗
    echo 請檢查錯誤訊息並修正問題
    exit /b 1
)

echo === 建置測試完成 ===
pause
