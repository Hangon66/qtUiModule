#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QColor>

class uiWidget : public QWidget
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

    explicit uiWidget(QWidget *parent = nullptr);

    // ==================== 图像设置 ====================

    /**
     * @brief 设置背景图像路径。
     *
     * @param imagePath 图像文件路径。
     */
    void setImage(const QString &imagePath);

    /**
     * @brief 设置背景 QPixmap 图像。
     *
     * @param pixmap 要显示的 QPixmap 对象。
     */
    void setImage(const QPixmap &pixmap);

    /**
     * @brief 清除背景图像。
     */
    void clearImage();

    // ==================== 文本对齐 ====================

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

    // ==================== 文本边距 ====================

    /**
     * @brief 设置水平边距比例。
     *
     * 仅在水平左对齐或右对齐时生效。
     * 左对齐时为左边距，右对齐时为右边距。
     * 居中时忽略此设置。
     *
     * @param ratio 边距比例，范围0.0~1.0，相对于控件宽度。
     */
    void setHorizontalMargin(qreal ratio);

    /**
     * @brief 设置垂直边距比例。
     *
     * 仅在垂直上对齐或下对齐时生效。
     * 上对齐时为上边距，下对齐时为下边距。
     * 居中时忽略此设置。
     *
     * @param ratio 边距比例，范围0.0~1.0，相对于控件高度。
     */
    void setVerticalMargin(qreal ratio);

    // ==================== 文本样式 ====================

    /**
     * @brief 设置文本内容。
     *
     * @param text 要显示的文本。
     */
    void setText(const QString &text);

    /**
     * @brief 获取文本内容。
     *
     * @return 当前文本。
     */
    QString text() const;

    /**
     * @brief 设置文本字号（点阵）。
     *
     * @param pointSize 字号，单位为点。
     */
    void setFontSize(int pointSize);

    /**
     * @brief 设置文本字号（像素）。
     *
     * @param pixelSize 字号，单位为像素。
     */
    void setFontPixelSize(int pixelSize);

    /**
     * @brief 设置文本颜色。
     *
     * @param color 文本颜色。
     */
    void setTextColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    int heightForWidth(int width) const override;
    bool hasHeightForWidth() const override;
    QSize sizeHint() const override;

private:
    QPixmap m_pixmap;                          ///< 背景图像

    QString m_text;                            ///< 文本内容
    HorizontalAlignment m_hAlignment = HCenter; ///< 水平对齐模式，默认居中
    VerticalAlignment m_vAlignment = VCenter;   ///< 垂直对齐模式，默认居中
    qreal m_hMargin = 0.0;                      ///< 水平边距比例 (0.0 ~ 1.0)
    qreal m_vMargin = 0.0;                      ///< 垂直边距比例 (0.0 ~ 1.0)
    QColor m_textColor;                         ///< 文本颜色（空则使用默认）
};

#endif // UIWIDGET_H
