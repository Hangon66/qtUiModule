#include "uiCheckbox.h"
#include <QPainter>
#include <QMouseEvent>
#include <QImage>

uiCheckbox::uiCheckbox(QWidget *parent)
    : uiImageTextMixin<QWidget>(parent)
{
    setMouseTracking(true);  // 启用鼠标追踪，用于悬浮状态检测
    setCursor(Qt::PointingHandCursor);  // 设置手型光标
}

void uiCheckbox::setChecked(bool checked)
{
    if (m_checked != checked) {
        m_checked = checked;
        emit stateChanged(m_checked);
        update();
    }
}

void uiCheckbox::toggle()
{
    setChecked(!m_checked);
}

void uiCheckbox::setUncheckedImage(const QString &imagePath)
{
    m_uncheckedPixmap = QPixmap(imagePath);
    if (m_autoStateImages) {
        generateUncheckedStateImages();
    }
    update();
}

void uiCheckbox::setUncheckedImage(const QPixmap &pixmap)
{
    m_uncheckedPixmap = pixmap;
    if (m_autoStateImages) {
        generateUncheckedStateImages();
    }
    update();
}

void uiCheckbox::setUncheckedHoverImage(const QString &imagePath)
{
    m_uncheckedHoverPixmap = QPixmap(imagePath);
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setUncheckedHoverImage(const QPixmap &pixmap)
{
    m_uncheckedHoverPixmap = pixmap;
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setUncheckedPressedImage(const QString &imagePath)
{
    m_uncheckedPressedPixmap = QPixmap(imagePath);
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setUncheckedPressedImage(const QPixmap &pixmap)
{
    m_uncheckedPressedPixmap = pixmap;
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setCheckedImage(const QString &imagePath)
{
    m_checkedPixmap = QPixmap(imagePath);
    if (m_autoStateImages) {
        generateCheckedStateImages();
    }
    update();
}

void uiCheckbox::setCheckedImage(const QPixmap &pixmap)
{
    m_checkedPixmap = pixmap;
    if (m_autoStateImages) {
        generateCheckedStateImages();
    }
    update();
}

void uiCheckbox::setCheckedHoverImage(const QString &imagePath)
{
    m_checkedHoverPixmap = QPixmap(imagePath);
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setCheckedHoverImage(const QPixmap &pixmap)
{
    m_checkedHoverPixmap = pixmap;
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setCheckedPressedImage(const QString &imagePath)
{
    m_checkedPressedPixmap = QPixmap(imagePath);
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setCheckedPressedImage(const QPixmap &pixmap)
{
    m_checkedPressedPixmap = pixmap;
    m_autoStateImages = false;
    update();
}

void uiCheckbox::setAutoStateImages(bool enabled)
{
    m_autoStateImages = enabled;
    if (enabled) {
        generateUncheckedStateImages();
        generateCheckedStateImages();
        update();
    }
}

void uiCheckbox::clearStateImages()
{
    m_uncheckedPixmap = QPixmap();
    m_uncheckedHoverPixmap = QPixmap();
    m_uncheckedPressedPixmap = QPixmap();
    m_checkedPixmap = QPixmap();
    m_checkedHoverPixmap = QPixmap();
    m_checkedPressedPixmap = QPixmap();
    update();
}

void uiCheckbox::setText(const QString &text)
{
    m_text = text;
    update();
}

void uiCheckbox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 计算可用区域（减去外边距）
    QRect widgetRect = rect();
    QRect contentRect(
        widgetRect.x() + m_marginLeft,
        widgetRect.y() + m_marginTop,
        widgetRect.width() - m_marginLeft - m_marginRight,
        widgetRect.height() - m_marginTop - m_marginBottom
    );

    // 根据状态选择背景颜色：按下 > 悬浮 > 默认
    QColor currentBgColor;
    if (m_pressed && m_pressedBgColor.isValid()) {
        currentBgColor = m_pressedBgColor;
    } else if (m_hovered && m_hoverBgColor.isValid()) {
        currentBgColor = m_hoverBgColor;
    } else {
        currentBgColor = m_bgColor;
    }
    
    // 绘制纯色背景
    paintRoundedRect(painter, contentRect, currentBgColor);

    // 获取当前应显示的图片
    QPixmap pixmap = currentPixmap();

    // 绘制图片
    if (!pixmap.isNull()) {
        QRect targetRect;
        if (m_scaleMode == Stretch) {
            targetRect = contentRect;
        } else {
            QSize baseSize = pixmap.size() * m_scaleRatio;
            QSize scaledSize = baseSize;
            scaledSize.scale(contentRect.size(), Qt::KeepAspectRatio);
            int x = contentRect.x() + (contentRect.width() - scaledSize.width()) / 2;
            int y = contentRect.y() + (contentRect.height() - scaledSize.height()) / 2;
            targetRect = QRect(x, y, scaledSize.width(), scaledSize.height());
        }
        painter.drawPixmap(targetRect, pixmap);
    }

    // 绘制文本
    if (!m_text.isEmpty()) {
        QFontMetrics fm(font());
        QSize textSize = fm.size(Qt::TextSingleLine, m_text);

        // 计算水平位置
        int textX;
        int hMarginPixels = static_cast<int>(contentRect.width() * m_hMargin);
        switch (m_hAlignment) {
        case HLeft:
            textX = contentRect.x() + hMarginPixels;
            break;
        case HRight:
            textX = contentRect.x() + contentRect.width() - textSize.width() - hMarginPixels;
            break;
        case HCenter:
        default:
            textX = contentRect.x() + (contentRect.width() - textSize.width()) / 2;
            break;
        }

        // 计算垂直位置
        int textY;
        int vMarginPixels = static_cast<int>(contentRect.height() * m_vMargin);
        switch (m_vAlignment) {
        case VTop:
            textY = contentRect.y() + vMarginPixels;
            break;
        case VBottom:
            textY = contentRect.y() + contentRect.height() - textSize.height() - vMarginPixels;
            break;
        case VCenter:
        default:
            textY = contentRect.y() + (contentRect.height() - textSize.height()) / 2;
            break;
        }

        // 绘制文本
        QRect textRect(textX, textY, textSize.width(), textSize.height());
        painter.setFont(font());
        QColor textColor = m_textColor.isValid() ? m_textColor : palette().color(QPalette::WindowText);
        painter.setPen(textColor);
        painter.drawText(textRect, Qt::AlignCenter, m_text);
    }
}

void uiCheckbox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void uiCheckbox::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_pressed) {
        m_pressed = false;
        // 只有在鼠标仍在控件内时才触发切换
        if (rect().contains(event->pos())) {
            toggle();
            emit clicked();
        }
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void uiCheckbox::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    m_hovered = true;
    update();
}

void uiCheckbox::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hovered = false;
    m_pressed = false;  // 鼠标离开取消按下状态
    update();
}

QSize uiCheckbox::minimumSizeHint() const
{
    // 获取当前状态的图片
    QPixmap pixmap = currentPixmap();
    if (!pixmap.isNull() && imageAffectsSizeHint()) {
        return pixmap.size();
    }
    return QSize(24, 24);  // 默认最小尺寸
}

QString uiCheckbox::getText() const
{
    return m_text;
}

QColor uiCheckbox::getDefaultTextColor() const
{
    return palette().color(QPalette::WindowText);
}

QSize uiCheckbox::getBaseSizeHint() const
{
    return QWidget::sizeHint();
}

QPixmap uiCheckbox::currentPixmap() const
{
    if (m_checked) {
        // 选中状态：按下 > 悬浮 > 默认
        if (m_pressed && !m_checkedPressedPixmap.isNull()) {
            return m_checkedPressedPixmap;
        }
        if (m_hovered && !m_checkedHoverPixmap.isNull()) {
            return m_checkedHoverPixmap;
        }
        if (!m_checkedPixmap.isNull()) {
            return m_checkedPixmap;
        }
    } else {
        // 未选中状态：按下 > 悬浮 > 默认
        if (m_pressed && !m_uncheckedPressedPixmap.isNull()) {
            return m_uncheckedPressedPixmap;
        }
        if (m_hovered && !m_uncheckedHoverPixmap.isNull()) {
            return m_uncheckedHoverPixmap;
        }
        if (!m_uncheckedPixmap.isNull()) {
            return m_uncheckedPixmap;
        }
    }
    return QPixmap();
}

void uiCheckbox::generateUncheckedStateImages()
{
    generateStateImagesFromBase(m_uncheckedPixmap, m_uncheckedHoverPixmap, m_uncheckedPressedPixmap);
}

void uiCheckbox::generateCheckedStateImages()
{
    generateStateImagesFromBase(m_checkedPixmap, m_checkedHoverPixmap, m_checkedPressedPixmap);
}

void uiCheckbox::generateStateImagesFromBase(const QPixmap &basePixmap, 
                                              QPixmap &hoverPixmap, 
                                              QPixmap &pressedPixmap)
{
    if (basePixmap.isNull()) {
        hoverPixmap = QPixmap();
        pressedPixmap = QPixmap();
        return;
    }

    // 转换为非预乘 ARGB32 格式，避免颜色失真
    QImage baseImage = basePixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    // 生成悬浮状态图片（变亮）
    QImage hoverImage = baseImage.copy();
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
        hoverPixmap = QPixmap::fromImage(hoverImage);
    }

    // 生成按下状态图片（变暗）
    QImage pressedImage = baseImage.copy();
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
        pressedPixmap = QPixmap::fromImage(pressedImage);
    }
}
