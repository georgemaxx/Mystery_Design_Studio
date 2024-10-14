#include "newfiledialog.h"
#include "ui_newfiledialog.h"
#include "mainwindow.h"

extern MainWindow *w;
extern NewFileDialog *new_file_dialog;

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Create New File");
    this->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));

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

    // label3
    QLabel *label3 = new QLabel;
    label3->setText("File location");
    vLayout->addWidget(label3);

    newFileLocationLineEdit = new QLineEdit;
    //newFileLocationLineEdit->setText(w->prjRootPath);
    QPushButton *browseBtn = new QPushButton;
    QSize size = QSize(40, newFileLocationLineEdit->sizeHint().height());
    browseBtn->setText("...");
    browseBtn->setMinimumSize(size);
    browseBtn->setMaximumSize(size);
    browseBtn->setCursor(Qt::ArrowCursor);
    QHBoxLayout *sdkPackagePathHLayout = new QHBoxLayout;
    sdkPackagePathHLayout->setContentsMargins(0, 0, 0, 0);
    sdkPackagePathHLayout->addStretch();
    sdkPackagePathHLayout->addWidget(browseBtn);// QHBoxLayout->addwidget(QPushButton)  QLineEdit->setLayout(QHBoxLayout)
    newFileLocationLineEdit->setLayout(sdkPackagePathHLayout);
    newFileLocationLineEdit->setTextMargins(0, 1, size.width(), 1);
    vLayout->addWidget(newFileLocationLineEdit);

    connect(newFileLocationLineEdit,
            SIGNAL( textChanged(QString) ),
            this,
            SLOT( text_changed_event(QString) )
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

void NewFileDialog::text_edit_event(QString)
{
    bool state;
    QRegExp regx(".*[^.]$");
    state = regx.exactMatch(newFileNameLineEdit->text());
    if (state)
    {
        //qDebug() << "text is edited:" << newFileNameLineEdit->text();
        label1->setText(" Create a new file resource.");
        label1->setStyleSheet("QLabel{color:black}");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        label1->setText(" Invalid name");
        label1->setStyleSheet("QLabel{color:red}");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

    QFileInfo fileInfo(newFileLocationLineEdit->text() + "/" + newFileNameLineEdit->text());
    if (fileInfo.isFile())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        label1->setText(" New file exists!");
        label1->setStyleSheet("QLabel{color:red}");
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

}

void NewFileDialog::text_changed_event(QString)
{
    QFileInfo fileInfo(newFileLocationLineEdit->text() + "/" + newFileNameLineEdit->text());
    if (fileInfo.isFile())
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        label1->setText(" New file exists!");
        label1->setStyleSheet("QLabel{color:red}");
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        label1->setText(" Create a new file resource.");
        label1->setStyleSheet("QLabel{color:black}");
    }
}

void NewFileDialog::browse_button_clicked(bool)
{
    // 显示路径
    newFileLocation = QFileDialog::getExistingDirectory(this, "Choose New File Location", w->prjRootPath);

    if (!newFileLocation.isEmpty())
    {
        // set new file path
        newFileLocationLineEdit->setText(newFileLocation);
    }
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}
