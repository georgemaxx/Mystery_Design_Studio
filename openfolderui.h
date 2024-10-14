/*******************************************************
 *
 * @file        openfolderui.h
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/10/07
 *
 ******************************************************/

#ifndef OPENFOLDERUI_H
#define OPENFOLDERUI_H

#include "common.h"

class OpenFolder : public QWidget
{
    Q_OBJECT

public:
    QString prjFolderLocation;
    void list_dirs(QTreeWidgetItem *tw, const QString &path);
    explicit OpenFolder(QWidget *parent = nullptr);
    ~OpenFolder();

private:
    QLabel *label1;
    QLineEdit *prjFolderLineEdit;
    QPushButton *btn_ok;

private slots:
    void text_changed_event(QString);
    void browse_button_click_event();
    void ok_button_click_event(bool);
    void cancel_button_click_event(bool);

};

#endif // OPENFOLDERUI_H
