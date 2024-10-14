/*******************************************************
 *
 * @file        mainwindow.h
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/11
 *
 ******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"
#include "srceditor.h"
#include "highlight.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QComboBox *confPrjCombo;
    QTreeWidget *prjTreeWidget;
    QTextBrowser *text_browser;
    QString prjRootPath;
    CodeEditor *codeEditor;
    QTabWidget *cenTabWidget;
    int openFlag;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void click_menu_new_file();
    void click_menu_new_project();
    void click_menu_open_folder();
    void click_menu_save();
    void right_click_custom_context_menu_requested(const QPoint&);
    void double_click_item_handler(QTreeWidgetItem *, int);

    void proj_tree_widget_new_file_action();
    void proj_tree_widget_new_folder_action();
    void proj_tree_widget_delete_action();
    void proj_tree_widget_import_action();
    void proj_tree_widget_refresh_action();
    //void click_item_handler(QTreeWidgetItem *, int);

    void tab_bar_changed_event(int);

private:
    Ui::MainWindow *ui;
    QMenu *popMenu;
    QAction *newFileAction;
    QAction *newFolderAction;
    QAction *deleteAction;
    QAction *importAction;
    QAction *refreshAction;
    CSyntaxHighlight *cSyntaxHighlight;
    CodeEditor *ce[1000];
    QTreeWidgetItem *traverse_tree_widget_item(QTreeWidgetItem *item, int level, QString path);
    void tree_widget_add_item(QTreeWidgetItem *item, QString filePath, QString fileName);
    void display_file_content_in_tab_widget(QString filePath, QString fileName);
    void remove_item(QTreeWidgetItem *item, QString path);
};

// customize tab bar font style
class TabBarProxyStyle : public QProxyStyle
{
public:
    TabBarProxyStyle(QStyle *style = nullptr) : QProxyStyle(style) { }

    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override
    {
        if (element == CE_TabBarTab) {
            painter->save(); // save the painter to restore later
            painter->setFont(QFont("Calibri", 8)); // change the defaul font of painter
            QProxyStyle::drawControl(element, option, painter, widget); // paint the TabBarTab using the default drawControl
            painter->restore(); // restore to default painter
        }
        else{
            QProxyStyle::drawControl(element, option, painter, widget); // use default drawControl to paint all other components without changes.
        }
    }

};

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

};
#endif // MAINWINDOW_H
