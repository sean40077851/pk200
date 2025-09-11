# PowerShell script to clean up unnecessary libraries

Write-Host "=== 清理不需要的庫檔案 ===" -ForegroundColor Green

$librariesToRemove = @(
    "AccelStepper", "Adafruit_BusIO", "Adafruit_GFX_Library", "Adafruit_ILI9341",
    "Adafruit_ST7789_Library", "Adafruit_ST7796S", "Adafruit_seesaw_Library",
    "AnimatedGIF", "ArduinoJson", "ArduinoModbus", "ArduinoRS485", "Arduino_ESP32_OTA",
    "Arduino_GT911_Library", "Arduino_GigaDisplayTouch", "Arduino_GigaDisplay_GFX",
    "Arduino_JSON", "BitBang_I2C", "Button2", "ESP32_HUB75_LED_MATRIX_PANEL_DMA_Display",
    "FT6236", "FT6336U", "FT81x_Arduino_Driver", "FastLED", "GFX_Library_for_Arduino",
    "GFX_Library_for_Arduino-1.5.0", "GFX_Root", "GxEPD2", "HUB75nano",
    "LittleFS_esp32-1.0.6", "ModbusMaster", "PNGdec", "RunningAverage", "SD",
    "TAMC_FT62X6", "TAMC_GT911", "TFT_eSPI", "TJpg_Decoder", "Time", "U8g2",
    "Vector", "WiFiManager", "lvgl", "lvgl_8.3.9"
)

$removedCount = 0
$totalCount = $librariesToRemove.Count

foreach ($library in $librariesToRemove) {
    $path = "libraries\$library"
    if (Test-Path $path) {
        try {
            Remove-Item -Recurse -Force $path
            Write-Host "✓ 刪除庫: $library" -ForegroundColor Yellow
            $removedCount++
        }
        catch {
            Write-Host "✗ 無法刪除: $library - $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

# Remove lv_conf.h from libraries directory
$lvConfPath = "libraries\lv_conf.h"
if (Test-Path $lvConfPath) {
    Remove-Item $lvConfPath
    Write-Host "✓ 刪除 libraries\lv_conf.h" -ForegroundColor Yellow
}

# Remove remaining PlatformIO directories
$dirsToRemove = @("lib", "include", "test")
foreach ($dir in $dirsToRemove) {
    if (Test-Path $dir) {
        try {
            Remove-Item -Recurse -Force $dir
            Write-Host "✓ 刪除目錄: $dir" -ForegroundColor Yellow
        }
        catch {
            Write-Host "✗ 無法刪除目錄: $dir - $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

Write-Host "`n=== 清理完成 ===" -ForegroundColor Green
Write-Host "已刪除 $removedCount / $totalCount 個庫檔案" -ForegroundColor Cyan
Write-Host "保留的重要庫: lvgl-release-v8.4" -ForegroundColor Green

Write-Host "`n現在的專案結構是純 ESP-IDF 格式" -ForegroundColor Green
Write-Host "可以執行 final_fix_and_build.bat 來測試編譯" -ForegroundColor Cyan
