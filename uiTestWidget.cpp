#include "uiTestWidget.h"
#include "uiPushbutton.h"
#include "uiCheckbox.h"
#include "uiSlideVerify.h"
#include "ui_uiTestWidget.h"
#include <QGridLayout>

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
    ui->pushButton->setScaleRatio(0.5);


    ui->checkBox->setUncheckedImage(":/res/checkBox/no.png");
    ui->checkBox->setCheckedImage(":/res/checkBox/off.png");
    ui->checkBox->setImageSizeMode(uiCheckbox::ImageAdaptive);

    // ui->pushButton_2->setIcon(":/res/icon/camera_2.png", uiPushbutton::IconTop);
    // ui->pushButton_2->setIconSize(QSize(32, 32));
    // ui->pushButton_2->setBackgroundColor(Qt::blue);
    // ui->pushButton_2->setBorderRadius(10);
    ui->pushButton_2->setTransparentBackground(true);
    ui->pushButton_2->setTextColor(Qt::red);

    // 滑动验证测试
    uiSlideVerify *slider = new uiSlideVerify(this);
    slider->setMinimumSize(300, 40);
    slider->setBorderRadius(5);                          // 轨道圆角
    slider->setHintText(QString::fromUtf8("拖动滑块验证"));
    slider->setSliderColor(Qt::white);                    // 滑块白色填充
    slider->setSliderRadius(5);                          // 滑块圆角
    slider->setSliderBorderColor(QColor("#acacac"));      // 滑块边框
    slider->setSliderIcon(":/res/icon/slideBlock.png");   // 滑块图标
    slider->setProgressColor(QColor(82, 196, 26));        // 进度条绿色
    slider->setThreshold(0.95);
    slider->setSliderAspectRatio(1.25);

    QGridLayout *grid = qobject_cast<QGridLayout*>(layout());
    if (grid) {
        grid->addWidget(slider, 3, 1, 1, 2);
    }
}

uiTestWidget::~uiTestWidget()
{
    delete ui;
}
