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
    ui->pushButton->setPressedImage(":/res/pushbutton/monitoring2.png");
    ui->pushButton->setImageSizeMode(uiPushbutton::ImageAdaptive);


    ui->checkBox->setUncheckedImage(":/res/checkBox/no.png");
    ui->checkBox->setCheckedImage(":/res/checkBox/off.png");
    ui->checkBox->setImageSizeMode(uiCheckbox::ImageAdaptive);

    ui->pushButton_2->setIcon(":/res/icon/camera_2.png", uiPushbutton::IconTop);
    ui->pushButton_2->setIconSize(QSize(32, 32));
    ui->pushButton_2->setBackgroundColor(Qt::blue);
    ui->pushButton_2->setBorderRadius(10);
}

uiTestWidget::~uiTestWidget()
{
    delete ui;
}
