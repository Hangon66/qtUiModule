#include "uiSlideVerify.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>

uiSlideVerify::uiSlideVerify(QWidget *parent)
    : uiImageTextMixin<QWidget>(parent)
{
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
    // 默认最大高度 60px，防止布局过度拉伸
    setMaximumHeight(60);
}

void uiSlideVerify::setSliderColor(const QColor &color)
{
    m_sliderColor = color;
    update();
}

void uiSlideVerify::setSliderSize(const QSize &size)
{
    m_sliderSize = size;
    update();
}

void uiSlideVerify::setSliderAspectRatio(qreal ratio)
{
    m_sliderAspectRatio = qBound(0.1, ratio, 10.0);
    update();
}

void uiSlideVerify::setSliderRadius(int radius)
{
    m_sliderRadius = qMax(0, radius);
    update();
}

void uiSlideVerify::setSliderBorderColor(const QColor &color)
{
    m_sliderBorderColor = color;
    update();
}

void uiSlideVerify::setTrackColor(const QColor &color)
{
    m_trackColor = color;
    update();
}

void uiSlideVerify::setSliderIcon(const QString &path)
{
    m_sliderIcon = loadPixmapWithDpi(path);
    update();
}

void uiSlideVerify::setHintText(const QString &text)
{
    m_hintText = text;
    update();
}

void uiSlideVerify::setVerifiedText(const QString &text)
{
    m_verifiedText = text;
    update();
}

void uiSlideVerify::setProgressColor(const QColor &color)
{
    m_progressColor = color;
    update();
}

void uiSlideVerify::setVerifiedColor(const QColor &color)
{
    m_verifiedColor = color;
    update();
}

void uiSlideVerify::setThreshold(qreal ratio)
{
    m_threshold = qBound(0.0, ratio, 1.0);
}

void uiSlideVerify::reset()
{
    m_verified = false;
    m_currentPos = 0.0;
    m_dragging = false;
    update();
}

QRect uiSlideVerify::trackRect() const
{
    QRect r = rect();
    return QRect(
        r.x() + m_marginLeft,
        r.y() + m_marginTop,
        r.width() - m_marginLeft - m_marginRight,
        r.height() - m_marginTop - m_marginBottom
    );
}

QRect uiSlideVerify::sliderRect(const QRect &track) const
{
    int blockHeight = m_sliderSize.height();
    int blockWidth = m_sliderSize.width();

    // 高度默认等于轨道高度
    if (blockHeight <= 0) blockHeight = track.height();

    // 宽度计算：宽高比优先于 setSliderSize 设置的宽度
    if (m_sliderAspectRatio > 0) {
        blockWidth = static_cast<int>(blockHeight * m_sliderAspectRatio);
    } else if (blockWidth <= 0) {
        blockWidth = track.height();  // 默认正方形
    }

    // 限制宽高不超过轨道
    blockWidth = qMin(blockWidth, track.width());
    blockHeight = qMin(blockHeight, track.height());

    // 计算滑块左边缘 X 坐标（0.0~1.0 映射到可用滑动范围）
    int maxSlide = track.width() - blockWidth;
    int sliderX = track.x() + static_cast<int>(m_currentPos * maxSlide);
    int sliderY = track.y() + (track.height() - blockHeight) / 2;

    return QRect(sliderX, sliderY, blockWidth, blockHeight);
}

void uiSlideVerify::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QRect track = trackRect();

    // 圆角裁剪（对轨道和进度条生效）
    bool hasBorderRadius = m_borderRadiusTopLeft > 0 || m_borderRadiusTopRight > 0 ||
                           m_borderRadiusBottomRight > 0 || m_borderRadiusBottomLeft > 0;
    if (hasBorderRadius) {
        QPainterPath clipPath;
        int tl = m_borderRadiusTopLeft, tr = m_borderRadiusTopRight;
        int br = m_borderRadiusBottomRight, bl = m_borderRadiusBottomLeft;
        clipPath.moveTo(track.x() + tl, track.y());
        clipPath.lineTo(track.x() + track.width() - tr, track.y());
        if (tr > 0) clipPath.arcTo(track.x() + track.width() - 2*tr, track.y(), 2*tr, 2*tr, 90, -90);
        else clipPath.lineTo(track.x() + track.width(), track.y());
        clipPath.lineTo(track.x() + track.width(), track.y() + track.height() - br);
        if (br > 0) clipPath.arcTo(track.x() + track.width() - 2*br, track.y() + track.height() - 2*br, 2*br, 2*br, 0, -90);
        else clipPath.lineTo(track.x() + track.width(), track.y() + track.height());
        clipPath.lineTo(track.x() + bl, track.y() + track.height());
        if (bl > 0) clipPath.arcTo(track.x(), track.y() + track.height() - 2*bl, 2*bl, 2*bl, 270, -90);
        else clipPath.lineTo(track.x(), track.y() + track.height());
        clipPath.lineTo(track.x(), track.y() + tl);
        if (tl > 0) clipPath.arcTo(track.x(), track.y(), 2*tl, 2*tl, 180, -90);
        else clipPath.lineTo(track.x(), track.y());
        clipPath.closeSubpath();
        painter.setClipPath(clipPath);
    }

    // 1. 绘制轨道背景图片
    if (!m_pixmap.isNull()) {
        painter.drawPixmap(track, scaledPixmapForTarget(m_pixmap, track.size()));
    }

    // 2. 绘制轨道背景颜色（未划过区域底色）
    if (m_trackColor.isValid()) {
        painter.fillRect(track, m_trackColor);
    }

    // 3. 绘制轨道背景图片（覆盖在底色之上，如果有设置）
    if (!m_pixmap.isNull()) {
        painter.drawPixmap(track, scaledPixmapForTarget(m_pixmap, track.size()));
    }

    // 4. 绘制进度条
    QRect sliderR = sliderRect(track);
    int progressWidth = sliderR.center().x() - track.x();
    if (progressWidth > 0) {
        QRect progressRect(track.x(), track.y(), progressWidth, track.height());
        QColor progressColor = m_verified ? m_verifiedColor : m_progressColor;
        if (progressColor.isValid()) {
            painter.fillRect(progressRect, progressColor);
        }
    }

    // 5. 绘制提示文字（居中于轨道）
    QString displayText = m_verified ? m_verifiedText : m_hintText;
    if (!displayText.isEmpty()) {
        painter.setFont(font());
        QColor textColor = m_textColor.isValid() ? m_textColor : getDefaultTextColor();
        painter.setPen(textColor);
        painter.drawText(track, Qt::AlignCenter, displayText);
    }

    // 取消裁剪，以便滑块块可以略微超出轨道（如果有边距差异）
    painter.setClipping(false);

    // 6. 绘制滑块块（圆角填充 + 边框）
    if (m_sliderColor.isValid() || m_sliderBorderColor.isValid()) {
        qreal r = m_sliderRadius > 0 ? m_sliderRadius : 0;
        // 边框 1px 居中描边，rect 内缩 0.5px 避免半像素溢出控件边界
        QRectF borderRect = QRectF(sliderR).adjusted(0.5, 0.5, -0.5, -0.5);

        QPainterPath sliderPath;
        sliderPath.addRoundedRect(borderRect, r, r);

        // 填充（与边框使用同一 rect，保证圆角完全重合）
        if (m_sliderColor.isValid()) {
            painter.fillPath(sliderPath, m_sliderColor);
        }

        // 边框
        if (m_sliderBorderColor.isValid()) {
            QPen pen(m_sliderBorderColor, 1);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(borderRect, r, r);
        }
    }

    // 7. 绘制滑块块图标（居中，保持原图比例）
    if (!m_sliderIcon.isNull()) {
        QSize iconSize = m_sliderIcon.size();
        // 等比例缩放，不超过滑块块大小（留 4px 边距）
        iconSize.scale(sliderR.width() - 4, sliderR.height() - 4, Qt::KeepAspectRatio);
        int iconX = sliderR.x() + (sliderR.width() - iconSize.width()) / 2;
        int iconY = sliderR.y() + (sliderR.height() - iconSize.height()) / 2;
        QRect iconRect(iconX, iconY, iconSize.width(), iconSize.height());
        painter.drawPixmap(iconRect, scaledPixmapForTarget(m_sliderIcon, iconRect.size()));
    }
}

void uiSlideVerify::mousePressEvent(QMouseEvent *event)
{
    if (m_verified) return;

    QRect track = trackRect();
    QRect slider = sliderRect(track);

    if (slider.contains(event->pos())) {
        m_dragging = true;
        m_pressOffset = event->pos().x() - slider.x();
    }
}

void uiSlideVerify::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging) return;

    QRect track = trackRect();
    int blockWidth = sliderRect(track).width();
    int maxSlide = track.width() - blockWidth;
    if (maxSlide <= 0) return;

    int newX = event->pos().x() - m_pressOffset - track.x();
    m_currentPos = qBound(0.0, static_cast<qreal>(newX) / maxSlide, 1.0);
    update();
}

void uiSlideVerify::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (!m_dragging) return;
    m_dragging = false;

    if (m_currentPos >= m_threshold) {
        // 验证通过：滑块吸附到终点
        m_currentPos = 1.0;
        m_verified = true;
        emit verified();
    } else {
        // 回弹：滑块回到起点
        m_currentPos = 0.0;
    }
    update();
}

QSize uiSlideVerify::sizeHint() const
{
    return QSize(300, 40);
}

QSize uiSlideVerify::minimumSizeHint() const
{
    return QSize(100, 30);
}

// ==================== Mixin 虚方法实现 ====================

QString uiSlideVerify::getText() const
{
    return m_verified ? m_verifiedText : m_hintText;
}

QColor uiSlideVerify::getDefaultTextColor() const
{
    return QColor(80, 80, 80);  // #505050
}

QSize uiSlideVerify::getBaseSizeHint() const
{
    return QWidget::sizeHint();
}
