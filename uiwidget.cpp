#include "uiwidget.h"
#include <QPainter>

uiWidget::uiWidget(QWidget *parent)
    : uiImageTextMixin<QWidget>(parent)
{
    // Minimum 策略：控件至少为 sizeHint（图像大小），可以更大
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(0, 0);
}

void uiWidget::setText(const QString &text)
{
    m_text = text;
    update();
}

QString uiWidget::text() const
{
    return m_text;
}

void uiWidget::setMargin(int margin)
{
    m_marginLeft = m_marginTop = m_marginRight = m_marginBottom = qMax(0, margin);
    updateGeometry();
}

void uiWidget::setMargin(int left, int top, int right, int bottom)
{
    m_marginLeft = qMax(0, left);
    m_marginTop = qMax(0, top);
    m_marginRight = qMax(0, right);
    m_marginBottom = qMax(0, bottom);
    updateGeometry();
}

void uiWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    paintImageAndText();
}

QSize uiWidget::minimumSizeHint() const
{
    return QSize(0, 0);  // 允许任意压缩
}

// ==================== Mixin 虚方法实现 ====================

QString uiWidget::getText() const
{
    return m_text;
}

QColor uiWidget::getDefaultTextColor() const
{
    return palette().color(QPalette::WindowText);
}

QSize uiWidget::getBaseSizeHint() const
{
    return QWidget::sizeHint();
}
