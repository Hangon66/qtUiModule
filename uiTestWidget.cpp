#include "uiTestWidget.h"
#include "ui_uiTestWidget.h"

uiTestWidget::uiTestWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiTestWidget)
{
    ui->setupUi(this);
    ui->label->setImage(":/res/pushbutton/Stat2.png");
    ui->label_2->setImage(":/res/pushbutton/Stat1.png");

    ui->pushButton->setImage(":/res/pushbutton/monitoring1.png");
    ui->pushButton->setHoverImage(":/res/pushbutton/monitoring2.png");
    ui->pushButton->setPressedImage(":/res/pushbutton/monitoring2.png");

    ui->pushButton_2->setImage(":/res/pushbutton/para1.png");
    ui->pushButton_2->setHoverImage(":/res/pushbutton/para2.png");
    ui->pushButton_2->setPressedImage(":/res/pushbutton/para2.png");
}

uiTestWidget::~uiTestWidget()
{
    delete ui;
}
