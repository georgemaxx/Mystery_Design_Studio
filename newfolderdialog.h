#ifndef NEWFOLDERDIALOG_H
#define NEWFOLDERDIALOG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class NewFolderDialog;
}

class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolderDialog(QWidget *parent = nullptr);
    ~NewFolderDialog();

    QLabel *label1;
    QLineEdit *newFolderNameLineEdit;
    QLineEdit *newFolderLocationLineEdit;
    QString newFolderLocation;

private slots:
    void text_edit_event(QString);
    void text_changed_event(QString);
    void browse_button_clicked(bool);
    void judge_folder_exist(void);

private:
    Ui::NewFolderDialog *ui;
};

#endif // NEWFOLDERDIALOG_H
