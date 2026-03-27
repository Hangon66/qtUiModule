#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "uiImageTextMixin.h"

class uiWidget : public uiImageTextMixin<QWidget>
{
    Q_OBJECT

public:
    explicit uiWidget(QWidget *parent = nullptr);

    // ==================== 文本内容（Widget 特有）====================

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

    // ==================== 外边距设置 ====================

    /**
     * @brief 设置外边距（统一）。
     *
     * @param margin 外边距值，单位为像素。
     */
    void setMargin(int margin);

    /**
     * @brief 设置外边距（四边独立）。
     *
     * @param left 左外边距。
     * @param top 上外边距。
     * @param right 右外边距。
     * @param bottom 下外边距。
     */
    void setMargin(int left, int top, int right, int bottom);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;

    // ==================== Mixin 虚方法实现 ====================

    QString getText() const override;
    QColor getDefaultTextColor() const override;
    QSize getBaseSizeHint() const override;

private:
    QString m_text;  ///< 文本内容
};

#endif // UIWIDGET_H
