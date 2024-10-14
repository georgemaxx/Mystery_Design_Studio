/*******************************************************
 *
 * @file        openfolderui.cpp
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/10/01
 *
 ******************************************************/

#include "openfolderui.h"
#include "common.h"
#include "mainwindow.h"

extern MainWindow *w;

OpenFolder::OpenFolder(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(5);

    // label0
    QLabel *label0 = new QLabel;
    label0->setText("Open Folder");
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    label0->setFont(f);
    vLayout->addWidget(label0);

    // separate line
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    vLayout->addWidget(line, 0, Qt::AlignTop);

    // label1
    label1 = new QLabel;
    label1->setText("Project Folder");
    vLayout->addWidget(label1);

    // lineEdit
    prjFolderLineEdit = new QLineEdit;
    QPushButton *browseBtn = new QPushButton;
    QSize size = QSize(40, prjFolderLineEdit->sizeHint().height());
    browseBtn->setText("...");
    browseBtn->setMinimumSize(size);
    browseBtn->setMaximumSize(size);
    browseBtn->setCursor(Qt::ArrowCursor);
    QHBoxLayout *sdkPackagePathHLayout = new QHBoxLayout;
    sdkPackagePathHLayout->setContentsMargins(0, 0, 0, 0);
    sdkPackagePathHLayout->addStretch();
    sdkPackagePathHLayout->addWidget(browseBtn);// QHBoxLayout->addwidget(QPushButton)  QLineEdit->setLayout(QHBoxLayout)
    prjFolderLineEdit->setLayout(sdkPackagePathHLayout);
    prjFolderLineEdit->setTextMargins(0, 1, size.width(), 1);
    vLayout->addWidget(prjFolderLineEdit);
    //browse button click
    connect(browseBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( browse_button_click_event() )
            );

    connect(prjFolderLineEdit,
            SIGNAL( textChanged(QString) ),
            this,
            SLOT( text_changed_event(QString) )
            );


    vLayout->addStretch(10);

    // pushbutton
    QHBoxLayout *pushBtnLayout = new QHBoxLayout;
    btn_ok = new QPushButton;
    btn_ok->setText("OK");
    btn_ok->setFixedWidth(BUTTON_WIDTH);
    QPushButton *btn_cancel = new QPushButton;
    btn_cancel->setText("Cancel");
    btn_cancel->setFixedWidth(BUTTON_WIDTH);
    pushBtnLayout->addStretch(5);
    pushBtnLayout->addWidget(btn_ok, Qt::AlignRight);
    pushBtnLayout->addWidget(btn_cancel, Qt::AlignRight);
    vLayout->addLayout(pushBtnLayout);
    connect(btn_ok,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( ok_button_click_event(bool) )
            );

    connect(btn_cancel,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( cancel_button_click_event(bool) )
            );


    setLayout(vLayout);
}

// when text in the prjFolderLineEdit is changed
void OpenFolder::text_changed_event(QString)
{
    // count of treewidget top level item
    int cnt = w->prjTreeWidget->topLevelItemCount();
    for (int i = 0; i < cnt; i++)
    {
#if (DEBUG)
        qDebug() << w->prjTreeWidget->topLevelItem(i)->text(0);
        qDebug() << w->prjTreeWidget->topLevelItem(i)->data(0, Qt::UserRole).toString();
        qDebug() << w->prjTreeWidget->topLevelItem(i)->data(0, Qt::ToolTipRole).toString();
        qDebug() << w->prjTreeWidget->topLevelItem(i)->child(0)->text(0);
#endif
        int result = QString::compare(prjFolderLocation,
                                      w->prjTreeWidget->topLevelItem(i)->data(0, Qt::UserRole).toString(),
                                      Qt::CaseSensitive);
        // if two strings are same, result is equal to 0
        if (result == 0)
        {
            // display prj folder has already been displayed
            label1->setText("Project Folder already exist!!!");
            // the color change to red
            label1->setStyleSheet("QLabel{color:red}");
            btn_ok->setEnabled(false);
        }
        else
        {
            // restore
            label1->setText("Project Folder");
            label1->setStyleSheet("QLabel{color:black}");
            btn_ok->setEnabled(true);
        }
    }
}

// project folder location browse button click event
void OpenFolder::browse_button_click_event()
{
    QString path;
    if (prjFolderLineEdit->text().isEmpty())
    {
        path = "C:\\";
    }
    else
    {
        path = prjFolderLineEdit->text();
    }
    prjFolderLocation = QFileDialog::getExistingDirectory(this, "Choose Project Folder", path);
    if (!prjFolderLocation.isEmpty())
    {
        prjFolderLineEdit->setText(prjFolderLocation);
    }
}

void OpenFolder::list_dirs(QTreeWidgetItem *tw, const QString &path)
{
    QDir dir(path);

    QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const QString &entry : entries)
    {
        QString fullPath = dir.filePath(entry);
#if (DEBUG)
        qDebug() << "Entry:" << entry;
#endif
        QTreeWidgetItem *entryItem = new QTreeWidgetItem;
        if (QFileInfo(fullPath).isDir())
        {
#if (DEBUG)
            qDebug() << "Directory:" << fullPath;
#endif
            entryItem->setText(0, entry);
            entryItem->setIcon(0, QIcon(":/icons/icons/folder.ico"));
            entryItem->setData(0, Qt::UserRole, "folder");
            entryItem->setData(0, Qt::ToolTipRole, fullPath);
            entryItem->setText(1, "folder");
            tw->addChild(entryItem);
            this->list_dirs(entryItem, fullPath);
        }
        else
        {
            entryItem->setText(0, entry);
            entryItem->setData(0, Qt::ToolTipRole, fullPath);
            entryItem->setText(1, "file");
            QString s = entry;
            if (s.right(2) == ".c")
            {
                entryItem->setIcon(0, QIcon(":/icons/icons/c.ico"));
            }
            else if (s.right(2) == ".h")
            {
                entryItem->setIcon(0, QIcon(":/icons/icons/head.ico"));
            }
            else if (s.right(4) == ".cpp")
            {
                entryItem->setIcon(0, QIcon(":/icons/icons/cpp.ico"));
            }
            else if (s.right(2) == ".S" || s.right(2) == ".s")
            {
                entryItem->setIcon(0, QIcon(":/icons/icons/asm.ico"));
            }
            else
            {
                entryItem->setIcon(0, QIcon(":/icons/icons/c.ico"));
            }
            entryItem->setData(0, Qt::UserRole, "file");
            tw->addChild(entryItem);
#if (DEBUG)
            qDebug() << "File:" << fullPath;
#endif
        }
    }

}

// ok button click event
void OpenFolder::ok_button_click_event(bool)
{
    // display in the treewidget
    QString path = prjFolderLocation;
    QStringList pl = path.split("/");
    qDebug() << pl.last();
    QStringList prjName;
    prjName << pl.last() << "project";
    QTreeWidgetItem *prjItem = new QTreeWidgetItem(w->prjTreeWidget, prjName);
    prjItem->setIcon(0, QIcon(":/icons/icons/chuizi_48.png"));
    w->prjTreeWidget->addTopLevelItem(prjItem);
    QFont fnt = prjItem->font(0);
    fnt.setBold(true);
    prjItem->setFont(0, fnt);
    prjItem->setData(0, Qt::UserRole, "project");
    prjItem->setData(0, Qt::ToolTipRole, path);

    // display the child of top level widget, traverse the project folder
    this->list_dirs(prjItem, path);

    // close widget
    this->close();
}

// cancel button click event
void OpenFolder::cancel_button_click_event(bool)
{
    // close widget
    this->close();
}

OpenFolder::~OpenFolder()
{

}
