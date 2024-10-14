/*******************************************************
 *
 * @file        mainwindow.cpp
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/10
 *
 ******************************************************/

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "form.h"
#include "ui_form.h"
#include "newfiledialog.h"
#include "newfolderdialog.h"
#include "openfolderui.h"
#include "import.h"
#include "newfile.h"
#include "common.h"
#include "srceditor.h"
#include <QMessageBox>
#include <QtCore>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

Form *wizard;
extern MainWindow *w;
NewFile *newFileDialog;
NewFileDialog *new_file_dialog;
NewFolderDialog *new_folder_dialog;
Import *importWidget;
OpenFolder *openfolderuiWidget;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wizard = new Form;
    importWidget = new Import;
    codeEditor = new CodeEditor;
    openfolderuiWidget = new OpenFolder;
    openFlag = 0;

    // 菜单栏
    connect(ui->actionNew_File,
            SIGNAL(triggered()),
            this,
            SLOT(click_menu_new_file())
            );

    connect(ui->actionNew_Project,
            SIGNAL(triggered()),
            this,
            SLOT(click_menu_new_project()));

    connect(ui->actionOpen_Folder,
            SIGNAL(triggered()),
            this,
            SLOT(click_menu_open_folder()));

    connect(ui->actionSave,
            SIGNAL( triggered() ),
            this,
            SLOT( click_menu_save() )
            );

    // 工具栏 include <QToolBar>
    QToolBar *tbar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, tbar);
    tbar->setMovable(true);
    tbar->setFloatable(false);
    tbar->setIconSize(QSize(18, 18));

    ui->actionIDE->setIcon(QIcon(":/icons/icons/ide.png"));
    tbar->addAction(ui->actionIDE);
    ui->actionClock->setIcon(QIcon(":/icons/icons/clock.png"));
    tbar->addAction(ui->actionClock);
    ui->actionPins->setIcon(QIcon(":/icons/icons/pins.png"));
    tbar->addAction(ui->actionPins);

    // toolbar for folder operation
    QToolBar *fBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, fBar);
    fBar->setFloatable(false);
    fBar->setIconSize(QSize(15, 15));
    // open folder
    fBar->addAction(ui->actionOpen_Folder);
    //ui->actionOpen_Folder->setIcon(QIcon(":/icons/icons/folder.ico"));
    ui->actionOpen_Folder->setIcon(QIcon(":/icons/icons/dir.png"));
    // new file
    fBar->addAction(ui->actionNew_File);
    ui->actionNew_File->setIcon(QIcon(":/icons/icons/file.svg"));

    fBar->addSeparator();
    // save
    fBar->addAction(ui->actionSave);
    ui->actionSave->setIcon(QIcon(":/icons/icons/save.png"));
    ui->actionSave->setShortcut(Qt::CTRL + Qt::Key_S);
    ui->actionOpen_Folder->setShortcut(Qt::CTRL + Qt::Key_O);
    ui->actionNew_File->setShortcut(Qt::CTRL + Qt::Key_N);

    // DockWidget
    QDockWidget *projectExplorerDockWidget = new QDockWidget;
    projectExplorerDockWidget->setMinimumSize(325, 0);
    projectExplorerDockWidget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    projectExplorerDockWidget->setWindowTitle("Project Explorer");
    projectExplorerDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, projectExplorerDockWidget); // add dock widgt project explorer

    // QTreeWidget
    prjTreeWidget = new QTreeWidget;
    QStringList prjTreeWidgetHeader;
    prjTreeWidgetHeader << "Items" << "Type";
    prjTreeWidget->setHeaderLabels(prjTreeWidgetHeader); // set up tree widget header 树头
    prjTreeWidget->setColumnWidth(0, 210); // setup column width
    prjTreeWidget->setSortingEnabled(true);
    prjTreeWidget->sortItems(1, Qt::DescendingOrder); // sort by column 1
    projectExplorerDockWidget->setWidget(prjTreeWidget);

    prjRootPath = "C:/Users/george/Desktop/conf_tool_test/HelloWorld";
    QStringList pl = prjRootPath.split("/");
    QStringList prjName;
    prjName << pl.last() << "project";
    QTreeWidgetItem *prjItem = new QTreeWidgetItem(prjTreeWidget, prjName);
    prjItem->setIcon(0, QIcon(":/icons/icons/chuizi_48.png"));
    prjTreeWidget->addTopLevelItem(prjItem);
    QFont fnt = prjItem->font(0);
    fnt.setBold(true);
    prjItem->setFont(0, fnt);
    prjItem->setData(0, Qt::UserRole, "project");
    prjItem->setData(0, Qt::ToolTipRole, prjRootPath);
    // display the child of top level widget, traverse the project folder
    openfolderuiWidget->list_dirs(prjItem, prjRootPath);

#if 0
    QStringList modules_configuration;
    modules_configuration << "Modules";
    QTreeWidgetItem *modules_config_item = new QTreeWidgetItem(prjTreeWidget, modules_configuration);
    prjTreeWidget->addTopLevelItem(modules_config_item);

    modules_config_item->setIcon(0, QIcon(":/icons/icons/chuizi_48.png"));
    modules_config_item->setData(0, Qt::UserRole, "project");
    modules_config_item->setData(0, Qt::ToolTipRole, "C:/Users/george/Desktop/conf_tool_test");

    QStringList ip_list;
    ip_list << "FlexCAN" << "ACMP" << "ADC" << "eTMR" << "pTMR" << "lpTMR" << "I2C" << "SPI";
    foreach(QString ip, ip_list)
    {
        QTreeWidgetItem *ip_config_item = new QTreeWidgetItem; // 定义子节点
        ip_config_item->setText(0, ip); // 将ip的名字设置到给子节点
        modules_config_item->addChild(ip_config_item); // 将节点加入到modules作为子节点
        if (ip_config_item->text(0) == "lpTMR")
        {
            QTreeWidgetItem *c0 = new QTreeWidgetItem;
            c0->setText(0, "CMP");
            ip_config_item->addChild(c0);
            ip_config_item->setExpanded(true);
            c0->setData(0, Qt::ToolTipRole, "C:/Users/george/Desktop/conf_tool_test/lpTMR/CMP");
            ip_config_item->setIcon(0, QIcon(":/icons/icons/folder.ico"));
            c0->setIcon(0, QIcon(":/icons/icons/cpp.ico"));
        }
        ip_config_item->setIcon(0, QIcon(":/icons/icons/folder.ico"));
        ip_config_item->setData(0, Qt::ToolTipRole, "C:/Users/george/Desktop/conf_tool_test/" + ip);
    }

    // 展开子节点
    //prjTreeWidget->expandItem(modules_config_item);
#endif



    prjTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 设置菜单文本的策略为自定义
    connect(prjTreeWidget,
            SIGNAL( customContextMenuRequested(const QPoint&) ),
            this,
            SLOT( right_click_custom_context_menu_requested(const QPoint&) )
            );

    connect(prjTreeWidget,
            SIGNAL( itemDoubleClicked(QTreeWidgetItem *, int) ),
            this,
            SLOT( double_click_item_handler(QTreeWidgetItem *, int) )
            );

    // output DockWidget
    QDockWidget *outputDockWidget = new QDockWidget;
    addDockWidget(Qt::BottomDockWidgetArea, outputDockWidget);
    text_browser = new QTextBrowser;
    outputDockWidget->setWidget(text_browser);
    outputDockWidget->setWindowTitle("Output");


    // 状态栏 include <QStatusBar>
    QStatusBar *sbar = statusBar();
    QLabel *label = new QLabel(this);
    label->setText("Ready");
    sbar->addWidget(label);

    // tabwidget
    cenTabWidget = new QTabWidget(this);
    cenTabWidget->setTabsClosable(true);
    cenTabWidget->setMovable(true);
    cenTabWidget->setTabShape(QTabWidget::Triangular);
    cenTabWidget->setUsesScrollButtons(true);
    cenTabWidget->tabBar()->setStyle(new TabBarProxyStyle);

    connect(cenTabWidget->tabBar(), &QTabBar::tabCloseRequested, [=](int index){
            if (index == -1)
                return;

            // 移除Tab
            cenTabWidget->removeTab(index);
        });

    connect(cenTabWidget,
            SIGNAL( currentChanged(int) ),
            this,
            SLOT( tab_bar_changed_event(int) )
            );

    setCentralWidget(cenTabWidget);

}

// 遍历treewidget
QTreeWidgetItem *MainWindow::traverse_tree_widget_item(QTreeWidgetItem *item, int level = 0, QString path = "")
{
    QTreeWidgetItem *curItem = new QTreeWidgetItem;

    if (path == prjRootPath)
    {
        curItem = item;
    }
    else
    {
        for (int i = 0; i < item->childCount(); i++)
        {
            if (path == item->child(i)->data(0, Qt::ToolTipRole))
            {
                curItem = item->child(i);
                break;
            }
            traverse_tree_widget_item(item->child(i), level + 1, path);
        }
    }

    return curItem;
}

// add new item into treewidget
void MainWindow::tree_widget_add_item(QTreeWidgetItem *item, QString filePath, QString fileName)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem;
    newItem->setText(0, fileName);
    newItem->setText(1, "file");
    newItem->setData(0, Qt::UserRole, "file");
    newItem->setData(0, Qt::ToolTipRole, filePath);
    if (fileName.right(2) == ".c")
    {
        newItem->setIcon(0, QIcon(":/icons/icons/c.ico"));
    }
    else if (fileName.right(2) == ".h")
    {
        newItem->setIcon(0, QIcon(":/icons/icons/head.ico"));
    }
    else if (fileName.right(4) == ".cpp")
    {
        newItem->setIcon(0, QIcon(":/icons/icons/cpp.ico"));
    }
    else if (fileName.right(2) == ".S" || fileName.right(2) == ".s")
    {
        newItem->setIcon(0, QIcon(":/icons/icons/asm.ico"));
    }
    else
    {
        newItem->setIcon(0, QIcon(":/icons/icons/c.ico"));
    }

    item->addChild(newItem);
    item->setExpanded(true);

    // create new file
    QFile file(filePath);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        file.close();
    }
}

// 显示新的tab，并且展示代码
void MainWindow::display_file_content_in_tab_widget(QString filePath, QString fileName)
{
    // view in tab
    QFile file(filePath);
    int count = cenTabWidget->count();
    qDebug() << "count: " << count;
    ce[count] = new CodeEditor(cenTabWidget);
    ce[count]->setLineWrapMode(QPlainTextEdit::NoWrap);
    openFlag = 0;
    // set default font family as Courier New
    QTextDocument *doc = ce[count]->document();
    QFont docFont = doc->defaultFont();
    docFont.setFamily("Consolas");
    doc->setDefaultFont(docFont);
    cSyntaxHighlight = new CSyntaxHighlight(doc);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in(&file);
        ce[count]->setPlainText(in.readAll());
        file.close();
    }
    else
    {
        ce[count]->setPlainText("Error opening file.");
    }

    int curId = cenTabWidget->addTab(ce[count], fileName);
    cenTabWidget->setTabToolTip(curId, filePath);
    cenTabWidget->setCurrentIndex(curId);
    w->setWindowTitle(filePath + " - " + IDEMAN);
    cenTabWidget->setVisible(true);
    if (fileName.right(2) == ".c")
    {
        cenTabWidget->setTabIcon(curId, QIcon(":/icons/icons/c.ico"));
    }
    else if (fileName.right(2) == ".h")
    {
        cenTabWidget->setTabIcon(curId, QIcon(":/icons/icons/head.ico"));
    }
    else if (fileName.right(4) == ".cpp")
    {
        cenTabWidget->setTabIcon(curId, QIcon(":/icons/icons/cpp.ico"));
    }
    else if (fileName.right(2) == ".S" || fileName.right(2) == ".s")
    {
        cenTabWidget->setTabIcon(curId, QIcon(":/icons/icons/asm.ico"));
    }
    else
    {
        cenTabWidget->setTabIcon(curId, QIcon(":/icons/icons/c.ico"));
    }
}

// click new file
void MainWindow::click_menu_new_file()
{
    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();
    QTreeWidgetItem *parItem = new QTreeWidgetItem;
    QString root;
    QTreeWidgetItem *rootItem;
    while (parItem != NULL)
    {
        root = curItem->data(0, Qt::ToolTipRole).toString();
        rootItem = curItem;
        parItem = curItem->parent();
        curItem = parItem;
    }
    //qDebug() << root;

    new_file_dialog = new NewFileDialog;
    int result = new_file_dialog->exec();

    QString path = new_file_dialog->newFileLocation;
    QString fileName = new_file_dialog->newFileNameLineEdit->text();
    QString filePath = path + "/" + fileName;
    if (result == QDialog::Accepted)
    {
        curItem = traverse_tree_widget_item(rootItem, 0, path);
        //qDebug() << curItem->text(0);
        tree_widget_add_item(curItem, filePath, fileName);
        display_file_content_in_tab_widget(filePath, fileName);
    }
    else if (result == QDialog::Rejected)
    {
        // no operation
    }
    else
    {
        // no operation
    }
}

// click new project
void MainWindow::click_menu_new_project()
{
    wizard->show();
    wizard->setWindowTitle("New Project");
    wizard->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));
}

// click open folder
void MainWindow::click_menu_open_folder()
{
    openfolderuiWidget->show();
    openfolderuiWidget->setWindowTitle("Open Folder");
    openfolderuiWidget->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));
    openfolderuiWidget->setMinimumSize(450, 300);
}

// click save to save file
void MainWindow::click_menu_save()
{
    int curId = cenTabWidget->currentIndex();
    //qDebug() << curId;

    QString filePath = cenTabWidget->tabToolTip(curId);
    QString name = cenTabWidget->tabText(curId);
    QStringList nameList;
    if (name.contains("*"))
    {
        nameList = name.split("*");
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write(ce[curId]->toPlainText().toUtf8());
            file.close();
            cenTabWidget->setTabText(curId, nameList[0]);
        }
        w->openFlag = 1;
    }

}

void MainWindow::right_click_custom_context_menu_requested(const QPoint &pos)
{
    popMenu = new QMenu(this);
//    popMenu->clear(); // clear original menu

    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();// 得到所点之item
    QTreeWidgetItem *parItem = new QTreeWidgetItem;
    QTreeWidgetItem *preItem = new QTreeWidgetItem;

//    QTreeWidgetItem *item = prjTreeWidget->itemAt(pos);
//    qDebug() << prjTreeWidget->indexOfTopLevelItem(curItem);// top level >= 0 , child < 0
//    qDebug() << curItem->text(0) << prjTreeWidget->currentIndex();

    parItem = curItem->parent();
    if (parItem == NULL)
    {
        prjRootPath = curItem->data(0, Qt::ToolTipRole).toString();
    }
    else
    {
        while (parItem != NULL)
        {
            preItem = parItem;
            parItem = parItem->parent();
        }
        prjRootPath = preItem->data(0, Qt::ToolTipRole).toString();
    }

    if (curItem == NULL)
    {
//        qDebug() << "Empty space.";
        return;
    }
    else
    {
        newFileAction = new QAction(QStringLiteral("New File"), this);
        newFolderAction = new QAction(QStringLiteral("New Folder"), this);
        deleteAction = new QAction(QStringLiteral("Delete"), this);
        importAction = new QAction(QStringLiteral("Import"), this);
        refreshAction = new QAction(QStringLiteral("Refresh"), this);

        newFileAction->setIcon(QIcon(":/icons/icons/file.svg"));

        connect(newFileAction, SIGNAL( triggered() ), this,
                SLOT( proj_tree_widget_new_file_action() )
                );

        connect(newFolderAction, SIGNAL( triggered() ), this,
                SLOT( proj_tree_widget_new_folder_action() )
                );

        connect(deleteAction, SIGNAL( triggered() ), this,
                SLOT( proj_tree_widget_delete_action() )
                );

        connect(importAction, SIGNAL( triggered() ), this,
                SLOT( proj_tree_widget_import_action() )
                );

        connect(refreshAction, SIGNAL( triggered() ), this,
                SLOT( proj_tree_widget_refresh_action() )
                );

        popMenu->addAction(newFileAction);
        popMenu->addAction(newFolderAction);
        popMenu->addAction(deleteAction);
        popMenu->addAction(importAction);
        popMenu->addAction(refreshAction);

        popMenu->exec(prjTreeWidget->viewport()->mapToGlobal(pos));

    }
}

// new file under the specified item
void MainWindow::proj_tree_widget_new_file_action()
{
    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();

    if (curItem->data(0, Qt::UserRole).toString() == "file")
    {
        return;
    }

    newFileDialog = new NewFile;
    int result = newFileDialog->exec();

    if (result == QDialog::Accepted)
    {
        QString fileName = newFileDialog->newFileNameLineEdit->text();
        QString filePath = curItem->data(0, Qt::ToolTipRole).toString() + "/" + fileName;
        tree_widget_add_item(curItem, filePath, fileName);
        display_file_content_in_tab_widget(filePath, fileName);
    }
    else if (result == QDialog::Rejected)
    {

    }
    else
    {

    }
}

void MainWindow::proj_tree_widget_new_folder_action()
{
    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();
    new_folder_dialog = new NewFolderDialog;
    int result = new_folder_dialog->exec();

    if (result == QDialog::Accepted)
    {

        QTreeWidgetItem *newItem = new QTreeWidgetItem;
        newItem->setText(0, new_folder_dialog->newFolderNameLineEdit->text());
        newItem->setData(0, Qt::UserRole, "folder");
        newItem->setData(0, Qt::ToolTipRole, new_folder_dialog->newFolderLocation);
        newItem->setIcon(0, QIcon(":/icons/icons/folder.ico"));
        curItem->addChild(newItem);
        curItem->setExpanded(true);

        // create new folder
        QString dst = new_folder_dialog->newFolderLocationLineEdit->text() + "/" +
                      new_folder_dialog->newFolderNameLineEdit->text();
        //qDebug() << dst;
        QDir dstDir;
        if (!dstDir.exists(dst))
        {
            dstDir.mkpath(dst);
        }
    }
    else if (result == QDialog::Rejected)
    {

    }
    else
    {

    }
}

void MainWindow::proj_tree_widget_delete_action()
{

    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();
    QString type = curItem->data(0, Qt::UserRole).toString();
    QString name = curItem->text(0);
    QString filePath = curItem->data(0, Qt::ToolTipRole).toString();
    int index = 0;
    if (curItem != NULL)
    {
        // 删除节点
        delete curItem;
        // 删除文件
        if (type == "file")
        {
            QFile file(filePath);
            bool result = file.remove();
            if (result)
            {
//                qDebug() << "remove successfully";
                w->openFlag = 1;
                int count = cenTabWidget->count();
                for (int i = 0; i < count; i++)
                {
                    if (cenTabWidget->tabToolTip(i) == filePath)
                    {
                        index = i;
                    }
                }
//                qDebug() << cenTabWidget->tabText(index);
                QString newName = cenTabWidget->tabText(index) + "*";
                cenTabWidget->setTabText(index, newName);
            }
            else
            {
                // QMessageBox
            }
        }
        else if (type == "folder")
        {
            QDir dir(filePath);
            dir.removeRecursively();
        }
        // 调取对话框来实现目录的删除，否则只在界面上删除
    }
}

void MainWindow::proj_tree_widget_import_action()
{
    importWidget->setFixedSize(400, 480);
    QDesktopWidget *desk = QApplication::desktop();
    importWidget->move((desk->width() - importWidget->width())/2, (desk->height() - importWidget->height())/2);
    importWidget->show();
    importWidget->setWindowTitle("Import project");
    importWidget->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));
}

void MainWindow::remove_item(QTreeWidgetItem *item, QString path = "")
{
    int count = item->childCount();
    qDebug() << path;
    qDebug() << item->text(0);
    qDebug() << "count: " << count;

    while (item->childCount() > 0)
    {
        remove_item(item->child(0), path);
    }

    qDebug() << item->data(0, Qt::ToolTipRole).toString();
    qDebug() << path;
    if (item->data(0, Qt::ToolTipRole).toString() != path)
    {
        if (count == 0)
        {
            delete item;
        }
    }
}

// Refresh project
void MainWindow::proj_tree_widget_refresh_action()
{
    QTreeWidgetItem *curItem = prjTreeWidget->currentItem();
    QTreeWidgetItem *parItem = new QTreeWidgetItem;
    QString root;
    QTreeWidgetItem *rootItem;
    while (parItem != NULL)
    {
        root = curItem->data(0, Qt::ToolTipRole).toString();
        rootItem = curItem;
        parItem = curItem->parent();
        curItem = parItem;
    }
    qDebug() << root << " " << rootItem->text(0);

    // remove rootItem
    remove_item(rootItem, prjRootPath);

    openfolderuiWidget->list_dirs(rootItem, root);
}

// treewidget double click event
void MainWindow::double_click_item_handler(QTreeWidgetItem *item, int)
{
    QString type = item->data(0, Qt::UserRole).toString();
    QString fileName = item->text(0);
    if (type == "file")
    {
        QString filePath = item->data(0, Qt::ToolTipRole).toString();
        // judge if the same file is selected.
        for (int i = 0; i < cenTabWidget->count(); i++)
        {
            if (cenTabWidget->tabToolTip(i) == filePath)
            {
                cenTabWidget->setCurrentIndex(i);
                w->setWindowTitle(filePath + " - " + IDEMAN);
                return;
            }
        }

        display_file_content_in_tab_widget(filePath, fileName);
    }
}

void MainWindow::tab_bar_changed_event(int index)
{
    //qDebug() << index;
    if (index < 0)
    {
        w->setWindowTitle(IDEMAN);
    }
    else
    {
        QString filePath = cenTabWidget->tabToolTip(index);
        w->setWindowTitle(filePath + " - " + IDEMAN);
    }
    w->openFlag = 1;
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRectF rectangle(0, 20, 50, 80);
    painter.drawEllipse(rectangle);
    event->accept();
}

// PaintWidget的构造函数
PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
