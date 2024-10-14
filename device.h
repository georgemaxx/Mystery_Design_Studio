/*******************************************************
 *
 * @file        device.h
 *
 * @author      george.ma<tracymxx@163.com>
 * @date        2024/09/11
 *
 ******************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLine>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTreeWidget>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QVector>
#include <QHeaderView>
#include "common.h"


typedef struct {
    QString name;
    QString start;
    QString size;
} memory_t;

typedef struct {
    QString name;
    QString type;
} package_t;

typedef struct {
    QString part;
    QString ipName;
    QString instance;
} peripheral_t;

typedef struct {
    QString vendor;
    QString tarDevice;
    QString core;
    quint32 memNum;
    QVector<memory_t> memories;
    quint32 packNum;
    QVector<package_t> packages;
    quint32 ipNum;
    QVector<peripheral_t> ip;
} device_t;



namespace Ui {
class Device;
}

class Device : public QWidget
{
    Q_OBJECT

public:
    quint32 devNum;
    QVector<device_t> devInfo;
    QTableWidget *deviceTableWidget;
    QLabel *deviceLabel;
    explicit Device(QWidget *parent = nullptr);
    ~Device();

private slots:
    void click_table_widget_select_device(int, int);
    void double_click_table_widget_select_device(int, int);
    void click_ok_select_device(bool);
    void click_cancel_select_device(bool);

private:
    Ui::Device *ui;
};

#endif // DEVICE_H
