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
    
    // 设置文本偏移比例 (-1.0 ~ 1.0, 0为居中)
    void setTextOffset(qreal horizontalRatio, qreal verticalRatio);
    // 设置水平偏移比例 (-1.0 ~ 1.0)
    void setHorizontalOffset(qreal ratio);
    // 设置垂直偏移比例 (-1.0 ~ 1.0)
    void setVerticalOffset(qreal ratio);
        
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
    qreal m_hOffsetRatio = 0.0; // 水平偏移比例 (-1.0 ~ 1.0)
    qreal m_vOffsetRatio = 0.0; // 垂直偏移比例 (-1.0 ~ 1.0)
    QColor m_textColor;         // 文本颜色（空则使用默认）
};

#endif // uiPushbutton_H
