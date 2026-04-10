#include "uiTestWidget.h"
#include "uiPushbutton.h"
#include "uiCheckbox.h"
#include "ui_uiTestWidget.h"

uiTestWidget::uiTestWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiTestWidget)
{
    ui->setupUi(this);
    ui->label->setImage(":/res/pushbutton/Stat2.png");
    ui->label->setImageSizeMode(uiLabel::ImageAdaptive);
    ui->label->setBorderRadius(10, 10, 0, 0);

    ui->pushButton->setImage(":/res/pushbutton/monitoring1.png");
    ui->pushButton->setHoverImage(":/res/pushbutton/monitoring2.png");
    ui->pushButton->setPressedImage(":/res/pushbutton/monitoring2.png");
    ui->pushButton->setImageSizeMode(uiPushbutton::ImageAdaptive);

    ui->checkBox->setUncheckedImage(":/res/checkBox/no.png");
    ui->checkBox->setCheckedImage(":/res/checkBox/off.png");
    ui->checkBox->setImageSizeMode(uiCheckbox::ImageAdaptive);
}

uiTestWidget::~uiTestWidget()
{
    delete ui;
}
