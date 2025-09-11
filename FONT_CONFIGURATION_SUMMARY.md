# 🔤 LVGL Montserrat 字型配置總結

## ✅ 已啟用的 Montserrat 字型

根據 UI 檔案中的實際使用情況，已在 `components/lvgl/lv_conf.h` 中啟用以下字型：

### 📋 啟用的字型清單：

| 字型大小 | 狀態 | 使用位置 |
|---------|------|----------|
| `LV_FONT_MONTSERRAT_10` | ✅ **啟用** | Home, Step1, Step2, Welcome 畫面的小標籤 |
| `LV_FONT_MONTSERRAT_12` | ✅ **啟用** | 大部分文字標籤、日期、電話號碼 |
| `LV_FONT_MONTSERRAT_14` | ✅ **啟用** | 預設字型、按鈕標籤、一般文字 |
| `LV_FONT_MONTSERRAT_20` | ✅ **啟用** | 中等大小標題 |
| `LV_FONT_MONTSERRAT_24` | ✅ **啟用** | 卡片標題、重要資訊 |
| `LV_FONT_MONTSERRAT_26` | ✅ **啟用** | Charging 畫面的數值顯示 |
| `LV_FONT_MONTSERRAT_28` | ✅ **啟用** | Charging 畫面的時間顯示 |
| `LV_FONT_MONTSERRAT_32` | ✅ **啟用** | 百分比數字、重要數值 |
| `LV_FONT_MONTSERRAT_34` | ✅ **啟用** | Charging 畫面的大數值 |
| `LV_FONT_MONTSERRAT_38` | ✅ **啟用** | End 畫面的大標題 |
| `LV_FONT_MONTSERRAT_48` | ✅ **啟用** | End 畫面的價格顯示 |

### 🚫 未啟用的字型：

| 字型大小 | 狀態 | 原因 |
|---------|------|------|
| `LV_FONT_MONTSERRAT_8` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_16` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_18` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_22` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_30` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_36` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_40` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_42` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_44` | ❌ 未啟用 | UI 中未使用 |
| `LV_FONT_MONTSERRAT_46` | ❌ 未啟用 | UI 中未使用 |

## 📊 字型使用統計

### 各畫面使用的字型：

#### 🏠 Home 畫面
- `lv_font_montserrat_10` - 價格和幫助標籤
- `lv_font_montserrat_12` - 電話號碼、日期、充電器ID

#### 🔌 Step1 畫面  
- `lv_font_montserrat_10` - 價格和幫助標籤
- `lv_font_montserrat_12` - 電話號碼、日期、充電器ID
- `lv_font_montserrat_14` - 步驟說明

#### 🔐 Step2 畫面
- `lv_font_montserrat_10` - 價格和幫助標籤
- `lv_font_montserrat_12` - 電話號碼、日期、充電器ID、步驟標籤

#### 🎉 Welcome 畫面
- `lv_font_montserrat_10` - 歡迎文字
- `lv_font_montserrat_12` - 電話號碼、日期、費用標籤、卡片文字
- `lv_font_montserrat_24` - 費用數值

#### ⚡ Charging 畫面
- `lv_font_montserrat_12` - 電話號碼、日期、小標籤
- `lv_font_montserrat_14` - 一般標籤
- `lv_font_montserrat_20` - 中等標題
- `lv_font_montserrat_26` - 數值顯示
- `lv_font_montserrat_28` - 時間顯示
- `lv_font_montserrat_32` - 百分比數字
- `lv_font_montserrat_34` - 大數值

#### 🏁 End 畫面
- `lv_font_montserrat_12` - 電話號碼、日期、小標籤
- `lv_font_montserrat_14` - 一般文字
- `lv_font_montserrat_20` - 標題
- `lv_font_montserrat_38` - 大標題
- `lv_font_montserrat_48` - 價格顯示

#### 🔗 Connect 畫面
- `lv_font_montserrat_12` - 認證測試文字

## 🎯 配置檔案位置

**檔案**：`components/lvgl/lv_conf.h`
**區段**：第 320-340 行（字型配置區域）

## 💾 記憶體影響

啟用的字型將會增加 Flash 記憶體使用量：

- **已啟用**：11 個字型大小
- **預估大小**：每個字型約 50-200KB（取決於字型大小）
- **總計**：約 800KB - 1.5MB Flash 記憶體

## ⚠️ 重要提醒

1. **編譯時間**：啟用更多字型會增加編譯時間
2. **Flash 使用**：確保 ESP32 有足夠的 Flash 空間
3. **字型品質**：Montserrat 字型提供良好的可讀性
4. **一致性**：所有 UI 畫面使用統一的字型系列

## 🔧 如何修改字型配置

如果需要調整字型配置，請編輯 `components/lvgl/lv_conf.h`：

```c
// 啟用字型
#define LV_FONT_MONTSERRAT_XX 1

// 停用字型  
#define LV_FONT_MONTSERRAT_XX 0
```

## ✅ 編譯驗證

配置完成後，所有 UI 檔案中使用的字型都已正確啟用，應該不會再出現 `lv_font_montserrat_xx` 未定義的錯誤。

現在可以進行編譯測試來驗證字型配置是否正確！
