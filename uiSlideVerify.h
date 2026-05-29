#ifndef UISLIDEVERIFY_H
#define UISLIDEVERIFY_H

#include "uiImageTextMixin.h"

/**
 * @brief 滑动验证滑块组件。
 *
 * 继承 uiImageTextMixin<QWidget>，提供网页端常见的滑动解锁验证功能。
 * 用户从左向右拖动滑块，到达阈值位置即验证通过。
 * 支持自定义轨道背景、滑块外观、提示文字和进度条颜色。
 */
class uiSlideVerify : public uiImageTextMixin<QWidget>
{
    Q_OBJECT

public:
    explicit uiSlideVerify(QWidget *parent = nullptr);

    // ==================== 滑块块样式 ====================

    /**
     * @brief 设置滑块块颜色。
     *
     * @param color 滑块块填充颜色。
     */
    void setSliderColor(const QColor &color);

    /**
     * @brief 设置滑块块大小。
     *
     * 宽度为固定值，高度在绘制时自动适配轨道高度。
     * 若未设置，默认宽度等于控件高度。
     *
     * @param size 滑块块尺寸（宽度生效，高度仅供参考）。
     */
    void setSliderSize(const QSize &size);

    /**
     * @brief 设置滑块块宽高比例。
     *
     * 设置后，滑块块宽度将根据轨道高度和此比例自动计算。
     * 优先级高于 setSliderSize 设置的宽度。
     * 例如 ratio=1.2 表示宽度是高度的 1.2 倍。
     *
     * @param ratio 宽高比（width/height），范围 0.1~10.0，默认 1.0（正方形）。
     */
    void setSliderAspectRatio(qreal ratio);

    /**
     * @brief 设置滑块块圆角半径。
     *
     * @param radius 圆角半径，单位为像素。
     */
    void setSliderRadius(int radius);

    /**
     * @brief 设置滑块块边框颜色。
     *
     * 边框宽度固定 1px，围绕滑块块圆角矩形绘制。
     *
     * @param color 边框颜色。
     */
    void setSliderBorderColor(const QColor &color);

    /**
     * @brief 设置滑块块图标。
     *
     * 图标居中绘制在滑块块上。
     *
     * @param path 图标资源路径。
     */
    void setSliderIcon(const QString &path);

    // ==================== 提示文字 ====================

    /**
     * @brief 设置提示文字。
     *
     * 验证未通过时显示在轨道中央的文字，如"请向右滑动"。
     *
     * @param text 提示文字内容。
     */
    void setHintText(const QString &text);

    /**
     * @brief 设置验证成功后的提示文字。
     *
     * 验证通过后显示的文字，默认为"验证成功"。
     *
     * @param text 验证成功文字内容。
     */
    void setVerifiedText(const QString &text);

    // ==================== 轨道背景 ====================

    /**
     * @brief 设置轨道背景颜色。
     *
     * 滑块未划过的区域填充此颜色，默认 #e9e9e9。
     *
     * @param color 轨道背景颜色。
     */
    void setTrackColor(const QColor &color);

    // ==================== 进度条 ====================

    /**
     * @brief 设置进度条颜色。
     *
     * 拖动过程中，滑块左侧区域的填充颜色。
     *
     * @param color 进度条颜色。
     */
    void setProgressColor(const QColor &color);

    /**
     * @brief 设置验证成功颜色。
     *
     * 验证通过后，进度条区域的颜色。
     *
     * @param color 验证成功颜色。
     */
    void setVerifiedColor(const QColor &color);

    // ==================== 阈值 ====================

    /**
     * @brief 设置验证阈值。
     *
     * 滑块需要拖动到的位置比例，超过此值即验证通过。
     *
     * @param ratio 阈值比例，范围 0.0~1.0，默认 1.0（需滑到底部）。
     */
    void setThreshold(qreal ratio);

    /**
     * @brief 获取验证阈值。
     *
     * @return 当前阈值比例。
     */
    qreal threshold() const { return m_threshold; }

    // ==================== 状态控制 ====================

    /**
     * @brief 获取是否已验证通过。
     *
     * @return true 表示已验证，false 表示未验证。
     */
    bool isVerified() const { return m_verified; }

    /**
     * @brief 重置为未验证状态。
     *
     * 滑块回到起始位置，提示文字恢复为未验证状态。
     */
    void reset();

signals:
    /**
     * @brief 验证通过信号。
     *
     * 滑块拖动超过阈值后发出此信号。
     */
    void verified();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // ==================== Mixin 虚方法实现 ====================

    QString getText() const override;
    QColor getDefaultTextColor() const override;
    QSize getBaseSizeHint() const override;

private:
    /**
     * @brief 滑块块填充颜色。
     */
    QColor m_sliderColor = QColor(255, 255, 255);

    /**
     * @brief 滑块块大小（宽度生效，高度自动适配轨道）。
     */
    QSize m_sliderSize;

    /**
     * @brief 滑块块宽高比（width/height），默认 1.0（正方形）。
     *
     * 设置后宽度根据轨道高度自动计算，优先级高于 m_sliderSize 的宽度。
     */
    qreal m_sliderAspectRatio = 1.0;

    /**
     * @brief 滑块块圆角半径。
     */
    int m_sliderRadius = 0;

    /**
     * @brief 滑块块边框颜色。
     */
    QColor m_sliderBorderColor = QColor(172, 172, 172);

    /**
     * @brief 轨道背景颜色（未划过区域），默认 #e9e9e9。
     */
    QColor m_trackColor = QColor(233, 233, 233);

    /**
     * @brief 滑块块图标（居中绘制）。
     */
    QPixmap m_sliderIcon;

    /**
     * @brief 进度条颜色（拖动中）。
     */
    QColor m_progressColor = QColor(82, 196, 26);

    /**
     * @brief 验证成功后的进度条颜色。
     */
    QColor m_verifiedColor = QColor(82, 196, 26);

    /**
     * @brief 未验证时的提示文字。
     */
    QString m_hintText;

    /**
     * @brief 验证成功后的提示文字。
     */
    QString m_verifiedText = QString::fromUtf8("验证成功");

    /**
     * @brief 验证阈值比例（0.0~1.0），默认 1.0。
     */
    qreal m_threshold = 1.0;

    /**
     * @brief 当前滑块位置比例（0.0~1.0）。
     */
    qreal m_currentPos = 0.0;

    /**
     * @brief 是否正在拖动滑块。
     */
    bool m_dragging = false;

    /**
     * @brief 按下时鼠标相对于滑块左边缘的像素偏移。
     */
    int m_pressOffset = 0;

    /**
     * @brief 是否已验证通过。
     */
    bool m_verified = false;

    /**
     * @brief 计算轨道矩形区域。
     *
     * @return 减去外边距后的轨道区域。
     */
    QRect trackRect() const;

    /**
     * @brief 计算滑块块矩形区域。
     *
     * @param track 轨道矩形。
     * @return 当前滑块块所在矩形。
     */
    QRect sliderRect(const QRect &track) const;
};

#endif // UISLIDEVERIFY_H
