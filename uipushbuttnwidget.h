#ifndef UIPUSHBUTTNWIDGET_H
#define UIPUSHBUTTNWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class uiPushButtnWidget;
}
QT_END_NAMESPACE

class uiPushButtnWidget : public QWidget
{
    Q_OBJECT

public:
    uiPushButtnWidget(QWidget *parent = nullptr);
    ~uiPushButtnWidget();

private:
    Ui::uiPushButtnWidget *ui;
};
#endif // UIPUSHBUTTNWIDGET_H
