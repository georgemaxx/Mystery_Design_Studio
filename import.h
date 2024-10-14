#ifndef IMPORT_H
#define IMPORT_H

#include <QWidget>
#include "common.h"

class Import : public QWidget
{
    Q_OBJECT
public:
    QLineEdit *prjLocationLineEdit;
    QString prjLocation;

    explicit Import(QWidget *parent = nullptr);
    ~Import();

signals:

private slots:
    void on_browse_button_clicked(bool);
    void on_finish_button_clicked(bool);

};

#endif // IMPORT_H
