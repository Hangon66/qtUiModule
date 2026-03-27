#include "uipushbuttnwidget.h"
#include "ui_uipushbuttnwidget.h"

uiPushButtnWidget::uiPushButtnWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::uiPushButtnWidget)
{
    ui->setupUi(this);
}

uiPushButtnWidget::~uiPushButtnWidget()
{
    delete ui;
}
