#include "import.h"
#include "common.h"

Import::Import(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->setContentsMargins(5, 5, 5, 5);
    vLayout->setSpacing(8);

    // label0
    QLabel *label0 = new QLabel;
    label0->setText("Import");
    QFont f;
    f.setBold(true);
    f.setPointSize(10);
    label0->setFont(f);
    vLayout->addWidget(label0);

    // label1
    QLabel *label1 = new QLabel;
    label1->setText(" Import a new project.");
    vLayout->addWidget(label1);

    // separate line
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    vLayout->addWidget(line, 0, Qt::AlignTop);

    QLabel *label2 = new QLabel;
    label2->setText("Project location:");
    vLayout->addWidget(label2);

    // lineEdit
    prjLocationLineEdit = new QLineEdit;
    //newFolderLocationLineEdit->setText(w->prjRootPath);
    QPushButton *browseBtn = new QPushButton;
    QSize size = QSize(40, prjLocationLineEdit->sizeHint().height());
    browseBtn->setText("...");
    browseBtn->setMinimumSize(size);
    browseBtn->setMaximumSize(size);
    browseBtn->setCursor(Qt::ArrowCursor);
    QHBoxLayout *sdkPackagePathHLayout = new QHBoxLayout;
    sdkPackagePathHLayout->setContentsMargins(0, 0, 0, 0);
    sdkPackagePathHLayout->addStretch();
    sdkPackagePathHLayout->addWidget(browseBtn);// QHBoxLayout->addwidget(QPushButton)  QLineEdit->setLayout(QHBoxLayout)
    prjLocationLineEdit->setLayout(sdkPackagePathHLayout);
    prjLocationLineEdit->setTextMargins(0, 1, size.width(), 1);
    vLayout->addWidget(prjLocationLineEdit);

    vLayout->addStretch(10);

    // button
    QHBoxLayout *new_finish_widget_layout = new QHBoxLayout;
    new_finish_widget_layout->addStretch();
    QPushButton *finishBtn = new QPushButton;
    finishBtn->setText("Finish");
    new_finish_widget_layout->addWidget(finishBtn);
    vLayout->addItem(new_finish_widget_layout);
    setLayout(vLayout);

    connect(browseBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( on_browse_button_clicked(bool) )
            );

    connect(finishBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( on_finish_button_clicked(bool) )
            );
}

void Import::on_browse_button_clicked(bool)
{
    if (prjLocationLineEdit->text().isEmpty())
    {
        prjLocation = QFileDialog::getExistingDirectory(this, "Import Project Location", "C:\\");
    }
    else
    {
        prjLocation = QFileDialog::getExistingDirectory(this, "Import Project Location", prjLocationLineEdit->text());
    }

    if (!prjLocation.isEmpty())
    {
        // set new folder path
        prjLocationLineEdit->setText(prjLocation);
    }
}

void Import::on_finish_button_clicked(bool)
{
    this->close();

    // display in the treewidget
}

Import::~Import()
{

}
