#include "newfolderdialog.h"
#include "ui_newfolderdialog.h"
#include "mainwindow.h"

extern MainWindow *w;

NewFolderDialog::NewFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolderDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Create New Folder");
    this->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(5);

    // label0
    QLabel *label0 = new QLabel;
    label0->setText("Folder");
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    label0->setFont(f);
    vLayout->addWidget(label0);

    // label1
    label1 = new QLabel;
    label1->setText(" Create a new folder.");
    vLayout->addWidget(label1);

    // separate line
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    vLayout->addWidget(line, 0, Qt::AlignTop);



    // label2
    QLabel *label2 = new QLabel;
    label2->setText("Folder name:");
    vLayout->addWidget(label2);

    // new file name line edit <---> label2
    newFolderNameLineEdit = new QLineEdit;
    connect(newFolderNameLineEdit,
            SIGNAL( textEdited(QString) ),
            this,
            SLOT( text_edit_event(QString) )
            );
    vLayout->addWidget(newFolderNameLineEdit);


    // label3
    QLabel *label3 = new QLabel;
    label3->setText("Folder location");
    vLayout->addWidget(label3);

    newFolderLocationLineEdit = new QLineEdit;
    //newFolderLocationLineEdit->setText(w->prjRootPath);
    QPushButton *browseBtn = new QPushButton;
    QSize size = QSize(40, newFolderLocationLineEdit->sizeHint().height());
    browseBtn->setText("...");
    browseBtn->setMinimumSize(size);
    browseBtn->setMaximumSize(size);
    browseBtn->setCursor(Qt::ArrowCursor);
    QHBoxLayout *sdkPackagePathHLayout = new QHBoxLayout;
    sdkPackagePathHLayout->setContentsMargins(0, 0, 0, 0);
    sdkPackagePathHLayout->addStretch();
    sdkPackagePathHLayout->addWidget(browseBtn);// QHBoxLayout->addwidget(QPushButton)  QLineEdit->setLayout(QHBoxLayout)
    newFolderLocationLineEdit->setLayout(sdkPackagePathHLayout);
    newFolderLocationLineEdit->setTextMargins(0, 1, size.width(), 1);
    vLayout->addWidget(newFolderLocationLineEdit);

    connect(newFolderLocationLineEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(text_changed_event(QString))
            );

    connect(browseBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( browse_button_clicked(bool) )
            );

    // 空
    vLayout->addStretch(10);

    vLayout->addWidget(ui->buttonBox);

    this->setLayout(vLayout);
}

void NewFolderDialog::judge_folder_exist(void)
{
    QString newFolderPath;
    newFolderPath = newFolderLocationLineEdit->text() + "/" + newFolderNameLineEdit->text();
    QDir dir(newFolderPath);
    if (dir.exists())
    {
        label1->setText("New folder exists.");
        label1->setStyleSheet("QLabel{color:red}");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        label1->setText("Create a new folder.");
        label1->setStyleSheet("QLabel{color:black}");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void NewFolderDialog::text_edit_event(QString)
{
    judge_folder_exist();
}

void NewFolderDialog::text_changed_event(QString)
{
    judge_folder_exist();
}

void NewFolderDialog::browse_button_clicked(bool)
{
    newFolderLocation = QFileDialog::getExistingDirectory(this, "Choose New Folder Location", w->prjRootPath);
    if (!newFolderLocation.isEmpty())
    {
        // set new folder path
        newFolderLocationLineEdit->setText(newFolderLocation);
    }
}

NewFolderDialog::~NewFolderDialog()
{
    delete ui;
}
