#include "uiPushbutton.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QImage>

uiPushbutton::uiPushbutton(QWidget *parent)
    : uiImageTextMixin<QPushButton>(parent)
{
    // 默认非扁平，保持原生按钮样式
    // 有图片时通过setImage设置flat为true
    setMouseTracking(true);  // 启用鼠标追踪，用于悬浮状态检测
    
    // 默认：控件最小为图像大小，可以更大
    setImageSizeMode(MinimumToImage);
}

void uiPushbutton::setImage(const QString &imagePath)
{
    uiImageTextMixin<QPushButton>::setImage(imagePath);
    if (m_autoStateImages) {
        generateStateImages();
    }
    // 有图片时设置flat去除边框，无图片时恢复原生样式
    setFlat(!image().isNull());
    update();
}

void uiPushbutton::setImage(const QPixmap &pixmap)
{
    uiImageTextMixin<QPushButton>::setImage(pixmap);
    if (m_autoStateImages) {
        generateStateImages();
    }
    // 有图片时设置flat去除边框，无图片时恢复原生样式
    setFlat(!image().isNull());
    update();
}

void uiPushbutton::setAutoStateImages(bool enabled)
{
    m_autoStateImages = enabled;
}

void uiPushbutton::generateStateImages()
{
    QPixmap normalPixmap = image();
    if (normalPixmap.isNull()) {
        m_hoverPixmap = QPixmap();
        m_pressedPixmap = QPixmap();
        return;
    }

    // 转换为非预乘 ARGB32 格式，避免颜色失真
    QImage normalImage = normalPixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // 生成悬浮状态图片（变亮）
    QImage hoverImage = normalImage.copy();
    {
        for (int y = 0; y < hoverImage.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(hoverImage.scanLine(y));
            for (int x = 0; x < hoverImage.width(); ++x) {
                QRgb pixel = line[x];
                int a = qAlpha(pixel);
                if (a > 0) {
                    // 只对不透明像素进行变亮
                    int r = qBound(0, qRed(pixel) + 40, 255);
                    int g = qBound(0, qGreen(pixel) + 40, 255);
                    int b = qBound(0, qBlue(pixel) + 40, 255);
                    line[x] = qRgba(r, g, b, a);
                }
            }
        }
        m_hoverPixmap = QPixmap::fromImage(hoverImage);
    }

    // 生成按下状态图片（变暗）
    QImage pressedImage = normalImage.copy();
    {
        for (int y = 0; y < pressedImage.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(pressedImage.scanLine(y));
            for (int x = 0; x < pressedImage.width(); ++x) {
                QRgb pixel = line[x];
                int a = qAlpha(pixel);
                if (a > 0) {
                    // 只对不透明像素进行变暗
                    int r = qBound(0, qRed(pixel) - 50, 255);
                    int g = qBound(0, qGreen(pixel) - 50, 255);
                    int b = qBound(0, qBlue(pixel) - 50, 255);
                    line[x] = qRgba(r, g, b, a);
                }
            }
        }
        m_pressedPixmap = QPixmap::fromImage(pressedImage);
    }

    update();
}

void uiPushbutton::setHoverImage(const QString &imagePath)
{
    m_hoverPixmap = QPixmap(imagePath);
    m_autoStateImages = false;  // 手动设置后禁用自动生成
    update();
}

void uiPushbutton::setHoverImage(const QPixmap &pixmap)
{
    m_hoverPixmap = pixmap;
    m_autoStateImages = false;  // 手动设置后禁用自动生成
    update();
}

void uiPushbutton::setPressedImage(const QString &imagePath)
{
    m_pressedPixmap = QPixmap(imagePath);
    m_autoStateImages = false;  // 手动设置后禁用自动生成
    update();
}

void uiPushbutton::setPressedImage(const QPixmap &pixmap)
{
    m_pressedPixmap = pixmap;
    update();
}

void uiPushbutton::clearStateImages()
{
    m_hoverPixmap = QPixmap();
    m_pressedPixmap = QPixmap();
    update();
}

void uiPushbutton::setTextOffset(qreal horizontalRatio, qreal verticalRatio)
{
    m_hOffsetRatio = qBound(-1.0, horizontalRatio, 1.0);
    m_vOffsetRatio = qBound(-1.0, verticalRatio, 1.0);
    update();
}

void uiPushbutton::setHorizontalOffset(qreal ratio)
{
    m_hOffsetRatio = qBound(-1.0, ratio, 1.0);
    update();
}

void uiPushbutton::setVerticalOffset(qreal ratio)
{
    m_vOffsetRatio = qBound(-1.0, ratio, 1.0);
    update();
}

void uiPushbutton::paintEvent(QPaintEvent *event)
{
    // 根据状态选择图片：优先级 点击 > 选中 > 悬浮 > 默认
    QPixmap currentPixmap;
    if ((isDown() || isChecked()) && !m_pressedPixmap.isNull()) {
        currentPixmap = m_pressedPixmap;
    } else if (underMouse() && !isChecked() && !m_hoverPixmap.isNull()) {
        currentPixmap = m_hoverPixmap;
    } else {
        currentPixmap = m_pixmap;
    }
    
    // 计算可用区域（减去外边距）
    QRect buttonRect = rect();
    QRect contentRect(
        buttonRect.x() + m_marginLeft,
        buttonRect.y() + m_marginTop,
        buttonRect.width() - m_marginLeft - m_marginRight,
        buttonRect.height() - m_marginTop - m_marginBottom
    );
    
    bool hasIcon = !m_icon.isNull();
    QString btnText = text();
    
    // 判断是否设置了任意背景图片（正常/悬浮/按下）
    bool hasAnyBgImage = !m_pixmap.isNull() || !m_hoverPixmap.isNull() || !m_pressedPixmap.isNull();
    
    // 根据状态选择背景颜色：按下 > 悬浮 > 默认
    QColor currentBgColor;
    if ((isDown() || isChecked()) && m_pressedBgColor.isValid()) {
        currentBgColor = m_pressedBgColor;
    } else if (underMouse() && !isChecked() && m_hoverBgColor.isValid()) {
        currentBgColor = m_hoverBgColor;
    } else {
        currentBgColor = m_bgColor;
    }
    
    // 没有任何背景图片、背景颜色且没有 Icon 时，完全使用原生按钮绘制
    if (!hasAnyBgImage && !currentBgColor.isValid() && !hasIcon) {
        QPushButton::paintEvent(event);
        return;
    }
    
    // 没有任何背景图片和背景颜色但有 Icon 时，用原生样式绘制背景，再绘制 Icon
    if (!hasAnyBgImage && !currentBgColor.isValid()) {
        QStyleOptionButton opt;
        opt.initFrom(this);
        opt.text = QString();
        opt.icon = QIcon();
        opt.features = QStyleOptionButton::None;
        if (isDown()) opt.state |= QStyle::State_Sunken;
        else opt.state |= QStyle::State_Raised;
        QPainter nativePainter(this);
        style()->drawControl(QStyle::CE_PushButton, &opt, &nativePainter, this);
        nativePainter.end();
        
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        paintIconAndText(painter, contentRect, btnText, hasIcon);
        return;
    }

    // 设置了背景图片或背景颜色，始终使用自定义绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 绘制纯色背景
    paintRoundedRect(painter, contentRect, currentBgColor);

    // 绘制当前状态的背景图片（可能为空，表示该状态无背景）
    if (!currentPixmap.isNull()) {
        QRect targetRect;
        if (m_scaleMode == Stretch) {
            targetRect = contentRect;
        } else {
            QSize baseSize = currentPixmap.size() * m_scaleRatio;
            QSize scaledSize = baseSize;
            scaledSize.scale(contentRect.size(), Qt::KeepAspectRatio);
            int x = contentRect.x() + (contentRect.width() - scaledSize.width()) / 2;
            int y = contentRect.y() + (contentRect.height() - scaledSize.height()) / 2;
            targetRect = QRect(x, y, scaledSize.width(), scaledSize.height());
        }
        painter.drawPixmap(targetRect, currentPixmap);
    }

    // 绘制 Icon 和文本
    paintIconAndText(painter, contentRect, btnText, hasIcon);
}

void uiPushbutton::paintIconAndText(QPainter &painter, const QRect &contentRect, const QString &btnText, bool hasIcon)
{
    if (btnText.isEmpty() && !hasIcon) {
        return;
    }
    
    QFontMetrics fm(font());
    QSize textSize = fm.size(Qt::TextSingleLine, btnText);
    QSize iconSize = hasIcon ? m_iconSize : QSize(0, 0);
    int spacing = hasIcon ? m_iconSpacing : 0;
    
    // 计算 Icon 和文本的组合区域大小
    int totalWidth, totalHeight;
    if (hasIcon) {
        switch (m_iconPosition) {
        case IconLeft:
        case IconRight:
            totalWidth = iconSize.width() + spacing + textSize.width();
            totalHeight = qMax(iconSize.height(), textSize.height());
            break;
        case IconTop:
        case IconBottom:
            totalWidth = qMax(iconSize.width(), textSize.width());
            totalHeight = iconSize.height() + spacing + textSize.height();
            break;
        }
    } else {
        totalWidth = textSize.width();
        totalHeight = textSize.height();
    }
    
    // 计算组合区域的起始位置（基于对齐方式）
    int baseX, baseY;
    int hMarginPixels = static_cast<int>(contentRect.width() * m_hMargin);
    int vMarginPixels = static_cast<int>(contentRect.height() * m_vMargin);
    
    switch (m_hAlignment) {
    case HLeft:
        baseX = contentRect.x() + hMarginPixels;
        break;
    case HRight:
        baseX = contentRect.x() + contentRect.width() - totalWidth - hMarginPixels;
        break;
    case HCenter:
    default:
        baseX = contentRect.x() + (contentRect.width() - totalWidth) / 2;
        break;
    }
    
    switch (m_vAlignment) {
    case VTop:
        baseY = contentRect.y() + vMarginPixels;
        break;
    case VBottom:
        baseY = contentRect.y() + contentRect.height() - totalHeight - vMarginPixels;
        break;
    case VCenter:
    default:
        baseY = contentRect.y() + (contentRect.height() - totalHeight) / 2;
        break;
    }
    
    // 应用偏移比例
    int maxHOffset = (contentRect.width() - totalWidth) / 2;
    int maxVOffset = (contentRect.height() - totalHeight) / 2;
    baseX += static_cast<int>(maxHOffset * m_hOffsetRatio);
    baseY += static_cast<int>(maxVOffset * m_vOffsetRatio);
    
    // 计算 Icon 和文本的具体位置
    int iconX = 0, iconY = 0, textX, textY;
    if (hasIcon) {
        switch (m_iconPosition) {
        case IconLeft:
            iconX = baseX;
            iconY = baseY + (totalHeight - iconSize.height()) / 2;
            textX = baseX + iconSize.width() + spacing;
            textY = baseY + (totalHeight - textSize.height()) / 2;
            break;
        case IconRight:
            iconX = baseX + textSize.width() + spacing;
            iconY = baseY + (totalHeight - iconSize.height()) / 2;
            textX = baseX;
            textY = baseY + (totalHeight - textSize.height()) / 2;
            break;
        case IconTop:
            iconX = baseX + (totalWidth - iconSize.width()) / 2;
            iconY = baseY;
            textX = baseX + (totalWidth - textSize.width()) / 2;
            textY = baseY + iconSize.height() + spacing;
            break;
        case IconBottom:
            iconX = baseX + (totalWidth - iconSize.width()) / 2;
            iconY = baseY + textSize.height() + spacing;
            textX = baseX + (totalWidth - textSize.width()) / 2;
            textY = baseY;
            break;
        }
        
        // 绘制 Icon
        QRect iconRect(iconX, iconY, iconSize.width(), iconSize.height());
        painter.drawPixmap(iconRect, m_icon.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        textX = baseX;
        textY = baseY;
    }
    
    // 绘制文本
    if (!btnText.isEmpty()) {
        QRect textRect(textX, textY, textSize.width(), textSize.height());
        painter.setFont(font());
        QColor textColor = m_textColor.isValid() ? m_textColor : palette().color(QPalette::ButtonText);
        painter.setPen(textColor);
        painter.drawText(textRect, Qt::AlignCenter, btnText);
    }
}

void uiPushbutton::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    if (!m_hoverPixmap.isNull()) {
        update();
    }
}

void uiPushbutton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (!m_hoverPixmap.isNull()) {
        update();
    }
}

QSize uiPushbutton::minimumSizeHint() const
{
    // 图像影响尺寸时，返回图像大小作为最小尺寸
    if (!image().isNull() && imageAffectsSizeHint()) {
        return image().size();
    }
    return QSize(0, 0);
}

// ==================== Mixin 虚方法实现 ====================

QString uiPushbutton::getText() const
{
    return QPushButton::text();
}

QColor uiPushbutton::getDefaultTextColor() const
{
    return palette().color(QPalette::ButtonText);
}

QSize uiPushbutton::getBaseSizeHint() const
{
    return QPushButton::sizeHint();
}
