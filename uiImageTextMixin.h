#ifndef UIIMAGETEXTMIXIN_H
#define UIIMAGETEXTMIXIN_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QFontMetrics>
#include <QDebug>

/**
 * @brief 图像文本混合模板类
 *
 * 提供图像显示和文本渲染的公共功能。
 * 通过 CRTP 模式实现 Mixin，继承此类即可获得图像文本功能。
 *
 * @tparam Base 基础控件类型（QWidget、QLabel 或 QPushButton）
 */
template<typename Base>
class uiImageTextMixin : public Base
{
public:
    // ==================== 枚举定义 ====================

    /**
     * @brief 水平对齐模式。
     */
    enum HorizontalAlignment { HLeft, HCenter, HRight };

    /**
     * @brief 垂直对齐模式。
     */
    enum VerticalAlignment { VTop, VCenter, VBottom };

    /**
     * @brief 图像缩放模式。
     */
    enum ImageScaleMode { KeepAspectRatio, Stretch };

    // ==================== 构造函数 ====================

    explicit uiImageTextMixin(QWidget *parent = nullptr)
        : Base(parent)
    {
        initBase();
    }

    // ==================== 图像设置 ====================

    /**
     * @brief 设置图像路径。
     *
     * @param imagePath 图像文件路径。
     */
    void setImage(const QString &imagePath)
    {
        m_pixmap = QPixmap(imagePath);
        if (m_pixmap.isNull()) {
            qWarning() << "[uiImageTextMixin] 图像加载失败:" << imagePath;
        }
        this->update();
    }

    /**
     * @brief 设置 QPixmap 图像。
     *
     * @param pixmap 要显示的 QPixmap 对象。
     */
    void setImage(const QPixmap &pixmap)
    {
        m_pixmap = pixmap;
        this->update();
    }

    /**
     * @brief 清除图像。
     */
    void clearImage()
    {
        m_pixmap = QPixmap();
        this->update();
    }

    /**
     * @brief 获取当前图像。
     *
     * @return 当前 QPixmap 对象。
     */
    QPixmap image() const { return m_pixmap; }

    // ==================== 图像缩放模式 ====================

    /**
     * @brief 设置图像缩放模式。
     *
     * @param mode 缩放模式：KeepAspectRatio（保持宽高比，默认）、Stretch（拉伸填充）。
     */
    void setImageScaleMode(ImageScaleMode mode)
    {
        m_scaleMode = mode;
        this->update();
    }

    /**
     * @brief 获取图像缩放模式。
     *
     * @return 当前缩放模式。
     */
    ImageScaleMode imageScaleMode() const { return m_scaleMode; }

    /**
     * @brief 设置图像缩放比例。
     *
     * 控制图像相对于原始尺寸的缩放比例。
     * 仅在 KeepAspectRatio 模式下生效。
     *
     * @param ratio 缩放比例，范围0.01~10.0，默认1.0（原始尺寸）。
     */
    void setScaleRatio(qreal ratio)
    {
        m_scaleRatio = qBound(0.01, ratio, 10.0);
        this->update();
    }

    /**
     * @brief 获取图像缩放比例。
     *
     * @return 当前缩放比例。
     */
    qreal scaleRatio() const { return m_scaleRatio; }

    /**
     * @brief 设置图像是否影响控件大小。
     *
     * @param affects true：图像尺寸影响 sizeHint（默认）
     *               false：图像不影响 sizeHint，图像自适应控件大小
     */
    void setImageAffectsSizeHint(bool affects)
    {
        m_imageAffectsSizeHint = affects;
        this->updateGeometry();
    }

    /**
     * @brief 获取图像是否影响控件大小。
     */
    bool imageAffectsSizeHint() const { return m_imageAffectsSizeHint; }

    // ==================== 尺寸策略 ====================

    /**
     * @brief 设置尺寸策略（便捷方法，单参数）。
     *
     * 同时设置水平和垂直策略为相同值。
     *
     * @param policy 策略：QSizePolicy::Preferred、Maximum、Minimum、Fixed 等
     */
    void setSizePolicySimple(QSizePolicy::Policy policy)
    {
        Base::setSizePolicy(policy, policy);
        this->updateGeometry();
    }

    // ==================== 文本对齐 ====================

    /**
     * @brief 设置水平对齐模式。
     *
     * @param align 水平对齐方式：HLeft（左对齐）、HCenter（居中）、HRight（右对齐）。
     */
    void setHorizontalAlignment(HorizontalAlignment align)
    {
        m_hAlignment = align;
        this->update();
    }

    /**
     * @brief 获取水平对齐模式。
     */
    HorizontalAlignment horizontalAlignment() const { return m_hAlignment; }

    /**
     * @brief 设置垂直对齐模式。
     *
     * @param align 垂直对齐方式：VTop（上对齐）、VCenter（居中）、VBottom（下对齐）。
     */
    void setVerticalAlignment(VerticalAlignment align)
    {
        m_vAlignment = align;
        this->update();
    }

    /**
     * @brief 获取垂直对齐模式。
     */
    VerticalAlignment verticalAlignment() const { return m_vAlignment; }

    // ==================== 文本边距 ====================

    /**
     * @brief 设置水平边距比例。
     *
     * 仅在水平左对齐或右对齐时生效。
     *
     * @param ratio 边距比例，范围0.0~1.0。
     */
    void setHorizontalMargin(qreal ratio)
    {
        m_hMargin = qBound(0.0, ratio, 1.0);
        this->update();
    }

    /**
     * @brief 获取水平边距比例。
     */
    qreal horizontalMargin() const { return m_hMargin; }

    /**
     * @brief 设置垂直边距比例。
     *
     * 仅在垂直上对齐或下对齐时生效。
     *
     * @param ratio 边距比例，范围0.0~1.0。
     */
    void setVerticalMargin(qreal ratio)
    {
        m_vMargin = qBound(0.0, ratio, 1.0);
        this->update();
    }

    /**
     * @brief 获取垂直边距比例。
     */
    qreal verticalMargin() const { return m_vMargin; }

    // ==================== 文本样式 ====================

    /**
     * @brief 设置文本字号（点阵）。
     *
     * @param pointSize 字号，单位为点。
     */
    void setFontSize(int pointSize)
    {
        QFont f = this->font();
        f.setPointSize(pointSize);
        this->setFont(f);
        this->update();
    }

    /**
     * @brief 设置文本字号（像素）。
     *
     * @param pixelSize 字号，单位为像素。
     */
    void setFontPixelSize(int pixelSize)
    {
        QFont f = this->font();
        f.setPixelSize(pixelSize);
        this->setFont(f);
        this->update();
    }

    /**
     * @brief 设置文本颜色。
     *
     * @param color 文本颜色。
     */
    void setTextColor(const QColor &color)
    {
        m_textColor = color;
        this->update();
    }

    /**
     * @brief 获取文本颜色。
     */
    QColor textColor() const { return m_textColor; }

    // ==================== 尺寸计算 ====================

    /**
     * @brief 获取建议尺寸。
     *
     * @return 基于图像或文本的推荐尺寸。
     */
    QSize sizeHint() const override
    {
        QSize hint;
        if (m_pixmap.isNull() || !m_imageAffectsSizeHint) {
            // 无图像或图像不影响尺寸时，返回基类 sizeHint
            hint = getBaseSizeHint();
        } else {
            // 图像影响尺寸时，返回图像尺寸
            hint = m_pixmap.size() * m_scaleRatio;
        }
        
        QSize result = hint + QSize(m_marginLeft + m_marginRight, m_marginTop + m_marginBottom);
        return result;
    }

    /**
     * @brief 高度依赖宽度计算。
     */
    int heightForWidth(int width) const override
    {
        if (m_pixmap.isNull() || m_scaleMode == Stretch) {
            return width;
        }
        double aspectRatio = static_cast<double>(m_pixmap.height()) / m_pixmap.width();
        return static_cast<int>(width * aspectRatio);
    }

    /**
     * @brief 是否有高度依赖宽度。
     */
    bool hasHeightForWidth() const override { return true; }

protected:
    // ==================== 绘制方法 ====================

    /**
     * @brief 绘制图像和文本。
     *
     * 在 paintEvent 中调用此方法进行绘制。
     */
    void paintImageAndText()
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);

        // 计算可用区域（减去外边距）
        QRect rect = this->rect();
        QRect contentRect(
            rect.x() + m_marginLeft,
            rect.y() + m_marginTop,
            rect.width() - m_marginLeft - m_marginRight,
            rect.height() - m_marginTop - m_marginBottom
        );

        // 绘制图像
        if (!m_pixmap.isNull()) {
            QRect targetRect;
            if (m_scaleMode == Stretch) {
                targetRect = contentRect;
            } else {
                // 先按 scaleRatio 缩放
                QSize baseSize = m_pixmap.size() * m_scaleRatio;
                // 再保持宽高比适应可用区域
                QSize scaledSize = baseSize;
                scaledSize.scale(contentRect.size(), Qt::KeepAspectRatio);
                int x = contentRect.x() + (contentRect.width() - scaledSize.width()) / 2;
                int y = contentRect.y() + (contentRect.height() - scaledSize.height()) / 2;
                targetRect = QRect(x, y, scaledSize.width(), scaledSize.height());
            }
            painter.drawPixmap(targetRect, m_pixmap);
        }

        // 获取文本
        QString labelText = getText();
        if (!labelText.isEmpty()) {
            QFontMetrics fm(this->font());
            QSize textSize = fm.size(Qt::TextSingleLine, labelText);

            // 计算水平位置（在 contentRect 内）
            int textX;
            int hMarginPixels = static_cast<int>(contentRect.width() * m_hMargin);
            switch (m_hAlignment) {
            case HLeft:
                textX = contentRect.x() + hMarginPixels;
                break;
            case HRight:
                textX = contentRect.x() + contentRect.width() - textSize.width() - hMarginPixels;
                break;
            case HCenter:
            default:
                textX = contentRect.x() + (contentRect.width() - textSize.width()) / 2;
                break;
            }

            // 计算垂直位置（在 contentRect 内）
            int textY;
            int vMarginPixels = static_cast<int>(contentRect.height() * m_vMargin);
            switch (m_vAlignment) {
            case VTop:
                textY = contentRect.y() + vMarginPixels;
                break;
            case VBottom:
                textY = contentRect.y() + contentRect.height() - textSize.height() - vMarginPixels;
                break;
            case VCenter:
            default:
                textY = contentRect.y() + (contentRect.height() - textSize.height()) / 2;
                break;
            }

            // 绘制文本
            QRect textRect(textX, textY, textSize.width(), textSize.height());
            painter.setFont(this->font());
            QColor textColor = m_textColor.isValid() ? m_textColor : getDefaultTextColor();
            painter.setPen(textColor);
            painter.drawText(textRect, Qt::AlignCenter, labelText);
        }
    }

    // ==================== 虚方法（派生类实现） ====================

    /**
     * @brief 获取文本内容（派生类必须实现）。
     */
    virtual QString getText() const = 0;

    /**
     * @brief 获取默认文本颜色（派生类必须实现）。
     */
    virtual QColor getDefaultTextColor() const = 0;

    /**
     * @brief 获取基类的 sizeHint（派生类必须实现）。
     */
    virtual QSize getBaseSizeHint() const = 0;

protected:
    // ==================== 成员变量 ====================

    QPixmap m_pixmap;                                    ///< 图像

    HorizontalAlignment m_hAlignment = HCenter;           ///< 水平对齐模式，默认居中
    VerticalAlignment m_vAlignment = VCenter;             ///< 垂直对齐模式，默认居中
    qreal m_hMargin = 0.0;                               ///< 水平边距比例
    qreal m_vMargin = 0.0;                               ///< 垂直边距比例

    QColor m_textColor;                                  ///< 文本颜色（空则使用默认）

    ImageScaleMode m_scaleMode = KeepAspectRatio;         ///< 图像缩放模式
    qreal m_scaleRatio = 1.0;                            ///< 图像缩放比例
    bool m_imageAffectsSizeHint = false;                 ///< 图像是否影响 sizeHint，默认不自适应

    int m_marginLeft = 0;                                ///< 左外边距
    int m_marginTop = 0;                                 ///< 上外边距
    int m_marginRight = 0;                               ///< 右外边距
    int m_marginBottom = 0;                              ///< 下外边距

private:
    void initBase()
    {
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->setAutoFillBackground(false);
    }
};

#endif // UIIMAGETEXTMIXIN_H
