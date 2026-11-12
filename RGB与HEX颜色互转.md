# RGB 与 HEX 颜色互转

> 来源：ColorConv 项目
> 平台：跨平台 (Qt5/Qt6)

---

## 一、核心函数

### HEX → RGB

```cpp
QString HexToRgb(QString hex)
{
    // 1. 去除 # 前缀
    if (hex.contains("#"))
        hex.remove(0, 1);

    // 2. 长度校验（不含 # 最多 6 位）
    if (hex.length() > 6)       // 实际代码用 hex.length() > 7 包含#
        return "Error";

    // 3. 将十六进制字符串解析为无符号整数，构造 QColor
    QColor color(hex.toUInt(nullptr, 16));
    // toUInt base=16 将 "00BFFF" 转为 0x0000BFFF
    // QColor(QRgb) 构造函数提取 R G B 通道

    // 4. 提取各通道
    int r = color.red();      // 0
    int g = color.green();    // 191
    int b = color.blue();     // 255

    // 5. 格式化为 "R, G, B"
    return QString("%1, %2, %3").arg(r).arg(g).arg(b);
    // 结果: "0, 191, 255"
}
```

### RGB → HEX

```cpp
QString RgbToHex(QString rgb)
{
    // 1. 按分隔符拆分
    QStringList list;
    if (rgb.contains(","))
        list = rgb.split(",");    // "0, 191, 255" → ["0", " 191", " 255"]
    else if (rgb.contains(" "))
        list = rgb.split(" ");

    // 2. 分量数量校验
    if (list.size() < 3)
        return "Error";

    // 3. 构造 QColor（自动钳位到 0-255）
    QColor color(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt());

    // 4. 钳位 + 零填充 2 位大写十六进制
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    QString sr = QString("%1").arg(r & 0xFF, 2, 16, QLatin1Char('0'));
    QString sg = QString("%1").arg(g & 0xFF, 2, 16, QLatin1Char('0'));
    QString sb = QString("%1").arg(b & 0xFF, 2, 16, QLatin1Char('0'));

    // 5. 拼接 # 前缀
    return QString("#%1%2%3").arg(sr.toUpper()).arg(sg.toUpper()).arg(sb.toUpper());
    // 结果: "#00BFFF"
}
```

---

## 二、关键技术点

### 1. 十六进制解析

```cpp
hex.toUInt(nullptr, 16)
```
- `base=16` 将字符串按十六进制解释
- 允许带或不带 `0x`/`#` 前缀（已在前面去除 `#`）
- 超出范围的值被静默截断

### 2. RGB 钳位 `& 0xFF`

```cpp
red & 0xFF   // 等效于 red % 256，强制 0-255 范围
```

### 3. 零填充格式化

```cpp
QString("%1").arg(value, 2, 16, QLatin1Char('0'))
//            arg(值, 最小宽度, 进制, 填充字符)
// arg(15, 2, 16, '0') → "0f" → toUpper() → "0F"
```

### 4. 分隔符兼容

代码支持逗号分隔 (`0,191,255`) 和空格分隔 (`0 191 255`) 两种 RGB 输入格式。`toInt()` 会自动处理前后空格。

---

## 三、色块文字对比度

```cpp
void setLabelColor(QColor color)
{
    int r = color.red(), g = color.green(), b = color.blue();

    // 设置背景色
    QString style = QString("background-color:rgb(%1,%2,%3); border-radius:36px")
                        .arg(r).arg(g).arg(b);

    // 简单亮度启发式：平均值 >= 200 用黑字，否则白字
    int avg = (r + g + b) / 3;
    QString textColor = (avg >= 200) ? "#000000" : "#FEFEFE";
    style += QString("color:%1").arg(textColor);

    label->setStyleSheet(style);
}
```

**注意**：这是简化的亮度判断 `(R+G+B)/3`，非感知亮度公式 `0.299R + 0.587G + 0.114B`，但在多数场景下足够。

---

## 四、交互流程

```
用户在 HEX 输入框按 Enter
  → HexToRgb() 转换
  → 结果写入 RGB 输入框
  → 结果复制到系统剪贴板
  → 色块更新背景色
  → 显示 "copied" 提示 (2 秒后消失)

用户在 RGB 输入框按 Enter
  → RgbToHex() 转换
  → 结果写入 HEX 输入框
  → 结果复制到系统剪贴板
  → 色块更新背景色
  → 显示 "copied" 提示
```

---

## 五、已知局限

| 问题 | 说明 |
|------|------|
| 不支持 3 位简写十六进制 | `#FFF` 被解析为 `0x00000FFF`，结果 `0, 15, 255` 而非预期的 `255, 255, 255` |
| 不支持 Alpha 通道 | 仅处理 R G B 三通道，无 `#RRGGBBAA` 或 `rgba()` |
| 无输入校验 | 非数字 RGB 输入被 `toInt()` 返回 0，静默生成 `#000000` |
| HEX 输入最长 7 字符 | UI 层 `maxLength=7`，即 `#XXXXXX` |
