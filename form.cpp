/*******************************************************
 *
 * @file        form.cpp
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/10
 *
 ******************************************************/

#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"

Device *device_select_widget;
extern Form *wizard;
extern MainWindow *w;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    device_select_widget = new Device;

    QVBoxLayout *new_widget_layout = new QVBoxLayout;
    new_widget_layout->setContentsMargins(5, 5, 5, 5);
    new_widget_layout->setSpacing(8);

    // label
    QLabel *label0 = new QLabel;
    label0->setText("Choose a target device");
    new_widget_layout->addWidget(label0, 0, Qt::AlignTop);

    // line
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);
    new_widget_layout->addWidget(line, 0, Qt::AlignTop);

    new_widget_layout->addStretch();

    // label
    QLabel *label1 = new QLabel;
    label1->setText("Device");
    new_widget_layout->addWidget(label1, 0, Qt::AlignTop);


    // lineEdit
    lineEdit = new QLineEdit;
    lineEdit->setReadOnly(true);
    lineEdit->setStyleSheet("QLineEdit:read-only { background-color: rgb(224, 224, 224); }");
    // pushbutton
    QPushButton *searchBtn = new QPushButton;
    QSize size = QSize(40, lineEdit->sizeHint().height());
    searchBtn->setText("...");
    searchBtn->setMinimumSize(size);
    searchBtn->setMaximumSize(size);
    QHBoxLayout *select_device_hlayout = new QHBoxLayout;
    select_device_hlayout->setContentsMargins(0, 0, 0, 0);
    select_device_hlayout->addStretch();
    select_device_hlayout->addWidget(searchBtn);
    lineEdit->setLayout(select_device_hlayout); // widget setLayout(QV/QH)
    lineEdit->setTextMargins(0, 1, size.width(), 1);
    lineEdit->setReadOnly(true); // 只读
    new_widget_layout->addWidget(lineEdit);

    // label
    QLabel *packageLabel = new QLabel;
    packageLabel->setText("Package");
    new_widget_layout->addWidget(packageLabel);

    // lineEdit
    packageLineEdit = new QLineEdit;
    packageLineEdit->setReadOnly(true);
    packageLineEdit->setStyleSheet("QLineEdit:read-only { background-color: rgb(224, 224, 224); }");
    new_widget_layout->addWidget(packageLineEdit);

    // label
    QLabel *ideLabel = new QLabel;
    ideLabel->setText("IDE");
    new_widget_layout->addWidget(ideLabel);

    // combobox
    QComboBox *ideComboBox = new QComboBox;
    ideComboBox->addItem("GCC");
    ideComboBox->addItem("IAR");
    ideComboBox->addItem("Keil μVision5");
    ideComboBox->addItem("Eclipse");
    ideComboBox->addItem("Green Hills");
    new_widget_layout->addWidget(ideComboBox);

    // label
    projectNameLabel = new QLabel;
    projectNameLabel->setText("Project Name:");
    new_widget_layout->addWidget(projectNameLabel);

    // lineEdit
    projectNameLineEdit = new QLineEdit;
    new_widget_layout->addWidget(projectNameLineEdit);

    // Project Path label
    projPathLabel = new QLabel;
    projPathLabel->setText("Project Path");
    new_widget_layout->addWidget(projPathLabel);

    // Project Path lineEdit
    projPathLineEdit = new QLineEdit;
    QPushButton *browsePathBtn = new QPushButton;
    size = QSize(40, projPathLineEdit->sizeHint().height());
    browsePathBtn->setText("...");
    browsePathBtn->setMinimumSize(size);
    browsePathBtn->setMaximumSize(size);
    browsePathBtn->setCursor(Qt::ArrowCursor);// 标准箭头
    QHBoxLayout *projPathHLayout = new QHBoxLayout;
    projPathHLayout->setContentsMargins(0, 0, 0, 0);
    projPathHLayout->addStretch();
    projPathHLayout->addWidget(browsePathBtn);
    projPathLineEdit->setLayout(projPathHLayout);
    projPathLineEdit->setTextMargins(0, 1, size.width(), 1);
    new_widget_layout->addWidget(projPathLineEdit);

    // label
    QLabel *sdkPackageLabel = new QLabel;
    sdkPackageLabel->setText("SDK Package");
    new_widget_layout->addWidget(sdkPackageLabel);

    // lineEdit
    sdkPackagePathLineEdit = new QLineEdit;
    QPushButton *browseBtn = new QPushButton;
    size = QSize(40, sdkPackagePathLineEdit->sizeHint().height());
    browseBtn->setText("...");
    browseBtn->setMinimumSize(size);
    browseBtn->setMaximumSize(size);
    browseBtn->setCursor(Qt::ArrowCursor);
    QHBoxLayout *sdkPackagePathHLayout = new QHBoxLayout;
    sdkPackagePathHLayout->setContentsMargins(0, 0, 0, 0);
    sdkPackagePathHLayout->addStretch();
    sdkPackagePathHLayout->addWidget(browseBtn);// QHBoxLayout->addwidget(QPushButton)  QLineEdit->setLayout(QHBoxLayout)
    sdkPackagePathLineEdit->setLayout(sdkPackagePathHLayout);
    sdkPackagePathLineEdit->setTextMargins(0, 1, size.width(), 1);
    new_widget_layout->addWidget(sdkPackagePathLineEdit);

    // stretch
    new_widget_layout->addStretch(10);

    // button
    QHBoxLayout *new_finish_widget_layout = new QHBoxLayout;
    new_finish_widget_layout->addStretch();
    QPushButton *finishBtn = new QPushButton;
    finishBtn->setText("Finish");
    new_finish_widget_layout->addWidget(finishBtn);
    new_widget_layout->addItem(new_finish_widget_layout);

    setLayout(new_widget_layout);

    connect(searchBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( select_device_clicked(void) )
            );

    connect(browsePathBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( choose_project_path_browse_button_clicked(bool) )
            );

    connect(browseBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( browse_button_clicked(bool) )
            );

    connect(finishBtn,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( finish_button_clicked(bool) )
            );
}



void Form::select_device_clicked(void)
{
    device_select_widget->show();
    device_select_widget->setWindowTitle("Target device settings");
}

void Form::choose_project_path_browse_button_clicked(bool)
{
    projPath = QFileDialog::getExistingDirectory(this, "Choose Project Path", "C:\\");

    if (!projPath.isEmpty())
    {
        projPathLineEdit->setText(projPath);
    }
}

void Form::browse_button_clicked(bool)
{
    sdkPackage = QFileDialog::getOpenFileName(this, "Open File", "C:\\", tr("Zip (*.zip *.7z *.rar)"));

    if (!sdkPackage.isEmpty())
    {
        sdkPackagePathLineEdit->setText(sdkPackage);
    }
}

void Form::finish_button_clicked(bool)
{
    qDebug() << wizard->projectNameLineEdit->text();
    // 创建工程目录
    QString prjPath;
    prjPath = wizard->projPathLineEdit->text()+ "/" + wizard->projectNameLineEdit->text();
    QDir prjDir(prjPath);
    if (prjDir.exists())
    {
        qDebug() << "dir exists.";
        wizard->projectNameLabel->setText("Project Name Exists!!!");
        wizard->projectNameLabel->setStyleSheet("QLabel{color:red}");
    }
    else
    {
        wizard->projectNameLabel->setText("Project Name:");
        wizard->projectNameLabel->setStyleSheet("QLabel{color:black}");
        QStringList prjName;
        prjName << wizard->projectNameLineEdit->text();
        QTreeWidgetItem *prjWidgetItem = new QTreeWidgetItem(w->prjTreeWidget, prjName);
        prjWidgetItem->setIcon(0, QIcon(":/icons/icons/chuizi_48.png"));
        QFont fnt = prjWidgetItem->font(0);
        fnt.setBold(true);
        prjWidgetItem->setFont(0, fnt);
        prjWidgetItem->setData(0, Qt::ToolTipRole, prjPath);
        prjWidgetItem->setData(0, Qt::UserRole, "project");
        w->prjTreeWidget->addTopLevelItem(prjWidgetItem);//设置顶层
        //w->prjTreeWidget->expandAll();

        // 创建工程目录
        prjDir.mkdir(prjPath);

        this->close();
    }
}

Form::~Form()
{
    delete ui;
}
