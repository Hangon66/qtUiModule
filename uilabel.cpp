#include "uilabel.h"
#include <QPainter>

uiLabel::uiLabel(QWidget *parent)
    : uiImageTextMixin<QLabel>(parent)
{
    // Minimum 策略：控件至少为 sizeHint（图像大小），可以更大
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(0, 0);
}

void uiLabel::setMargin(int margin)
{
    m_marginLeft = m_marginTop = m_marginRight = m_marginBottom = qMax(0, margin);
    updateGeometry();
}

void uiLabel::setMargin(int left, int top, int right, int bottom)
{
    m_marginLeft = qMax(0, left);
    m_marginTop = qMax(0, top);
    m_marginRight = qMax(0, right);
    m_marginBottom = qMax(0, bottom);
    updateGeometry();
}

void uiLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintImageAndText();
}

QSize uiLabel::minimumSizeHint() const
{
    return QSize(0, 0);  // 允许任意压缩
}

// ==================== Mixin 虚方法实现 ====================

QString uiLabel::getText() const
{
    return QLabel::text();
}

QColor uiLabel::getDefaultTextColor() const
{
    return palette().color(QPalette::WindowText);
}

QSize uiLabel::getBaseSizeHint() const
{
    return QLabel::sizeHint();
}
