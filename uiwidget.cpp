#include "uiwidget.h"
#include <QPainter>
#include <QDebug>

uiWidget::uiWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
}

// ==================== 图像设置 ====================

void uiWidget::setImage(const QString &imagePath)
{
    m_pixmap = QPixmap(imagePath);
    if (m_pixmap.isNull()) {
        qWarning() << "[uiWidget] 图像加载失败:" << imagePath;
    }
    update();
}

void uiWidget::setImage(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void uiWidget::clearImage()
{
    m_pixmap = QPixmap();
    update();
}

void uiWidget::setImageScaleMode(ImageScaleMode mode)
{
    m_scaleMode = mode;
    update();
}

// ==================== 文本对齐 ====================

void uiWidget::setHorizontalAlignment(HorizontalAlignment align)
{
    m_hAlignment = align;
    update();
}

void uiWidget::setVerticalAlignment(VerticalAlignment align)
{
    m_vAlignment = align;
    update();
}

// ==================== 文本边距 ====================

void uiWidget::setHorizontalMargin(qreal ratio)
{
    m_hMargin = qBound(0.0, ratio, 1.0);
    update();
}

void uiWidget::setVerticalMargin(qreal ratio)
{
    m_vMargin = qBound(0.0, ratio, 1.0);
    update();
}

// ==================== 文本样式 ====================

void uiWidget::setText(const QString &text)
{
    m_text = text;
    update();
}

QString uiWidget::text() const
{
    return m_text;
}

void uiWidget::setFontSize(int pointSize)
{
    QFont f = font();
    f.setPointSize(pointSize);
    setFont(f);
    update();
}

void uiWidget::setFontPixelSize(int pixelSize)
{
    QFont f = font();
    f.setPixelSize(pixelSize);
    setFont(f);
    update();
}

void uiWidget::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

// ==================== 绘制 ====================

void uiWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QRect widgetRect = rect();

    // 绘制背景图像
    if (!m_pixmap.isNull()) {
        QRect targetRect;
        if (m_scaleMode == Stretch) {
            // 拉伸填充
            targetRect = widgetRect;
        } else {
            // 保持宽高比，居中显示
            QSize scaledSize = m_pixmap.size();
            scaledSize.scale(widgetRect.size(), Qt::KeepAspectRatio);
            int x = (widgetRect.width() - scaledSize.width()) / 2;
            int y = (widgetRect.height() - scaledSize.height()) / 2;
            targetRect = QRect(x, y, scaledSize.width(), scaledSize.height());
        }
        painter.drawPixmap(targetRect, m_pixmap);
    }

    // 绘制文本（对齐 + 边距）
    if (!m_text.isEmpty()) {
        QFontMetrics fm(font());
        QSize textSize = fm.size(Qt::TextSingleLine, m_text);

        // 计算水平位置
        int textX;
        int hMarginPixels = static_cast<int>(widgetRect.width() * m_hMargin);
        switch (m_hAlignment) {
        case HLeft:
            textX = hMarginPixels;
            break;
        case HRight:
            textX = widgetRect.width() - textSize.width() - hMarginPixels;
            break;
        case HCenter:
        default:
            textX = (widgetRect.width() - textSize.width()) / 2;
            break;
        }

        // 计算垂直位置
        int textY;
        int vMarginPixels = static_cast<int>(widgetRect.height() * m_vMargin);
        switch (m_vAlignment) {
        case VTop:
            textY = vMarginPixels;
            break;
        case VBottom:
            textY = widgetRect.height() - textSize.height() - vMarginPixels;
            break;
        case VCenter:
        default:
            textY = (widgetRect.height() - textSize.height()) / 2;
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

int uiWidget::heightForWidth(int width) const
{
    if (m_pixmap.isNull() || m_scaleMode == Stretch) {
        return width;  // 没有图像或拉伸模式时返回等宽等高
    }

    // 根据图像宽高比计算高度（保持宽高比模式）
    double aspectRatio = static_cast<double>(m_pixmap.height()) / m_pixmap.width();
    return static_cast<int>(width * aspectRatio);
}

bool uiWidget::hasHeightForWidth() const
{
    return true;
}

QSize uiWidget::sizeHint() const
{
    if (m_pixmap.isNull()) {
        return QWidget::sizeHint();
    }
    return m_pixmap.size();
}
