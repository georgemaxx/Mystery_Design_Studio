#ifndef NEWFILE_H
#define NEWFILE_H

#include "common.h"

class NewFile : public QDialog
{
    Q_OBJECT

public:
    explicit NewFile(QWidget *parent = nullptr);
    ~NewFile();

    QLabel *label1;
    QLineEdit *newFileNameLineEdit;
    QDialogButtonBox *buttonBox;

private slots:
    void text_edit_event(QString);

};

#endif // NEWFILE_H
