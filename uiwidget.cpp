#include "uiwidget.h"
#include <QPainter>

uiWidget::uiWidget(QWidget *parent)
    : uiImageTextMixin<QWidget>(parent)
{
    // 默认：控件最小为图像大小，可以更大
    setImageSizeMode(MinimumToImage);
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
    // 图像影响尺寸时，返回图像大小作为最小尺寸
    if (!image().isNull() && imageAffectsSizeHint()) {
        return image().size();
    }
    return QSize(0, 0);
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
