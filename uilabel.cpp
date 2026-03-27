#include "uilabel.h"
#include <QPainter>
#include <QDebug>

uiLabel::uiLabel(QWidget *parent)
    : QLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
}

// ==================== 图像设置 ====================

void uiLabel::setImage(const QString &imagePath)
{
    m_pixmap = QPixmap(imagePath);
    if (m_pixmap.isNull()) {
        qWarning() << "[uiLabel] 图像加载失败:" << imagePath;
    }
    update();
}

void uiLabel::setImage(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void uiLabel::clearImage()
{
    m_pixmap = QPixmap();
    update();
}

// ==================== 文本对齐 ====================

void uiLabel::setHorizontalAlignment(HorizontalAlignment align)
{
    m_hAlignment = align;
    update();
}

void uiLabel::setVerticalAlignment(VerticalAlignment align)
{
    m_vAlignment = align;
    update();
}

// ==================== 文本边距 ====================

void uiLabel::setHorizontalMargin(qreal ratio)
{
    m_hMargin = qBound(0.0, ratio, 1.0);
    update();
}

void uiLabel::setVerticalMargin(qreal ratio)
{
    m_vMargin = qBound(0.0, ratio, 1.0);
    update();
}

// ==================== 文本样式 ====================

void uiLabel::setFontSize(int pointSize)
{
    QFont f = font();
    f.setPointSize(pointSize);
    setFont(f);
    update();
}

void uiLabel::setFontPixelSize(int pixelSize)
{
    QFont f = font();
    f.setPixelSize(pixelSize);
    setFont(f);
    update();
}

void uiLabel::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

// ==================== 绘制 ====================

void uiLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QRect labelRect = rect();

    // 绘制图像（保持宽高比，居中显示）
    if (!m_pixmap.isNull()) {
        QSize scaledSize = m_pixmap.size();
        scaledSize.scale(labelRect.size(), Qt::KeepAspectRatio);

        int x = (labelRect.width() - scaledSize.width()) / 2;
        int y = (labelRect.height() - scaledSize.height()) / 2;
        QRect targetRect(x, y, scaledSize.width(), scaledSize.height());

        painter.drawPixmap(targetRect, m_pixmap);
    }

    // 绘制文本（对齐 + 边距）
    QString labelText = text();
    if (!labelText.isEmpty()) {
        QFontMetrics fm(font());
        QSize textSize = fm.size(Qt::TextSingleLine, labelText);

        // 计算水平位置
        int textX;
        int hMarginPixels = static_cast<int>(labelRect.width() * m_hMargin);
        switch (m_hAlignment) {
        case HLeft:
            textX = hMarginPixels;
            break;
        case HRight:
            textX = labelRect.width() - textSize.width() - hMarginPixels;
            break;
        case HCenter:
        default:
            textX = (labelRect.width() - textSize.width()) / 2;
            break;
        }

        // 计算垂直位置
        int textY;
        int vMarginPixels = static_cast<int>(labelRect.height() * m_vMargin);
        switch (m_vAlignment) {
        case VTop:
            textY = vMarginPixels;
            break;
        case VBottom:
            textY = labelRect.height() - textSize.height() - vMarginPixels;
            break;
        case VCenter:
        default:
            textY = (labelRect.height() - textSize.height()) / 2;
            break;
        }

        // 绘制文本
        QRect textRect(textX, textY, textSize.width(), textSize.height());
        painter.setFont(font());
        QColor textColor = m_textColor.isValid() ? m_textColor : palette().color(QPalette::WindowText);
        painter.setPen(textColor);
        painter.drawText(textRect, Qt::AlignCenter, labelText);
    }
}

int uiLabel::heightForWidth(int width) const
{
    if (m_pixmap.isNull()) {
        return width;
    }

    double aspectRatio = static_cast<double>(m_pixmap.height()) / m_pixmap.width();
    return static_cast<int>(width * aspectRatio);
}

bool uiLabel::hasHeightForWidth() const
{
    return true;
}

QSize uiLabel::sizeHint() const
{
    if (m_pixmap.isNull()) {
        return QLabel::sizeHint();
    }
    return m_pixmap.size();
}
