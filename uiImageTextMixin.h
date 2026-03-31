#ifndef UIIMAGETEXTMIXIN_H
#define UIIMAGETEXTMIXIN_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
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

    // ==================== Icon设置 ====================

    /**
     * @brief Icon位置枚举。
     */
    enum IconPosition {
        IconLeft,       ///< Icon在文字左侧
        IconRight,      ///< Icon在文字右侧
        IconTop,        ///< Icon在文字上方
        IconBottom      ///< Icon在文字下方
    };

    /**
     * @brief 设置Icon图像。
     *
     * @param path 图像资源路径。
     * @param position Icon位置，默认在文字左侧。
     */
    void setIcon(const QString &path, IconPosition position = IconLeft)
    {
        m_icon = QPixmap(path);
        m_iconPosition = position;
        this->update();
    }

    /**
     * @brief 设置Icon图像。
     *
     * @param pixmap 图像对象。
     * @param position Icon位置，默认在文字左侧。
     */
    void setIcon(const QPixmap &pixmap, IconPosition position = IconLeft)
    {
        m_icon = pixmap;
        m_iconPosition = position;
        this->update();
    }

    /**
     * @brief 设置Icon位置。
     *
     * @param position Icon位置。
     */
    void setIconPosition(IconPosition position)
    {
        m_iconPosition = position;
        this->update();
    }

    /**
     * @brief 获取Icon位置。
     */
    IconPosition iconPosition() const { return m_iconPosition; }

    /**
     * @brief 设置Icon大小。
     *
     * @param size Icon大小，单位为像素。
     */
    void setIconSize(const QSize &size)
    {
        m_iconSize = size;
        this->update();
    }

    /**
     * @brief 设置Icon大小。
     *
     * @param width 宽度。
     * @param height 高度。
     */
    void setIconSize(int width, int height)
    {
        m_iconSize = QSize(width, height);
        this->update();
    }

    /**
     * @brief 获取Icon大小。
     */
    QSize iconSize() const { return m_iconSize; }

    /**
     * @brief 设置Icon与文字的间距。
     *
     * @param spacing 间距，单位为像素。
     */
    void setIconSpacing(int spacing)
    {
        m_iconSpacing = qMax(0, spacing);
        this->update();
    }

    /**
     * @brief 获取Icon与文字的间距。
     */
    int iconSpacing() const { return m_iconSpacing; }

    /**
     * @brief 获取Icon图像。
     */
    QPixmap icon() const { return m_icon; }

    /**
     * @brief 清除Icon。
     */
    void clearIcon()
    {
        m_icon = QPixmap();
        this->update();
    }

    /**
     * @brief 设置图像在控件内的显示比例。
     *
     * 图像先等比例缩放到控件大小，然后按此比例调整显示大小。
     * 1.0 = 填满控件（保持比例），0.5 = 一半大小，2.0 = 超出控件
     * 不改变控件大小，仅影响图像绘制尺寸。
     * 仅在 KeepAspectRatio 模式下生效。
     *
     * @param ratio 显示比例，范围0.01~10.0，默认1.0（填满控件）。
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

    // ==================== 尺寸策略 ====================

    /**
     * @brief 图像尺寸模式枚举。
     */
    enum ImageSizeMode {
        FixedToImage,       ///< 控件固定为图像大小
        MinimumToImage,     ///< 控件最小为图像大小（默认）
        MaximumToImage,     ///< 控件最大为图像大小
        ImageAdaptive       ///< 图像自适应控件大小
    };

    /**
     * @brief 设置图像尺寸模式。
     *
     * @param mode 模式：FixedToImage、MinimumToImage、MaximumToImage、ImageAdaptive
     */
    void setImageSizeMode(ImageSizeMode mode)
    {
        switch (mode) {
        case FixedToImage:
            m_imageAffectsSizeHint = true;
            Base::setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            break;
        case MinimumToImage:
            m_imageAffectsSizeHint = true;
            Base::setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            break;
        case MaximumToImage:
            m_imageAffectsSizeHint = true;
            Base::setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            break;
        case ImageAdaptive:
            m_imageAffectsSizeHint = false;
            Base::setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            break;
        }
        this->updateGeometry();
    }

    /**
     * @brief 获取图像是否影响控件大小。
     *
     * @return true 表示图像影响 sizeHint，false 表示不影响。
     */
    bool imageAffectsSizeHint() const { return m_imageAffectsSizeHint; }

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

    // ==================== 圆角设置 ====================

    /**
     * @brief 设置圆角半径（四角相同）。
     *
     * @param radius 圆角半径，单位为像素。0 表示无圆角。
     */
    void setBorderRadius(int radius)
    {
        m_borderRadiusTopLeft = m_borderRadiusTopRight =
        m_borderRadiusBottomRight = m_borderRadiusBottomLeft = qMax(0, radius);
        this->update();
    }

    /**
     * @brief 设置四个角的圆角半径。
     *
     * @param topLeft 左上角圆角半径。
     * @param topRight 右上角圆角半径。
     * @param bottomRight 右下角圆角半径。
     * @param bottomLeft 左下角圆角半径。
     */
    void setBorderRadius(int topLeft, int topRight, int bottomRight, int bottomLeft)
    {
        m_borderRadiusTopLeft = qMax(0, topLeft);
        m_borderRadiusTopRight = qMax(0, topRight);
        m_borderRadiusBottomRight = qMax(0, bottomRight);
        m_borderRadiusBottomLeft = qMax(0, bottomLeft);
        this->update();
    }

    /**
     * @brief 获取左上角圆角半径。
     */
    int borderRadiusTopLeft() const { return m_borderRadiusTopLeft; }

    /**
     * @brief 获取右上角圆角半径。
     */
    int borderRadiusTopRight() const { return m_borderRadiusTopRight; }

    /**
     * @brief 获取右下角圆角半径。
     */
    int borderRadiusBottomRight() const { return m_borderRadiusBottomRight; }

    /**
     * @brief 获取左下角圆角半径。
     */
    int borderRadiusBottomLeft() const { return m_borderRadiusBottomLeft; }

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
     *
     * 仅在有图像且保持宽高比模式下生效。
     * 无图像时返回基类的高度计算。
     *
     * @param width 控件宽度。
     * @return 对应的高度值。
     */
    int heightForWidth(int width) const override
    {
        if (m_pixmap.isNull()) {
            // 无图像时，返回基类的 sizeHint 高度，避免错误的正方形高度
            return getBaseSizeHint().height();
        }
        if (m_scaleMode == Stretch) {
            // 拉伸模式不保持宽高比，返回基类高度
            return getBaseSizeHint().height();
        }
        double aspectRatio = static_cast<double>(m_pixmap.height()) / m_pixmap.width();
        return static_cast<int>(width * aspectRatio);
    }

    /**
     * @brief 是否有高度依赖宽度。
     *
     * 仅在有图像且非拉伸模式时返回 true。
     */
    bool hasHeightForWidth() const override
    {
        return !m_pixmap.isNull() && m_scaleMode != Stretch;
    }

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

        // 计算图像实际显示区域
        QRect targetRect;
        if (!m_pixmap.isNull()) {
            if (m_scaleMode == Stretch) {
                targetRect = contentRect;
            } else {
                // 1. 先计算图像等比例缩放到 contentRect 的大小
                QSize scaledToFit = m_pixmap.size();
                scaledToFit.scale(contentRect.size(), Qt::KeepAspectRatio);
                
                // 2. 再按 scaleRatio 调整（相对于 contentRect 的比例）
                int scaledWidth = static_cast<int>(scaledToFit.width() * m_scaleRatio);
                int scaledHeight = static_cast<int>(scaledToFit.height() * m_scaleRatio);
                
                // 3. 居中显示
                int x = contentRect.x() + (contentRect.width() - scaledWidth) / 2;
                int y = contentRect.y() + (contentRect.height() - scaledHeight) / 2;
                targetRect = QRect(x, y, scaledWidth, scaledHeight);
            }
            
            // 绘制图像（带圆角裁剪）
            bool hasBorderRadius = m_borderRadiusTopLeft > 0 || m_borderRadiusTopRight > 0 ||
                                   m_borderRadiusBottomRight > 0 || m_borderRadiusBottomLeft > 0;
            if (hasBorderRadius) {
                QPainterPath path;
                // 构建不同圆角的路径
                QRect r = targetRect;
                int tl = m_borderRadiusTopLeft;
                int tr = m_borderRadiusTopRight;
                int br = m_borderRadiusBottomRight;
                int bl = m_borderRadiusBottomLeft;

                path.moveTo(r.x() + tl, r.y());
                path.lineTo(r.x() + r.width() - tr, r.y());
                if (tr > 0) path.arcTo(r.x() + r.width() - 2*tr, r.y(), 2*tr, 2*tr, 90, -90);
                else path.lineTo(r.x() + r.width(), r.y());

                path.lineTo(r.x() + r.width(), r.y() + r.height() - br);
                if (br > 0) path.arcTo(r.x() + r.width() - 2*br, r.y() + r.height() - 2*br, 2*br, 2*br, 0, -90);
                else path.lineTo(r.x() + r.width(), r.y() + r.height());

                path.lineTo(r.x() + bl, r.y() + r.height());
                if (bl > 0) path.arcTo(r.x(), r.y() + r.height() - 2*bl, 2*bl, 2*bl, 270, -90);
                else path.lineTo(r.x(), r.y() + r.height());

                path.lineTo(r.x(), r.y() + tl);
                if (tl > 0) path.arcTo(r.x(), r.y(), 2*tl, 2*tl, 180, -90);
                else path.lineTo(r.x(), r.y());

                path.closeSubpath();
                painter.setClipPath(path);
            }
            painter.drawPixmap(targetRect, m_pixmap);
            painter.setClipping(false);  // 重置裁剪，避免影响文本绘制
        }

        // 获取文本
        QString labelText = getText();
        if (!labelText.isEmpty() || !m_icon.isNull()) {
            QFontMetrics fm(this->font());
            QSize textSize = fm.size(Qt::TextSingleLine, labelText);

            // 计算图像实际显示区域用于文本定位参考
            QRect imageRect;
            if (!m_pixmap.isNull() && m_scaleMode != Stretch) {
                // 使用已计算的 targetRect（图像实际显示区域）
                imageRect = targetRect;
            } else {
                imageRect = contentRect;
            }

            // 计算Icon和文本的总大小
            bool hasIcon = !m_icon.isNull();
            QSize iconSize = hasIcon ? m_iconSize : QSize(0, 0);
            int spacing = hasIcon ? m_iconSpacing : 0;

            // 计算组合区域大小
            int totalWidth, totalHeight;
            int iconX, iconY, textX, textY;

            if (hasIcon) {
                switch (m_iconPosition) {
                case IconLeft:
                case IconRight:
                    totalWidth = iconSize.width() + spacing + textSize.width();
                    totalHeight = qMax(iconSize.height(), textSize.height());
                    break;
                case IconTop:
                case IconBottom:
                    totalWidth = qMax(iconSize.width(), textSize.width());
                    totalHeight = iconSize.height() + spacing + textSize.height();
                    break;
                }
            } else {
                totalWidth = textSize.width();
                totalHeight = textSize.height();
            }

            // 计算组合区域的起始位置（基于对齐方式）
            int baseX, baseY;
            int hMarginPixels = static_cast<int>(imageRect.width() * m_hMargin);
            int vMarginPixels = static_cast<int>(imageRect.height() * m_vMargin);

            switch (m_hAlignment) {
            case HLeft:
                baseX = imageRect.x() + hMarginPixels;
                break;
            case HRight:
                baseX = imageRect.x() + imageRect.width() - totalWidth - hMarginPixels;
                break;
            case HCenter:
            default:
                baseX = imageRect.x() + (imageRect.width() - totalWidth) / 2;
                break;
            }

            switch (m_vAlignment) {
            case VTop:
                baseY = imageRect.y() + vMarginPixels;
                break;
            case VBottom:
                baseY = imageRect.y() + imageRect.height() - totalHeight - vMarginPixels;
                break;
            case VCenter:
            default:
                baseY = imageRect.y() + (imageRect.height() - totalHeight) / 2;
                break;
            }

            // 计算Icon和文本的具体位置
            if (hasIcon) {
                switch (m_iconPosition) {
                case IconLeft:
                    iconX = baseX;
                    iconY = baseY + (totalHeight - iconSize.height()) / 2;
                    textX = baseX + iconSize.width() + spacing;
                    textY = baseY + (totalHeight - textSize.height()) / 2;
                    break;
                case IconRight:
                    iconX = baseX + textSize.width() + spacing;
                    iconY = baseY + (totalHeight - iconSize.height()) / 2;
                    textX = baseX;
                    textY = baseY + (totalHeight - textSize.height()) / 2;
                    break;
                case IconTop:
                    iconX = baseX + (totalWidth - iconSize.width()) / 2;
                    iconY = baseY;
                    textX = baseX + (totalWidth - textSize.width()) / 2;
                    textY = baseY + iconSize.height() + spacing;
                    break;
                case IconBottom:
                    iconX = baseX + (totalWidth - iconSize.width()) / 2;
                    iconY = baseY + textSize.height() + spacing;
                    textX = baseX + (totalWidth - textSize.width()) / 2;
                    textY = baseY;
                    break;
                }

                // 绘制Icon
                QRect iconRect(iconX, iconY, iconSize.width(), iconSize.height());
                painter.drawPixmap(iconRect, m_icon.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                textX = baseX;
                textY = baseY;
            }

            // 绘制文本
            if (!labelText.isEmpty()) {
                QRect textRect(textX, textY, textSize.width(), textSize.height());
                painter.setFont(this->font());
                QColor textColor = m_textColor.isValid() ? m_textColor : getDefaultTextColor();
                painter.setPen(textColor);
                painter.drawText(textRect, Qt::AlignCenter, labelText);
            }
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

    int m_borderRadiusTopLeft = 0;                       ///< 左上角圆角半径
    int m_borderRadiusTopRight = 0;                      ///< 右上角圆角半径
    int m_borderRadiusBottomRight = 0;                   ///< 右下角圆角半径
    int m_borderRadiusBottomLeft = 0;                    ///< 左下角圆角半径

    QPixmap m_icon;                                      ///< Icon图像
    IconPosition m_iconPosition = IconLeft;              ///< Icon位置
    QSize m_iconSize = QSize(16, 16);                    ///< Icon大小
    int m_iconSpacing = 4;                               ///< Icon与文字的间距

    ImageScaleMode m_scaleMode = KeepAspectRatio;         ///< 图像缩放模式
    qreal m_scaleRatio = 1.0;                            ///< 图像缩放比例
    bool m_imageAffectsSizeHint = true;                  ///< 图像是否影响 sizeHint，默认影响（控件最小为图像大小）

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
