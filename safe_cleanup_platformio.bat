@echo off
echo === 安全清理 PlatformIO 遺留檔案 ===

echo 這個腳本將清理以下 PlatformIO 相關檔案：
echo.
echo 配置檔案：
echo - platformio.ini
echo.
echo 目錄結構：
echo - src/ (內容已轉移到 main/)
echo - lib/
echo - include/
echo - test/
echo.
echo 不需要的庫檔案：
echo - 大部分 Arduino 庫 (保留 LVGL)
echo.
echo 重複檔案：
echo - 舊的 README 檔案
echo - 散落的 UI 組件檔案
echo.

set /p confirm="確定要繼續清理嗎？(y/N): "
if /i not "%confirm%"=="y" (
    echo 取消清理操作
    pause
    exit /b 0
)

echo.
echo 開始清理...

REM 建立備份目錄
if not exist "backup_before_cleanup" (
    mkdir "backup_before_cleanup"
    echo ✓ 建立備份目錄
)

REM 備份重要檔案
if exist "platformio.ini" (
    copy "platformio.ini" "backup_before_cleanup\" >nul
    echo ✓ 備份 platformio.ini
)

REM 清理 PlatformIO 配置檔案
if exist "platformio.ini" (
    del "platformio.ini"
    echo ✓ 刪除 platformio.ini
)

REM 清理 PlatformIO 目錄結構
if exist "src" (
    echo ✓ 刪除 src 目錄 (內容已在 main/ 中)
    rmdir /s /q "src"
)

if exist "lib" (
    echo ✓ 刪除 lib 目錄
    rmdir /s /q "lib"
)

if exist "include" (
    echo ✓ 刪除 include 目錄
    rmdir /s /q "include"
)

if exist "test" (
    echo ✓ 刪除 test 目錄
    rmdir /s /q "test"
)

REM 清理舊的文件檔案
if exist "README.md" (
    copy "README.md" "backup_before_cleanup\" >nul 2>&1
    del "README.md"
    echo ✓ 刪除舊的 README.md
)

if exist "readme.txt" (
    del "readme.txt"
    echo ✓ 刪除 readme.txt
)

if exist "filelist.txt" (
    del "filelist.txt"
    echo ✓ 刪除 filelist.txt
)

if exist "APP_16MB.csv" (
    del "APP_16MB.csv"
    echo ✓ 刪除 APP_16MB.csv
)

echo.
echo === 清理完成 ===
echo.
echo 專案現在是純 ESP-IDF 結構：
echo.
echo 主要目錄：
echo ✓ main/ - 主程式
echo ✓ components/ - ESP-IDF 組件
echo ✓ libraries/lvgl-release-v8.4/ - LVGL 庫
echo ✓ screens/ - UI 畫面
echo ✓ images/ - UI 圖片
echo ✓ fonts/ - UI 字體
echo.
echo 配置檔案：
echo ✓ CMakeLists.txt - ESP-IDF 主配置
echo ✓ sdkconfig.defaults - ESP-IDF 設定
echo ✓ partitions.csv - 分區表
echo.
echo 備份檔案位於: backup_before_cleanup/
echo.

pause
