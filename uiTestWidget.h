#ifndef uiTestWidget_H
#define uiTestWidget_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class uiTestWidget;
}
QT_END_NAMESPACE

class uiTestWidget : public QWidget
{
    Q_OBJECT

public:
    uiTestWidget(QWidget *parent = nullptr);
    ~uiTestWidget();

private:
    Ui::uiTestWidget *ui;
};
#endif // uiTestWidget_H
