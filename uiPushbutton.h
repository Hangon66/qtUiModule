#ifndef uiPushbutton_H
#define uiPushbutton_H

#include <QObject>
#include <QPushButton>
#include <QPixmap>
#include <QPaintEvent>
#include <QColor>
#include <QEnterEvent>
#include <QEvent>

class uiPushbutton : public QPushButton
{
    Q_OBJECT

public:
    /**
     * @brief 水平对齐模式。
     */
    enum HorizontalAlignment { HLeft, HCenter, HRight };

    /**
     * @brief 垂直对齐模式。
     */
    enum VerticalAlignment { VTop, VCenter, VBottom };

    explicit uiPushbutton(QWidget *parent = nullptr);
    
    // 设置图片路径
    void setImage(const QString &imagePath);
    // 设置 QPixmap
    void setImage(const QPixmap &pixmap);
    // 清除图片
    void clearImage();
        
    // 设置悬浮状态图片
    void setHoverImage(const QString &imagePath);
    void setHoverImage(const QPixmap &pixmap);
    // 设置点击状态图片
    void setPressedImage(const QString &imagePath);
    void setPressedImage(const QPixmap &pixmap);
    // 清除状态图片
    void clearStateImages();
    
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
        
    /**
     * @brief 设置水平对齐模式。
     *
     * @param align 水平对齐方式：HLeft（左对齐）、HCenter（居中）、HRight（右对齐）。
     */
    void setHorizontalAlignment(HorizontalAlignment align);

    /**
     * @brief 设置垂直对齐模式。
     *
     * @param align 垂直对齐方式：VTop（上对齐）、VCenter（居中）、VBottom（下对齐）。
     */
    void setVerticalAlignment(VerticalAlignment align);

    /**
     * @brief 设置水平边距比例。
     *
     * 仅在水平左对齐或右对齐时生效。
     * 左对齐时为左边距，右对齐时为右边距。
     * 居中时忽略此设置。
     *
     * @param ratio 边距比例，范围0.0~1.0，相对于按钮宽度。
     */
    void setHorizontalMargin(qreal ratio);

    /**
     * @brief 设置垂直边距比例。
     *
     * 仅在垂直上对齐或下对齐时生效。
     * 上对齐时为上边距，下对齐时为下边距。
     * 居中时忽略此设置。
     *
     * @param ratio 边距比例，范围0.0~1.0，相对于按钮高度。
     */
    void setVerticalMargin(qreal ratio);

    // 设置文本字号（点阵）
    void setFontSize(int pointSize);
    // 设置文本字号（像素）
    void setFontPixelSize(int pixelSize);
    // 设置文本颜色
    void setTextColor(const QColor &color);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    int heightForWidth(int width) const override;
    bool hasHeightForWidth() const override;
    QSize sizeHint() const override;

private:
    QPixmap m_pixmap;           // 存储原始图片（默认状态）
    QPixmap m_hoverPixmap;      // 悬浮状态图片
    QPixmap m_pressedPixmap;    // 点击状态图片
    qreal m_hOffsetRatio = 0.0; ///< 水平偏移比例 (-1.0 ~ 1.0)
    qreal m_vOffsetRatio = 0.0; ///< 垂直偏移比例 (-1.0 ~ 1.0)

    HorizontalAlignment m_hAlignment = HCenter; ///< 水平对齐模式，默认居中
    VerticalAlignment m_vAlignment = VCenter;   ///< 垂直对齐模式，默认居中
    qreal m_hMargin = 0.0;                     ///< 水平边距比例 (0.0 ~ 1.0)
    qreal m_vMargin = 0.0;                     ///< 垂直边距比例 (0.0 ~ 1.0)
    QColor m_textColor;         // 文本颜色（空则使用默认）
};

#endif // uiPushbutton_H
