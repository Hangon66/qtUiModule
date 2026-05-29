#ifndef UILABEL_H
#define UILABEL_H

#include "uiImageTextMixin.h"

class uiLabel : public uiImageTextMixin<QLabel>
{
    Q_OBJECT

public:
    explicit uiLabel(QWidget *parent = nullptr);

    /**
     * @brief 设置外边距（统一）。
     *
     * 外边距是控件与其他控件之间的距离。
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
};

#endif // UILABEL_H
