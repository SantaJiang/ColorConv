#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_hex->setText("#00BFFF");
    QString str = HexToRgb(ui->lineEdit_hex->text());
    ui->lineEdit_rgb->setText(str);

    QFont font;
    font.setFamily("Arial Rounded MT Bold");
#ifdef Q_OS_WIN
    font.setPixelSize(10);
#else
    font.setPixelSize(12);
#endif
    ui->lineEdit_hex->setFont(font);
    ui->lineEdit_rgb->setFont(font);
    ui->label_hex->setFont(font);
    ui->label_rgb->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::RgbToHex(QString rgb)
{
    QStringList list;
    if(rgb.contains(","))
        list = rgb.split(",");
    else if(rgb.contains(" "))
        list = rgb.split(" ");

    if(list.count() < 3)
        return "Error";

    QColor color(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    QString strRed = QString("%1").arg(red & 0xFF, 2, 16, QLatin1Char('0'));
    QString strGreen = QString("%1").arg(green & 0xFF, 2, 16, QLatin1Char('0'));
    QString strBlue = QString("%1").arg(blue & 0xFF, 2, 16, QLatin1Char('0'));

    setLabelColor(color);
    return QString("#%1%2%3").arg(strRed.toUpper()).arg(strGreen.toUpper()).arg(strBlue.toUpper());
}

QString MainWindow::HexToRgb(QString hex)
{
    if(hex.contains("#"))
        hex.remove(0,1);

    if(hex.length() > 7)
        return "Error";

    QColor color(hex.toUInt(nullptr,16));
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    QString strRed = QString("%1").arg(red & 0xFF, 2, 16, QLatin1Char('0'));
    QString strGreen = QString("%1").arg(green & 0xFF, 2, 16, QLatin1Char('0'));
    QString strBlue = QString("%1").arg(blue & 0xFF, 2, 16, QLatin1Char('0'));

    setLabelColor(color);
    return QString("%1, %2, %3").arg(red).arg(green).arg(blue);
}

void MainWindow::setLabelColor(QColor color)
{
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    QString strRed = QString("%1").arg(red & 0xFF, 2, 16, QLatin1Char('0'));
    QString strGreen = QString("%1").arg(green & 0xFF, 2, 16, QLatin1Char('0'));
    QString strBlue = QString("%1").arg(blue & 0xFF, 2, 16, QLatin1Char('0'));

    QString bg = QString("rgb(%1, %2, %3)").arg(red).arg(green).arg(blue);

    ui->label_color->setStyleSheet(QString("border:2px solid rgb(169,169,169); border-radius: 36px; background-color: %1;").arg(bg));
}

void MainWindow::on_lineEdit_hex_returnPressed()
{
    QString str = HexToRgb(ui->lineEdit_hex->text());
    ui->lineEdit_rgb->setText(str);
    //复制到剪贴板
    QClipboard *clip = QApplication::clipboard();
    clip->setText(str);
}


void MainWindow::on_lineEdit_rgb_returnPressed()
{
    QString str = RgbToHex(ui->lineEdit_rgb->text());
    ui->lineEdit_hex->setText(str);
    //复制到剪贴板
    QClipboard *clip = QApplication::clipboard();
    clip->setText(str);
}

