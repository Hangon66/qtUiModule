#include "uilabel.h"
#include <QPainter>

uiLabel::uiLabel(QWidget *parent)
    : uiImageTextMixin<QLabel>(parent)
{
    // 默认：控件最小为图像大小，可以更大
    setImageSizeMode(MinimumToImage);
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
    // 图像影响尺寸时，返回图像大小作为最小尺寸
    if (!image().isNull() && imageAffectsSizeHint()) {
        return image().size();
    }
    // 无图像或图像不影响尺寸时，返回基类的 minimumSizeHint
    return QLabel::minimumSizeHint();
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
