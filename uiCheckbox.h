#ifndef UICHECKBOX_H
#define UICHECKBOX_H

#include "uiImageTextMixin.h"

/**
 * @brief 自定义复选框组件。
 *
 * 支持设置未选中和选中两种状态下的图片，
 * 每种状态包含正常、悬浮、按下三种视觉效果。
 * 继承自 uiImageTextMixin，具备图像文本混合显示能力。
 */
class uiCheckbox : public uiImageTextMixin<QWidget>
{
    Q_OBJECT

public:
    explicit uiCheckbox(QWidget *parent = nullptr);

    // ==================== 复选框状态 ====================

    /**
     * @brief 设置复选框的选中状态。
     *
     * @param checked true 表示选中，false 表示未选中。
     */
    void setChecked(bool checked);

    /**
     * @brief 获取当前选中状态。
     *
     * @return true 表示已选中，false 表示未选中。
     */
    bool isChecked() const { return m_checked; }

    /**
     * @brief 切换选中状态。
     */
    void toggle();

    // ==================== 未选中状态图片设置 ====================

    /**
     * @brief 设置未选中状态的默认图片。
     *
     * 设置后会自动生成悬浮（变亮）和按下（变暗）状态图片。
     *
     * @param imagePath 未选中状态图像路径。
     */
    void setUncheckedImage(const QString &imagePath);

    /**
     * @brief 设置未选中状态的默认图片。
     *
     * 设置后会自动生成悬浮（变亮）和按下（变暗）状态图片。
     *
     * @param pixmap 未选中状态 QPixmap 对象。
     */
    void setUncheckedImage(const QPixmap &pixmap);

    /**
     * @brief 设置未选中状态的悬浮图片。
     *
     * @param imagePath 悬浮状态图像路径。
     */
    void setUncheckedHoverImage(const QString &imagePath);

    /**
     * @brief 设置未选中状态的悬浮图片。
     *
     * @param pixmap 悬浮状态 QPixmap 对象。
     */
    void setUncheckedHoverImage(const QPixmap &pixmap);

    /**
     * @brief 设置未选中状态的按下图片。
     *
     * @param imagePath 按下状态图像路径。
     */
    void setUncheckedPressedImage(const QString &imagePath);

    /**
     * @brief 设置未选中状态的按下图片。
     *
     * @param pixmap 按下状态 QPixmap 对象。
     */
    void setUncheckedPressedImage(const QPixmap &pixmap);

    // ==================== 选中状态图片设置 ====================

    /**
     * @brief 设置选中状态的默认图片。
     *
     * 设置后会自动生成悬浮（变亮）和按下（变暗）状态图片。
     *
     * @param imagePath 选中状态图像路径。
     */
    void setCheckedImage(const QString &imagePath);

    /**
     * @brief 设置选中状态的默认图片。
     *
     * 设置后会自动生成悬浮（变亮）和按下（变暗）状态图片。
     *
     * @param pixmap 选中状态 QPixmap 对象。
     */
    void setCheckedImage(const QPixmap &pixmap);

    /**
     * @brief 设置选中状态的悬浮图片。
     *
     * @param imagePath 悬浮状态图像路径。
     */
    void setCheckedHoverImage(const QString &imagePath);

    /**
     * @brief 设置选中状态的悬浮图片。
     *
     * @param pixmap 悬浮状态 QPixmap 对象。
     */
    void setCheckedHoverImage(const QPixmap &pixmap);

    /**
     * @brief 设置选中状态的按下图片。
     *
     * @param imagePath 按下状态图像路径。
     */
    void setCheckedPressedImage(const QString &imagePath);

    /**
     * @brief 设置选中状态的按下图片。
     *
     * @param pixmap 按下状态 QPixmap 对象。
     */
    void setCheckedPressedImage(const QPixmap &pixmap);

    // ==================== 自动生成状态图片 ====================

    /**
     * @brief 设置是否自动生成状态图片。
     *
     * @param enabled true 为自动生成（默认），false 为手动设置。
     */
    void setAutoStateImages(bool enabled);

    /**
     * @brief 获取是否自动生成状态图片。
     */
    bool autoStateImages() const { return m_autoStateImages; }

    /**
     * @brief 清除所有状态图片。
     */
    void clearStateImages();

    // ==================== 文本设置 ====================

    /**
     * @brief 设置复选框文本。
     *
     * @param text 要显示的文本内容。
     */
    void setText(const QString &text);

    /**
     * @brief 获取复选框文本。
     */
    QString text() const { return m_text; }

signals:
    /**
     * @brief 选中状态改变时发出的信号。
     *
     * @param checked 新的选中状态。
     */
    void stateChanged(bool checked);

    /**
     * @brief 复选框被点击时发出的信号。
     */
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize minimumSizeHint() const override;

    // ==================== Mixin 虚方法实现 ====================

    QString getText() const override;
    QColor getDefaultTextColor() const override;
    QSize getBaseSizeHint() const override;

private:
    // 状态标志
    bool m_checked = false;            ///< 当前选中状态
    bool m_pressed = false;            ///< 当前是否处于按下状态
    bool m_hovered = false;            ///< 当前是否处于悬浮状态
    bool m_autoStateImages = true;     ///< 是否自动生成状态图片

    // 未选中状态图片
    QPixmap m_uncheckedPixmap;         ///< 未选中默认图片
    QPixmap m_uncheckedHoverPixmap;    ///< 未选中悬浮图片
    QPixmap m_uncheckedPressedPixmap;  ///< 未选中按下图片

    // 选中状态图片
    QPixmap m_checkedPixmap;           ///< 选中默认图片
    QPixmap m_checkedHoverPixmap;      ///< 选中悬浮图片
    QPixmap m_checkedPressedPixmap;    ///< 选中按下图片

    // 文本
    QString m_text;                    ///< 复选框文本

    /**
     * @brief 获取当前应显示的图片。
     *
     * 根据当前状态（选中/未选中、悬浮、按下）返回对应的图片。
     *
     * @return 当前状态对应的 QPixmap。
     */
    QPixmap currentPixmap() const;

    /**
     * @brief 自动生成未选中状态的悬浮和按下图片。
     */
    void generateUncheckedStateImages();

    /**
     * @brief 自动生成选中状态的悬浮和按下图片。
     */
    void generateCheckedStateImages();

    /**
     * @brief 根据基础图片生成变亮和变暗的状态图片。
     *
     * @param basePixmap 基础图片。
     * @param hoverPixmap 输出的悬浮图片。
     * @param pressedPixmap 输出的按下图片。
     */
    void generateStateImagesFromBase(const QPixmap &basePixmap, 
                                      QPixmap &hoverPixmap, 
                                      QPixmap &pressedPixmap);
};

#endif // UICHECKBOX_H
