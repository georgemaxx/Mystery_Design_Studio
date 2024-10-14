/*******************************************************
 *
 * @file        device.cpp
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/10
 *
 ******************************************************/

#include "device.h"
#include "ui_device.h"
#include "form.h"

extern Form *wizard;
int r;

Device::Device(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowCloseButtonHint);// 取消最大化窗口

    QVBoxLayout *tarDevSettingLayout = new QVBoxLayout;
    tarDevSettingLayout->setContentsMargins(5, 5, 5, 5);
    tarDevSettingLayout->setSpacing(8);

    // label
    QHBoxLayout *labelLayout = new QHBoxLayout;
    QLabel *label1 = new QLabel;
    label1->setText("Selected device:");
    deviceLabel = new QLabel;
    deviceLabel->setText("");
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setFixedWidth(100);
    labelLayout->addWidget(label1, Qt::AlignLeft);
    labelLayout->addWidget(deviceLabel, Qt::AlignLeft);
    labelLayout->addStretch(25);
    labelLayout->addWidget(lineEdit, Qt::AlignRight); // 放置textEdit

    // 读取xml文件
    QFile xml_file("../conf_tool/devices/le0.xml");
    device_t dev;
    if (xml_file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader *xml = new QXmlStreamReader(&xml_file);
        while (!xml->atEnd() && !xml->hasError())
        {
            QXmlStreamReader::TokenType type = xml->readNext();
            if (type == QXmlStreamReader::EndElement && xml->name()=="device")
            {
                break;
            }

            //qDebug() << type;
            if (xml->isStartElement()) // 判断是否是节点的开始
            {
                if (xml->name() == "info")
                {
                    qDebug() << xml->attributes().value("name").toString();
                    qDebug() << xml->attributes().value("vendor").toString();
                    qDebug() << xml->attributes().value("core").toString();
                    dev.vendor = xml->attributes().value("vendor").toString();
                    dev.tarDevice = xml->attributes().value("name").toString();
                    dev.core = xml->attributes().value("core").toString();
                }

                if (xml->name() == "memory")
                {
                    qDebug() << xml->attributes().value("name").toString();
                    qDebug() << xml->attributes().value("start").toString();
                    qDebug() << xml->attributes().value("size").toString();
                    memory_t mem;
                    mem.name = xml->attributes().value("name").toString();
                    mem.start = xml->attributes().value("start").toString();
                    mem.size = xml->attributes().value("size").toString();
                    dev.memories.push_back(mem);
                }

                if (xml->name() == "package")
                {
                    qDebug() << xml->attributes().value("name").toString();
                    qDebug() << xml->attributes().value("type").toString();
                    package_t package;
                    package.name = xml->attributes().value("name").toString();
                    package.type = xml->attributes().value("type").toString();
                    dev.packages.push_back(package);
                }

                if (xml->name() == "ip")
                {
                    peripheral_t peripheral;
                    peripheral.part = xml->attributes().value("part").toString();
                    peripheral.ipName = xml->attributes().value("name").toString();
                    peripheral.instance = xml->attributes().value("instance").toString();
                    dev.ip.push_back(peripheral);
                }

#if 0
                if (xml->name() == "ip")
                {
                    qDebug() << xml->readElementText();
                }
#endif

            }
        }
    }

    dev.packNum = dev.packages.size();
    dev.memNum = dev.memories.size();
    dev.ipNum = dev.ip.size();

    devInfo.push_back(dev);
    devNum = devInfo.size();

    xml_file.close();

    // table widget

    QStringList tableHeader;
    tableHeader << "Manufacturer" << "Device" << "Package" << "Core" << "Full Name";
    deviceTableWidget = new QTableWidget;
    deviceTableWidget->setColumnCount(tableHeader.count());
    deviceTableWidget->setHorizontalHeaderLabels(tableHeader);
    QHeaderView *headView = deviceTableWidget->horizontalHeader();
    headView->setDefaultAlignment(Qt::AlignLeft);// 表头字左对齐
    deviceTableWidget->verticalHeader()->setDefaultSectionSize(10);//设置行高
    deviceTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);// 只能选择一行，不能多行
    deviceTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);// 单击选择一行
    deviceTableWidget->verticalHeader()->setVisible(false);//隐藏行号
    deviceTableWidget->horizontalHeader()->setStretchLastSection(true);//最后一列铺满

    for (quint8 i = 0; i < devNum; i++)
    {
        for (quint8 j = 0; j < devInfo[i].packNum; j++)
        {
            // 添加新的一行
            deviceTableWidget->insertRow(deviceTableWidget->rowCount());
            for (quint8 k = 0; k < tableHeader.count(); k++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                if (k == 0)
                {
                    item->setText(devInfo[i].vendor);
                }

                if (k == 1)
                {
                    item->setText(devInfo[i].tarDevice);
                }

                if (k == 2)
                {
                    item->setText(devInfo[i].packages[j].type);
                }

                if (k == 3)
                {
                    item->setText(devInfo[i].core);
                }

                if (k == 4)
                {
                    item->setText(devInfo[i].packages[j].name);
                }

                deviceTableWidget->setItem(j, k, item);

            }

        }

    }

    QHBoxLayout *pushBtnLayout = new QHBoxLayout;
    QPushButton *btn_ok = new QPushButton;
    btn_ok->setText("OK");
    btn_ok->setFixedWidth(BUTTON_WIDTH);
    QPushButton *btn_cancel = new QPushButton;
    btn_cancel->setText("Cancel");
    btn_cancel->setFixedWidth(BUTTON_WIDTH);
    pushBtnLayout->addStretch(5);
    pushBtnLayout->addWidget(btn_ok, Qt::AlignRight);
    pushBtnLayout->addWidget(btn_cancel, Qt::AlignRight);


    // 布局
    tarDevSettingLayout->addLayout(labelLayout);
    tarDevSettingLayout->addWidget(deviceTableWidget);
    tarDevSettingLayout->addLayout(pushBtnLayout);

    setLayout(tarDevSettingLayout);

    connect(deviceTableWidget,
            SIGNAL(cellClicked(int, int)),
            this,
            SLOT(click_table_widget_select_device(int, int))
            );

    connect(deviceTableWidget,
            SIGNAL(cellDoubleClicked(int, int)),
            this,
            SLOT(double_click_table_widget_select_device(int, int))
            );

    connect(btn_ok,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( click_ok_select_device(bool) )
            );

    connect(btn_cancel,
            SIGNAL( clicked(bool) ),
            this,
            SLOT( click_cancel_select_device(bool) )
            );

}

// 单击事件
void Device::click_table_widget_select_device(int row, int col)
{
//    qDebug() << "row:" << row << ", col:" << col;
    (void)col;
    deviceLabel->setText(deviceTableWidget->item(row, 4)->text());
    r = row;
}

// 双击事件
void Device::double_click_table_widget_select_device(int row, int col)
{
    (void)col;
    deviceLabel->setText(deviceTableWidget->item(row, 4)->text());
    wizard->lineEdit->setText(deviceLabel->text());
    wizard->packageLineEdit->setText(deviceTableWidget->item(row, 2)->text());
    qDebug() << "device: " << deviceLabel->text();
    this->close();
}

void Device::click_ok_select_device(bool)
{
    // 所选device
    qDebug() << "device: " << deviceLabel->text();
    wizard->lineEdit->setText(deviceLabel->text());
    wizard->packageLineEdit->setText(deviceTableWidget->item(r, 2)->text());
    this->close();
}

void Device::click_cancel_select_device(bool)
{
    this->close();
}

Device::~Device()
{
    delete ui;
}
