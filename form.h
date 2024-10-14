/*******************************************************
 *
 * @file        form.h
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/11
 *
 ******************************************************/

#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "common.h"
#include "device.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    QLabel *projectNameLabel;
    QLabel *projPathLabel;
    QLineEdit *lineEdit;
    QLineEdit *packageLineEdit;
    QLineEdit *projectNameLineEdit;
    QLineEdit *sdkPackagePathLineEdit;
    QLineEdit *projPathLineEdit;
    QString sdkPackage;
    QString projPath;
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
    void select_device_clicked(void);
    void choose_project_path_browse_button_clicked(bool);
    void browse_button_clicked(bool);
    void finish_button_clicked(bool);

private:
    Ui::Form *ui;
};

#endif // FORM_H
