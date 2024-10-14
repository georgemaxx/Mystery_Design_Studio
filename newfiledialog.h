#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include "common.h"


namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = nullptr);
    ~NewFileDialog();

    QLabel *label1;
    QLineEdit *newFileNameLineEdit;
    QLineEdit *newFileLocationLineEdit;

    QString newFileLocation;

private slots:
    void text_edit_event(QString);
    void text_changed_event(QString);
    void browse_button_clicked(bool);

private:
    Ui::NewFileDialog *ui;
};

#endif // NEWFILEDIALOG_H
