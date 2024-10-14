#include "newfile.h"
#include "mainwindow.h"

extern MainWindow *w;

NewFile::NewFile(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Create New File");
    this->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));
    QPoint globalPos = w->mapToGlobal(QPoint(0, 0)); // parent window 绝对坐标
    this->setGeometry(0, 0, 345, 171);
    int x = globalPos.x() + (w->width() - this->width()) / 2;
    int y = globalPos.y() + (w->height() - this->height()) / 2;
    this->move(x, y);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(5);

    // label0
    QLabel *label0 = new QLabel;
    label0->setText("File");
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    label0->setFont(f);
    vLayout->addWidget(label0);

    // label1
    label1 = new QLabel;
    label1->setText(" Create a new file resource.");
    vLayout->addWidget(label1);

    // separate line
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    vLayout->addWidget(line, 0, Qt::AlignTop);

    // label2
    QLabel *label2 = new QLabel;
    label2->setText("File name:");
    vLayout->addWidget(label2);

    // new file name line edit <---> label2
    newFileNameLineEdit = new QLineEdit;
    connect(newFileNameLineEdit,
            SIGNAL( textEdited(QString) ),
            this,
            SLOT( text_edit_event(QString) )
            );
    vLayout->addWidget(newFileNameLineEdit);


    // 空
    vLayout->addStretch(10);

    // buttonbox
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    vLayout->addWidget(buttonBox);

    this->setLayout(vLayout);

}

void NewFile::text_edit_event(QString)
{
    bool state;
    QTreeWidgetItem *curItem = w->prjTreeWidget->currentItem();
    QRegExp regx(".*[^.]$");
    state = regx.exactMatch(newFileNameLineEdit->text());
    if (state)
    {
        //qDebug() << "text is edited:" << newFileNameLineEdit->text();
        label1->setText(" Create a new file resource.");
        label1->setStyleSheet("QLabel{color:black}");
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        label1->setText(" Invalid name");
        label1->setStyleSheet("QLabel{color:red}");
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

    QFileInfo fileInfo(curItem->data(0, Qt::ToolTipRole).toString() + "/" + newFileNameLineEdit->text());
    if (fileInfo.isFile())
    {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        label1->setText(" New file exists!");
        label1->setStyleSheet("QLabel{color:red}");
    }
    else
    {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}

NewFile::~NewFile()
{

}

