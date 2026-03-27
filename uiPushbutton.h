#ifndef uiPushbutton_H
#define uiPushbutton_H

#include "uiImageTextMixin.h"

class uiPushbutton : public uiImageTextMixin<QPushButton>
{
    Q_OBJECT

public:
    explicit uiPushbutton(QWidget *parent = nullptr);

    // ==================== 状态图片设置（PushButton 特有）====================

    /**
     * @brief 设置悬浮状态图片。
     *
     * @param imagePath 悬浮状态图像路径。
     */
    void setHoverImage(const QString &imagePath);

    /**
     * @brief 设置悬浮状态 QPixmap 图片。
     *
     * @param pixmap 悬浮状态 QPixmap 对象。
     */
    void setHoverImage(const QPixmap &pixmap);

    /**
     * @brief 设置点击状态图片。
     *
     * @param imagePath 点击状态图像路径。
     */
    void setPressedImage(const QString &imagePath);

    /**
     * @brief 设置点击状态 QPixmap 图片。
     *
     * @param pixmap 点击状态 QPixmap 对象。
     */
    void setPressedImage(const QPixmap &pixmap);

    /**
     * @brief 清除状态图片。
     */
    void clearStateImages();

    // ==================== 文本偏移（PushButton 特有）====================

    /**
     * @brief 设置文本偏移比例 (-1.0 ~ 1.0, 0为居中)。
     *
     * @param horizontalRatio 水平偏移比例。
     * @param verticalRatio 垂直偏移比例。
     */
    void setTextOffset(qreal horizontalRatio, qreal verticalRatio);

    /**
     * @brief 设置水平偏移比例 (-1.0 ~ 1.0)。
     */
    void setHorizontalOffset(qreal ratio);

    /**
     * @brief 设置垂直偏移比例 (-1.0 ~ 1.0)。
     */
    void setVerticalOffset(qreal ratio);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize minimumSizeHint() const override;

    // ==================== Mixin 虚方法实现 ====================

    QString getText() const override;
    QColor getDefaultTextColor() const override;
    QSize getBaseSizeHint() const override;

private:
    QPixmap m_hoverPixmap;      ///< 悬浮状态图片
    QPixmap m_pressedPixmap;    ///< 点击状态图片
    qreal m_hOffsetRatio = 0.0; ///< 水平偏移比例 (-1.0 ~ 1.0)
    qreal m_vOffsetRatio = 0.0; ///< 垂直偏移比例 (-1.0 ~ 1.0)
};

#endif // uiPushbutton_H
