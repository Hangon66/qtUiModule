#include "uiPushbutton.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QDebug>

uiPushbutton::uiPushbutton(QWidget *parent)
    : QPushButton(parent)
{
    // 设置透明背景
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    setFlat(true);  // 扁平化按钮，去除边框
    setMouseTracking(true);  // 启用鼠标追踪，用于悬浮状态检测
}

void uiPushbutton::setImage(const QString &imagePath)
{
    m_pixmap = QPixmap(imagePath);
    if (m_pixmap.isNull()) {
        qWarning() << "[uiPushbutton] 图片加载失败:" << imagePath;
    } else {
        //qDebug() << "[uiPushbutton] 图片加载成功:" << imagePath << "size:" << m_pixmap.size();
    }
    update();  // 触发重绘
}

void uiPushbutton::setImage(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void uiPushbutton::clearImage()
{
    m_pixmap = QPixmap();
    update();
}

void uiPushbutton::setHoverImage(const QString &imagePath)
{
    m_hoverPixmap = QPixmap(imagePath);
    update();
}

void uiPushbutton::setHoverImage(const QPixmap &pixmap)
{
    m_hoverPixmap = pixmap;
    update();
}

void uiPushbutton::setPressedImage(const QString &imagePath)
{
    m_pressedPixmap = QPixmap(imagePath);
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

void uiPushbutton::setFontSize(int pointSize)
{
    QFont f = font();
    f.setPointSize(pointSize);
    setFont(f);
    update();
}

void uiPushbutton::setFontPixelSize(int pixelSize)
{
    QFont f = font();
    f.setPixelSize(pixelSize);
    setFont(f);
    update();
}

void uiPushbutton::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void uiPushbutton::setHorizontalAlignment(HorizontalAlignment align)
{
    m_hAlignment = align;
    update();
}

void uiPushbutton::setVerticalAlignment(VerticalAlignment align)
{
    m_vAlignment = align;
    update();
}

void uiPushbutton::setHorizontalMargin(qreal ratio)
{
    m_hMargin = qBound(0.0, ratio, 1.0);
    update();
}

void uiPushbutton::setVerticalMargin(qreal ratio)
{
    m_vMargin = qBound(0.0, ratio, 1.0);
    update();
}

void uiPushbutton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    
    QRect buttonRect = rect();
    
    // 根据状态选择图片：优先级 点击 > 选中 > 悬浮 > 默认
    QPixmap currentPixmap;
    if ((isDown() || isChecked()) && !m_pressedPixmap.isNull()) {
        currentPixmap = m_pressedPixmap;
    } else if (underMouse() && !isChecked() && !m_hoverPixmap.isNull()) {
        currentPixmap = m_hoverPixmap;
    } else {
        currentPixmap = m_pixmap;
    }
    
    // 如果有图片，绘制图片（保持宽高比，居中显示）
    if (!currentPixmap.isNull()) {
        // 计算保持宽高比的缩放后的矩形
        QSize scaledSize = currentPixmap.size();
        scaledSize.scale(buttonRect.size(), Qt::KeepAspectRatio);
        
        // 居中显示
        int x = (buttonRect.width() - scaledSize.width()) / 2;
        int y = (buttonRect.height() - scaledSize.height()) / 2;
        QRect targetRect(x, y, scaledSize.width(), scaledSize.height());
        
        painter.drawPixmap(targetRect, currentPixmap);
    }
    
    // 绘制文本（对齐 + 边距 + 偏移）
    QString btnText = text();
    if (!btnText.isEmpty()) {
        QFontMetrics fm(font());
        QSize textSize = fm.size(Qt::TextSingleLine, btnText);
        
        // 计算水平位置
        int textX;
        int hMarginPixels = static_cast<int>(buttonRect.width() * m_hMargin);
        switch (m_hAlignment) {
        case HLeft:
            textX = hMarginPixels;
            break;
        case HRight:
            textX = buttonRect.width() - textSize.width() - hMarginPixels;
            break;
        case HCenter:
        default:
            textX = (buttonRect.width() - textSize.width()) / 2;
            break;
        }
        
        // 计算垂直位置
        int textY;
        int vMarginPixels = static_cast<int>(buttonRect.height() * m_vMargin);
        switch (m_vAlignment) {
        case VTop:
            textY = vMarginPixels;
            break;
        case VBottom:
            textY = buttonRect.height() - textSize.height() - vMarginPixels;
            break;
        case VCenter:
        default:
            textY = (buttonRect.height() - textSize.height()) / 2;
            break;
        }
        
        // 应用偏移比例 (-1.0 ~ 1.0 映射到按钮宽高，用于微调)
        int maxHOffset = (buttonRect.width() - textSize.width()) / 2;
        int maxVOffset = (buttonRect.height() - textSize.height()) / 2;
        textX += static_cast<int>(maxHOffset * m_hOffsetRatio);
        textY += static_cast<int>(maxVOffset * m_vOffsetRatio);
        
        // 绘制文本
        QRect textRect(textX, textY, textSize.width(), textSize.height());
        painter.setFont(font());
        // 使用自定义颜色或默认颜色
        QColor textColor = m_textColor.isValid() ? m_textColor : palette().color(QPalette::ButtonText);
        painter.setPen(textColor);
        painter.drawText(textRect, Qt::AlignCenter, btnText);
    }
}

void uiPushbutton::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    if (!m_hoverPixmap.isNull()) {
        update();  // 有悬浮图片时需要重绘
    }
}

void uiPushbutton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (!m_hoverPixmap.isNull()) {
        update();  // 有悬浮图片时需要重绘
    }
}

int uiPushbutton::heightForWidth(int width) const
{
    if (m_pixmap.isNull()) {
        return width;  // 没有图片时返回等宽等高
    }
    
    // 根据图片宽高比计算高度
    double aspectRatio = static_cast<double>(m_pixmap.height()) / m_pixmap.width();
    return static_cast<int>(width * aspectRatio);
}

bool uiPushbutton::hasHeightForWidth() const
{
    return true;  // 高度依赖宽度，用于布局系统
}

QSize uiPushbutton::sizeHint() const
{
    if (m_pixmap.isNull()) {
        return QPushButton::sizeHint();
    }
    // 返回图片原始大小作为建议大小
    return m_pixmap.size();
}
