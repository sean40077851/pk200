#!/bin/bash

# ESP-IDF 專案建置測試腳本

echo "=== PK200 LCD ESP-IDF 專案建置測試 ==="

# 檢查 ESP-IDF 環境
if [ -z "$IDF_PATH" ]; then
    echo "警告: ESP-IDF 環境未設定。"
    echo "請在 Windows 上執行以下命令設定環境："
    echo "D:\\Espressif\\frameworks\\esp-idf-v5.4.2\\export.bat"
    echo "或在 Linux/Mac 上執行: . \$HOME/esp/esp-idf/export.sh"
    echo ""
    echo "繼續檢查檔案結構..."
else
    echo "ESP-IDF 路徑: $IDF_PATH"
fi

# 檢查必要檔案
echo "檢查專案檔案..."

required_files=(
    "CMakeLists.txt"
    "sdkconfig.defaults"
    "partitions.csv"
    "main/CMakeLists.txt"
    "main/main.c"
    "components/lvgl/CMakeLists.txt"
    "components/display_driver/CMakeLists.txt"
    "components/touch_driver/CMakeLists.txt"
    "components/ui_components/CMakeLists.txt"
)

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "✗ $file (缺失)"
        exit 1
    fi
done

# 檢查 LVGL 庫
if [ -d "libraries/lvgl-release-v8.4" ]; then
    echo "✓ LVGL 庫存在"
else
    echo "✗ LVGL 庫缺失"
    exit 1
fi

# 檢查 UI 檔案
ui_files=(
    "src/ui.c"
    "src/ui.h"
    "src/ui_Screen1.c"
    "src/ui_helpers.c"
)

for file in "${ui_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "✗ $file (缺失)"
        exit 1
    fi
done

echo "所有必要檔案檢查完成！"

# 嘗試配置專案
echo "配置專案..."
if idf.py set-target esp32s3; then
    echo "✓ 目標設定成功"
else
    echo "✗ 目標設定失敗"
    exit 1
fi

# 嘗試建置專案
echo "開始建置專案..."
if idf.py build; then
    echo "✓ 專案建置成功！"
    echo "可以使用以下命令燒錄："
    echo "idf.py -p /dev/ttyUSB0 flash monitor"
else
    echo "✗ 專案建置失敗"
    echo "請檢查錯誤訊息並修正問題"
    exit 1
fi

echo "=== 建置測試完成 ==="
