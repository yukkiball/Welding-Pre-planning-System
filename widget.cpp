#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <cmath>
#include "data.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QLineSeries>
#include <QtCharts>
#include <vector>
#include <algorithm>
#include <QSharedPointer>


static int i = 0;
double Widget::height = 0;
double Widget::angle = 0;
double Widget::gap = 0;
double Widget::edge = 0;
int Widget::piles = 0;
int Widget::sum_pass = 0;
Layer Widget::mylayer[100];
QBrush Widget::mybrush[10000];
bool Widget::check = true;
QSqlDatabase Widget::database;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    d = new Data;
    timesFont = QFont("Times", 10, QFont::Normal);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for (int k = 0; k < 10000; ++k)
    {
        mybrush[k] = QBrush(QColor::fromHsl(qrand()%360,qrand()%256,qrand()%200), Qt::SolidPattern);
    }
    ui->setupUi(this);
    ui->pileline->setMaximum(100);
    ui->passline->setMaximum(100);
    ui->pushButton->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_2->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_3->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_4->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_5->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_6->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    ui->pushButton_7->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(showcontent()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(clearcontent()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(input()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(deleteinput()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(showpage()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(autoarrange()));
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        //创建数据库
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Welding.db");
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        //创建存储表格
        QString create_sql = "create table welding (id int primary key, layer int, pass int, X double, Z double, P double, "
                             "extension double, current double, speed double, frequency double, range double, time double, "
                             "height double, angle double, gap double, edge double, mode int)";
        QSqlQuery sql_query;
        if(!sql_query.exec(create_sql))
        {
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }

        models = new QSqlTableModel(this, database);
        models->setTable("welding");
        models->setEditStrategy(QSqlTableModel::OnManualSubmit);
        models->select();
        models->setHeaderData(0, Qt::Horizontal, tr("id"));
        models->setHeaderData(1, Qt::Horizontal, tr("层数"));
        models->setHeaderData(2, Qt::Horizontal, tr("道数"));
        models->setHeaderData(3, Qt::Horizontal, tr("X"));
        models->setHeaderData(4, Qt::Horizontal, tr("Z"));
        models->setHeaderData(5, Qt::Horizontal, tr("P"));
        models->setHeaderData(6, Qt::Horizontal, tr("干伸长"));
        models->setHeaderData(7, Qt::Horizontal, tr("焊接电流"));
        models->setHeaderData(8, Qt::Horizontal, tr("焊接速度"));
        models->setHeaderData(9, Qt::Horizontal, tr("摆频"));
        models->setHeaderData(10, Qt::Horizontal, tr("摆幅"));
        models->setHeaderData(11, Qt::Horizontal, tr("停留时间"));
        models->setHeaderData(12, Qt::Horizontal, tr("厚度"));
        models->setHeaderData(13, Qt::Horizontal, tr("夹角"));
        models->setHeaderData(14, Qt::Horizontal, tr("间隙"));
        models->setHeaderData(15, Qt::Horizontal, tr("钝边"));
        models->setHeaderData(16, Qt::Horizontal, tr("模式"));

    }
}

Widget::~Widget()
{
    delete ui;
    delete d;
    delete scene_1;
}

//public function
void Widget::set_pen(int width, const QColor & color)
{
    pen.setWidth(width);
    pen.setColor(color);
}

//钢板
void Widget::draw_steel()
{
    double height_1 = height - edge;
    double height_0 = 120;
    double height_2 = 120 - 10;
    double length_1 = 250;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::blue);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)-length_1, 0, 0, 0, pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)-length_1, 0, height_2/tan(angle_1 * PI/ 180)-length_1, height_0, pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)-length_1, height_0, height_2/tan(angle_1 * PI/ 180), height_0,pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)+gap_1, height_0, height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, height_0, pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, height_0,height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, 0, pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, 0, 2 * height_2/tan(angle_1 * PI/ 180)+gap_1, 0, pen);
    set_pen(3, Qt::red);
    scene_1->addLine(0 ,0 ,height_2/tan(angle_1 * PI/ 180), height_2,pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180), height_2, height_2/tan(angle_1 * PI/ 180), height_0,pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180), height_0, height_2/tan(angle_1 * PI/ 180)+gap_1, height_0,pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)+gap_1, height_0, height_2/tan(angle_1 * PI/ 180)+gap_1, height_2,pen);
    scene_1->addLine(height_2/tan(angle_1 * PI/ 180)+gap_1, height_2, 2 * height_2/tan(angle_1 * PI/ 180)+gap_1, 0,pen);

}

//填充左侧钢板
QPolygonF Widget::paint_steel_l()
{
    QPolygonF pgl;
    double height_1 = height - edge;
    double height_0 = 120;
    double height_2 = 120 - 10;
    double length_1 = 250;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    pgl << QPointF(0,0) << QPointF (height_2/tan(angle_1 * PI/ 180)-length_1, 0) << QPointF(height_2/tan(angle_1 * PI/ 180)-length_1, height_0)
        << QPointF(height_2/tan(angle_1 * PI/ 180), height_0) << QPointF(height_2/tan(angle_1 * PI/ 180), height_2) << QPointF(0,0);
    return pgl;
}

//填充右侧钢板
QPolygonF Widget::paint_steel_r()
{
    QPolygonF pgr;
    double height_1 = height - edge;
    double height_0 = 120;
    double height_2 = 120 - 10;
    double length_1 = 250;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    pgr << QPointF(2 * height_2/tan(angle_1 * PI/ 180)+gap_1, 0) << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, 0) <<  QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1+length_1, height_0)
        << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_0) << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_2) << QPointF(2 * height_2/tan(angle_1 * PI/ 180)+gap_1, 0);
    return pgr;
}
//平行线(等高）
void Widget::draw_para()
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    for (int j = 0; j < piles; j++)
    {
        scene_1->addLine(j * height_2/(piles * tan(angle_1 * PI/ 180)), j * height_2/piles,
                         2 * height_2/tan(angle_1 * PI/ 180) + gap_1 - j * height_2/(piles * tan(angle_1 * PI/ 180)), j * height_2/piles, pen);
    }
}

//平行线(等面积）
void Widget::draw_para_1()
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    for (int j = 0; j < piles; j++)
    {
        scene_1->addLine(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[j].get_pass_l() * per_area) - gap_1),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[j].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180),
                         height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[j].get_pass_l() * per_area) - gap_1),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[j].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180), pen);
    }

}

//奇数道（等高）
void Widget::draw_odd(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    for (int j = 0; j < (p - 1) / 2; j++)
    {
        scene_1->addLine(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles,
                         height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles, pen);
    }
    for (int j = (p + 1) / 2; j < p; j++)
    {
        scene_1->addLine(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles,
                         height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles, pen);
    }
}
//填充第一层（等高）
void Widget::paint_first()
{
    double height_1 = height - edge;
    double height_0 = 120;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    mylayer[0].mypass[0].polygon << QPointF(height_2/tan(angle_1 * PI/ 180), height_2) << QPointF(height_2/tan(angle_1 * PI/ 180), height_0)
                                 << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_0) << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_2)
                                 << QPointF( 2 * height_2/tan(angle_1 * PI/ 180) + gap_1 - (piles - 1) * height_2/(piles * tan(angle_1 * PI/ 180)),height_2 - 1 * height_2/piles)<< QPointF((piles - 1) * height_2/(piles * tan(angle_1 * PI/ 180)),height_2 - 1 * height_2/piles)
                                 << QPointF(height_2/tan(angle_1 * PI/ 180), height_2);
    //mylayer[0].mypass[0].path.addPolygon(mylayer[0].mypass[0].polygon);
    mylayer[0].mypass[0].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180), height_2), timesFont, "1");
}

//奇数道填充（等高）
void Widget::paint_odd(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    //左侧填充
    for (int j = 0; j < (p - 1) / 2; ++j)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 0.2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 1));
    }
    //右侧填充
    for (int j = (p + 1) / 2; j < p; ++j)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 0.3) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() - 2 * j + 2 * p));

    }
    //中间填充
    mylayer[l - 1].mypass[(p - 1) / 2].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 3)/ 2 + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                               << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 3)/ 2 + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                               << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p + 1)/ 2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                               << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p + 1)/ 2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
    //mylayer[l - 1].mypass[(p - 1) / 2].path.addPolygon(mylayer[l - 1].mypass[(p - 1) / 2].polygon);
    mylayer[l - 1].mypass[(p - 1) / 2].path.addText( QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 3)/ 2 + 1 + 0.2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() + p));
}

//偶数道（等高）
void Widget::draw_even(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    for (int j = 0; j < p / 2; j++)
    {
        scene_1->addLine(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles,
                         height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles, pen);
    }
    for (int j = (p + 2) / 2; j < p; j++)
    {
        scene_1->addLine(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles,
                         height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles, pen);
    }

}
//偶数道填充（等高）
void Widget::paint_even(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    set_pen(3, Qt::red);
    //左侧填充
    for (int j = 0; j < p / 2; ++j)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 0.2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 1));
    }
    //右侧填充
    for (int j = (p + 2) / 2; j < p; ++j)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + j * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + (j + 0.3) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() - 2 * j + 2 * p));
    }
    //中间填充
    mylayer[l - 1].mypass[p / 2].polygon << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 2)/ 2 + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 2)/ 2 + 1) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) - height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p + 2)/ 2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p) + height_2/(piles * tan(angle_1 * PI/ 180)), height_2 - l * height_2/piles)
                                         << QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p + 2)/ 2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles);
    //mylayer[l - 1].mypass[p / 2].path.addPolygon(mylayer[l - 1].mypass[p / 2].polygon);
    mylayer[l - 1].mypass[p / 2].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180) - (l - 1) * height_2/(piles * tan(angle_1 * PI/ 180)) + ((p - 2)/ 2 + 1 + 0.2) * ((gap_1 + 2 * (l - 1) *height_2/(piles * tan(angle_1 * PI/ 180)))/p), height_2 - (l - 1) * height_2/piles), timesFont, QString::number(mylayer[l - 1].get_pass_f() + p));
}

//偶数道（等面积）
void Widget::draw_even_1(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    for (int j = 0; j < p / 2; j++)
    {
        scene_1->addLine(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180),
                         height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180), pen);
    }

    for (int j = 0; j < (p - 2) / 2; j++)
    {
        scene_1->addLine(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180),
                         height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180), pen);

    }
}
//填充第一层（等面积）
void Widget::paint_first_1()
{
    double height_1 = height - edge;
    double height_0 = 120;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_1 = 90 - angle/2;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    mylayer[0].mypass[0].polygon << QPointF(height_2/tan(angle_1 * PI/ 180), height_2) << QPointF(height_2/tan(angle_1 * PI/ 180), height_0)
                                 << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_0) << QPointF(height_2/tan(angle_1 * PI/ 180)+gap_1, height_2)
                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[0].get_pass_l() * per_area) - gap_1),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[0].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))<< QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[0].get_pass_l() * per_area) - gap_1),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[0].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                 << QPointF(height_2/tan(angle_1 * PI/ 180), height_2);
    //mylayer[0].mypass[0].path.addPolygon(mylayer[0].mypass[0].polygon);
    mylayer[0].mypass[0].path.addText(QPointF(height_2/tan(angle_1 * PI/ 180), height_2), timesFont, "1");
}
//偶数道填充（等面积）
void Widget::paint_even_1(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    //左侧填充
    for (int j = 0; j < p / 2; j++)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 0.2) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 1));
    }
    //右侧填充
    for (int j = 0; j < (p - 2) / 2; j++)
    {
        mylayer[l - 1].mypass[p - 1 - j].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
        //mylayer[l - 1].mypass[p - 1 - j].path.addPolygon(mylayer[l - 1].mypass[p - 1 - j].polygon);
        mylayer[l - 1].mypass[p - 1 - j].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j  + 0.3) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 2));
    }
    //中间填充
    mylayer[l - 1].mypass[p / 2].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 2) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + ((p - 2) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - ((p - 4) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - ((p - 4) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 2) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
    //mylayer[l - 1].mypass[p / 2].path.addPolygon(mylayer[l - 1].mypass[p / 2].polygon);
    mylayer[l - 1].mypass[p / 2].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 2) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_l()));
}


//奇数道（等面积）
void Widget::draw_odd_1(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    for (int j = 0; j < (p - 1) / 2; j++)
    {
        scene_1->addLine(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180),
                         height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180), pen);
    }

    for (int j = 0; j < (p - 1) / 2; j++)
    {
        scene_1->addLine(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180),
                         height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180), pen);
    }

}

//奇数道填充（等面积）
void Widget::paint_odd_1(int l, int p)
{
    double height_1 = height - edge;
    double height_2 = 120 - 10;
    double gap_1 = 10;
    double angle_2 = angle * 0.5;
    double area = (gap_1 + height_2 * tan(angle_2 * PI/ 180)) * height_2;
    double per_area = area/sum_pass;
    set_pen(3, Qt::red);
    //左侧填充
    for (int j = 0; j < (p - 1) / 2; j++)
    {
        mylayer[l - 1].mypass[j].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                         << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
        //mylayer[l - 1].mypass[j].path.addPolygon(mylayer[l - 1].mypass[j].polygon);
        mylayer[l - 1].mypass[j].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + (j + 0.2) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 1));
    }
    //右侧填充
    for (int j = 0; j < (p - 1) / 2; j++)
    {
        mylayer[l - 1].mypass[p - 1 - j].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                                 << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
        //mylayer[l - 1].mypass[p - 1 - j].path.addPolygon(mylayer[l - 1].mypass[p - 1 - j].polygon);
        mylayer[l - 1].mypass[p - 1 - j].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - (j + 0.3) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_f() + 2 * j + 2));
    }
    //中间填充
    mylayer[l - 1].mypass[(p - 1) / 2].polygon << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) + ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1) - ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)),  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) + gap_1 + 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) - ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180))
                                               << QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180));
    //mylayer[l - 1].mypass[(p - 1) / 2].path.addPolygon(mylayer[l - 1].mypass[(p - 1) / 2].polygon);
    mylayer[l - 1].mypass[(p - 1) / 2].path.addText(QPointF(height_2 * tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1) + ((p - 3) / 2 + 1) * per_area/(0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_l() * per_area) - gap_1)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)) ,  height_2 - 0.5 * (sqrt(gap_1 * gap_1 + 4 * tan(angle_2 * PI/ 180) * mylayer[l - 1].get_pass_f() * per_area) - gap_1)/ tan(angle_2 * PI/ 180)), timesFont, QString::number(mylayer[l - 1].get_pass_l()));
}

//private slots
void Widget::submit()
{
    /*
    QString num_0 = ui->lengthline->text();
    QString num_1 = ui->heightline->text();
    QString num_2 = ui->angleline->text();
    QString num_3 = ui->gapline->text();
    QString num_4 = ui->edgeline->text();
    length = num_0.toDouble();
    height = num_1.toDouble();
    angle = num_2.toDouble();
    gap = num_3.toDouble();
    edge = num_4.toDouble();
    check = ui->radioButton->isChecked();
    */
    piles = i + 1;
    mylayer[i].set_pass_l(sum_pass);
    mylayer[0].set_pass_f(0);
    for (int j = i; j > 0; --j)
    {
        mylayer[j - 1].set_pass_l(mylayer[j].get_pass_l() - mylayer[j].get_pass());
        mylayer[j].set_pass_f(mylayer[j - 1].get_pass_l());


    }
    qDebug() << "高度： " << height << endl
             << "夹角： " << angle << endl
             << "间隙： " << gap << endl
             << "钝边： " << edge <<endl;
}

void Widget::showcontent()
{


    scene_1 = new QGraphicsScene;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    set_pen(3, Qt::red);
    QPen pen_1;
    pen_1.setWidth(2);
    pen_1.setColor(Qt::yellow);
    scene_1 -> addPolygon(paint_steel_l(), pen, QBrush(Qt::gray, Qt::SolidPattern));
    scene_1 -> addPolygon(paint_steel_r(), pen, QBrush(Qt::gray, Qt::SolidPattern));
    draw_steel();    
    //等高划分
    if(check == true)
    {
        draw_para();
        paint_first();
        for (int j = 1; j < piles; j++)
        {
            if (mylayer[j].get_pass() % 2 == 0)
            {
                draw_even(mylayer[j].get_layer(), mylayer[j].get_pass());
                paint_even(mylayer[j].get_layer(), mylayer[j].get_pass());
                for (int k = 0; k < mylayer[j].get_pass(); ++k)
                {

                    scene_1->addPolygon(mylayer[j].mypass[k].polygon, pen, mybrush[qrand()%10000]);
                    scene_1->addPath(mylayer[j].mypass[k].path, pen_1);
                }
            }
            else
            {
                draw_odd(mylayer[j].get_layer(), mylayer[j].get_pass());
                paint_odd(mylayer[j].get_layer(), mylayer[j].get_pass());
                for (int k = 0; k < mylayer[j].get_pass(); ++k)
                {
                    scene_1->addPolygon(mylayer[j].mypass[k].polygon, pen, mybrush[qrand()%10000]);
                    scene_1->addPath(mylayer[j].mypass[k].path, pen_1);
                }
            }
        }
    }
    //等面积划分
    else
    {
        draw_para_1();
        paint_first_1();
        for (int j = 1; j < piles; j++)
        {
            if (mylayer[j].get_pass() % 2 == 0)
            {
                draw_even_1(mylayer[j].get_layer(), mylayer[j].get_pass());
                paint_even_1(mylayer[j].get_layer(), mylayer[j].get_pass());
                for (int k = 0; k < mylayer[j].get_pass(); ++k)
                {
                    scene_1->addPolygon(mylayer[j].mypass[k].polygon, pen, mybrush[qrand()%10000]);
                    scene_1->addPath(mylayer[j].mypass[k].path, pen_1);
                }
            }
            else
            {
                draw_odd_1(mylayer[j].get_layer(), mylayer[j].get_pass());
                paint_odd_1(mylayer[j].get_layer(), mylayer[j].get_pass());
                for (int k = 0; k < mylayer[j].get_pass(); ++k)
                {
                    scene_1->addPolygon(mylayer[j].mypass[k].polygon, pen, mybrush[qrand()%10000]);
                    scene_1->addPath(mylayer[j].mypass[k].path, pen_1);
                }
            }
        }
    }

    //加入第一层填充
    scene_1->addPolygon(mylayer[0].mypass[0].polygon, pen, mybrush[qrand()%10000]);
    scene_1->addPath(mylayer[0].mypass[0].path, pen_1);
    //设置背景色
    scene_1->setBackgroundBrush(QBrush(QColor::fromHsl(218, 112, 214)));
    //显示图形
    ui->graphicsView->setScene(scene_1);
    ui->graphicsView->show();
}

void Widget::clearcontent()
{
    QGraphicsScene * new_scene = new QGraphicsScene;
    ui->graphicsView->setScene(new_scene);
    ui->graphicsView->show();
    for (int j = 0; j < i + 1; ++j)
    {
        mylayer[j].mypass.clear();
        mylayer[j].set_pass(0);
        mylayer[j].set_pass_f(0);
        mylayer[j].set_pass_l(0);
        mylayer[j].set_layer(0);
        mylayer[j].set_mode(1);
    }
    i = 0;
    sum_pass = 0;
    ui->heightline->setText(tr("0"));
    ui->angleline->setText(tr("0"));
    ui->gapline->setText(tr("0"));
    ui->edgeline->setText(tr("0"));
    ui->label_num->setNum(0);
    ui->label_num_2->setNum(0);
    ui->pileline->setValue(0);
    ui->passline->setValue(0);
    ui->radioButton->setChecked(true);

}

void Widget::input()
{
        //获取坡口参数

        QString num_1 = ui->heightline->text();
        QString num_2 = ui->angleline->text();
        QString num_3 = ui->gapline->text();
        QString num_4 = ui->edgeline->text();
        height = num_1.toDouble();
        angle = num_2.toDouble();
        gap = num_3.toDouble();
        edge = num_4.toDouble();
        check = ui->radioButton->isChecked();
        i = ui->pileline->text().toInt() - 1;
        mylayer[i].b1 = ui->pileline;
        mylayer[i].b2 = ui->passline;
        mylayer[i].l1 = ui->LineEdit_1;
        mylayer[i].l2 = ui->LineEdit_2;
        mylayer[i].l3 = ui->LineEdit_3;
        mylayer[i].l4 = ui->LineEdit_4;
        mylayer[i].l5 = ui->LineEdit_5;
        mylayer[i].l6 = ui->LineEdit_6;
        mylayer[i].l7 = ui->LineEdit_7;
        mylayer[i].l8 = ui->LineEdit_8;
        mylayer[i].l9 = ui->LineEdit_9;
        mylayer[i].get_info();
        ++sum_pass;
        int rowCount = models->rowCount();
        models->insertRow(rowCount);
        models->setData(models->index(rowCount,0), rowCount);
        models->setData(models->index(rowCount,1), mylayer[i].get_layer());
        models->setData(models->index(rowCount,2), mylayer[i].get_current_pass());
        models->setData(models->index(rowCount,3), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_X());
        models->setData(models->index(rowCount,4), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_Z());
        models->setData(models->index(rowCount,5), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_P());
        models->setData(models->index(rowCount,6), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_extension());
        models->setData(models->index(rowCount,7), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_current());
        models->setData(models->index(rowCount,8), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_speed());
        models->setData(models->index(rowCount,9), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_frequency());
        models->setData(models->index(rowCount,10), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_range());
        models->setData(models->index(rowCount,11), mylayer[i].mypass[mylayer[i].get_current_pass() - 1].get_time());
        models->setData(models->index(rowCount,12), height);
        models->setData(models->index(rowCount,13), angle);
        models->setData(models->index(rowCount,14), gap);
        models->setData(models->index(rowCount,15), edge);
        models->setData(models->index(rowCount,16), mylayer[i].get_mode());
        //提交至数据库
        models->submitAll();
        ui->label_num->setNum(i + 1);
        ui->label_num_2->setNum(sum_pass);
}

void Widget::deleteinput()
{
    if (i > 0)
    {
        --sum_pass;
        if (mylayer[i].get_pass() == 0)
            --i;
        mylayer[i].mypass.erase(mylayer[i].mypass.end() - 1);
        mylayer[i].set_pass(mylayer[i].mypass.size());
        int rowCount = models->rowCount();
        models->removeRow(rowCount - 1);
        models->submitAll();
        ui->label_num->setNum(i + 1);
        ui->label_num_2->setNum(sum_pass);
    }

}

void Widget::showpage()
{
    //设置位姿表格
    double height_1 = Widget::height - Widget::edge;
    double angle_1 = 90 - Widget::angle/2;
    double angle_2 = Widget::angle * 0.5;
    double area = (Widget::gap + height_1 * tan(angle_2 * PI/ 180)) * height_1;
    double per_area = area/Widget::sum_pass;
    for (int j = 0; j < Widget::piles; j++)
    qDebug() << "起始道数:" << Widget::mylayer[j].get_pass_f() << endl;
    QStandardItemModel *model;

    model = new QStandardItemModel(Widget::sum_pass, 11);
    ui->tableView->setModel(model);

    //设置列宽
    for (int j = 0; j < 2; j++)
        ui->tableView->setColumnWidth(j, 90);
    for (int j = 2; j < 11; j++)
        ui->tableView->setColumnWidth(j, 93);
    ui->tableView->verticalHeader()->setVisible(false);
    //设置标题
    /*
    model->setHeaderData(0, Qt::Horizontal, tr("焊道"));
    model->setHeaderData(1, Qt::Horizontal, tr("X"));
    model->setHeaderData(2, Qt::Horizontal, tr("Y"));
    model->setHeaderData(3, Qt::Horizontal, tr("Z"));
    model->setHeaderData(4, Qt::Horizontal, tr("W"));
    model->setHeaderData(5, Qt::Horizontal, tr("P"));
    model->setHeaderData(6, Qt::Horizontal, tr("R"));
    model->setHeaderData(7, Qt::Horizontal, tr("焊接电流(A)"));
    model->setHeaderData(8, Qt::Horizontal, tr("焊接电压(V)"));
    model->setHeaderData(9, Qt::Horizontal, tr("焊接速度(mm/s)"));
    */
    model->setHeaderData(0, Qt::Horizontal, tr("层数"));
    model->setHeaderData(1, Qt::Horizontal, tr("道数"));
    model->setHeaderData(2, Qt::Horizontal, tr("X"));
    model->setHeaderData(3, Qt::Horizontal, tr("Z"));
    model->setHeaderData(4, Qt::Horizontal, tr("P"));
    model->setHeaderData(5, Qt::Horizontal, tr("干伸长"));
    model->setHeaderData(6, Qt::Horizontal, tr("焊接电流"));
    model->setHeaderData(7, Qt::Horizontal, tr("焊接速度"));
    model->setHeaderData(8, Qt::Horizontal, tr("摆频"));
    model->setHeaderData(9, Qt::Horizontal, tr("摆幅"));
    model->setHeaderData(10, Qt::Horizontal, tr("停留时间"));

    //设置层数
    for (int j = 0; j < Widget::piles; j++)
    {
        for (int k = Widget::mylayer[j].get_pass_f(); k <  Widget::mylayer[j].get_pass_l(); k++)
        {
            model->setItem(k, 0, new QStandardItem(QString::number(Widget::mylayer[j].get_layer())));
            model->setItem(k, 1, new QStandardItem(QString::number(k - Widget::mylayer[j].get_pass_f() + 1)));
            model->setItem(k, 2, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_X())));
            model->setItem(k, 3, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_Z())));
            model->setItem(k, 4, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_P())));
            model->setItem(k, 5, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_extension())));
            model->setItem(k, 6, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_current())));
            model->setItem(k, 7, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_speed())));
            model->setItem(k, 8, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_frequency())));
            model->setItem(k, 9, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_range())));
            model->setItem(k, 10, new QStandardItem(QString::number(Widget::mylayer[j].mypass[k - Widget::mylayer[j].get_pass_f()].get_time())));



        }
    }
    /*
    //设置Y、W、R为 0
    for (int j = 0; j < Widget::sum_pass; j++)
    {
       model->setItem(j + 1, 0 , new QStandardItem(QString::number(j + 1)));
       model->setItem(j + 1, 2 , new QStandardItem(QString::number(0)));
       model->setItem(j + 1, 4 , new QStandardItem(QString::number(0)));
       model->setItem(j + 1, 6 , new QStandardItem(QString::number(0)));

    }
    //等高规划
    if (Widget::check == true)
    {
        //设置Z、焊接参数（等高）
        for (int j = 0; j < Widget::piles; j++)
        {
            for (int k = Widget::mylayer[j].get_pass_f(); k <  Widget::mylayer[j].get_pass_l(); k++)
                {
                    model->setItem(k + 1, 3, new QStandardItem(QString::number(0.5 * Widget::edge + j * height_1/Widget::piles)));

                    model->setItem(k + 1, 7, new QStandardItem(QString::number(Widget::mylayer[j].get_current())));
                    model->setItem(k + 1, 8, new QStandardItem(QString::number(Widget::mylayer[j].get_voltage())));
                    model->setItem(k + 1, 9, new QStandardItem(QString::number(Widget::mylayer[j].get_speed())));

                }
        }

        for (int j = 0; j < Widget::piles; j++)
        {
            //设置偶数道数层（等高）
            if (Widget::mylayer[j].get_pass() % 2 == 0)
            {
                //设置左侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f(); k < Widget::mylayer[j].get_pass_f() + Widget::mylayer[j].get_pass() / 2; k++)
                {
                    model->setItem(k + 1, 1 , new QStandardItem(QString::number(height_1/tan(angle_1 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2)
                                                                                - (Widget::mylayer[j].get_layer() - 1) * height_1/(Widget::piles * tan(angle_1 * PI/ 180))
                                                                                + double((k - Widget::mylayer[j].get_pass_f() + 0.25)) * ((Widget::gap + 2 * (Widget::mylayer[j].get_layer() - 1) *height_1/(Widget::piles * tan(angle_1 * PI/ 180)))/Widget::mylayer[j].get_pass()))));
                    model->setItem(k + 1, 5 , new QStandardItem(QString::number(45 + Widget::angle/4)));
                }
                //设置梯形
                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 2) / 2, 1,new QStandardItem(QString::number(0.5 * ((Widget::gap + 2 * (Widget::mylayer[j].get_layer() - 1) * height_1/(Widget::piles * tan(angle_1 * PI/ 180)))/Widget::mylayer[j].get_pass()))));

                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 2) / 2, 5, new QStandardItem(QString::number(90)));
                //设置右侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 2) / 2; k < Widget::mylayer[j].get_pass_l() ; k++)
                {
                    model->setItem(k + 1, 1, new QStandardItem(QString::number(height_1/tan(angle_1 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2)
                                                                               - (Widget::mylayer[j].get_layer() - 1) * height_1/(Widget::piles * tan(angle_1 * PI/ 180))
                                                                               + double((k - Widget::mylayer[j].get_pass_f() + 0.75)) * ((Widget::gap + 2 * (Widget::mylayer[j].get_layer() - 1) *height_1/(Widget::piles * tan(angle_1 * PI/ 180)))/Widget::mylayer[j].get_pass()))));
                    model->setItem(k + 1, 5, new QStandardItem(QString::number(135 - Widget::angle/4)));
                }
            }
            //设置奇数道数层（等高）
            else
            {
                //左侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f(); k < Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() - 1) / 2; k++)
                {
                    model->setItem(k + 1, 1 , new QStandardItem(QString::number(height_1/tan(angle_1 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2)
                                                                                - (Widget::mylayer[j].get_layer() - 1) * height_1/(Widget::piles * tan(angle_1 * PI/ 180))
                                                                                + double((k - Widget::mylayer[j].get_pass_f() + 0.25)) * ((Widget::gap + 2 * (Widget::mylayer[j].get_layer() - 1) *height_1/(Widget::piles * tan(angle_1 * PI/ 180)))/Widget::mylayer[j].get_pass()))));
                    model->setItem(k + 1, 5 , new QStandardItem(QString::number(45 + Widget::angle/4)));
                }
                //中间梯形
                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 1) / 2, 1,new QStandardItem(QString::number(0)));

                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 1) / 2, 5, new QStandardItem(QString::number(90)));
                //右侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 1) / 2; k < Widget::mylayer[j].get_pass_l() ; k++)
                {
                    model->setItem(k + 1, 1, new QStandardItem(QString::number(height_1/tan(angle_1 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2)
                                                                               - (Widget::mylayer[j].get_layer() - 1) * height_1/(Widget::piles * tan(angle_1 * PI/ 180))
                                                                               + double((k - Widget::mylayer[j].get_pass_f() + 0.75)) * ((Widget::gap + 2 * (Widget::mylayer[j].get_layer() - 1) *height_1/(Widget::piles * tan(angle_1 * PI/ 180)))/Widget::mylayer[j].get_pass()))));
                    model->setItem(k + 1, 5, new QStandardItem(QString::number(135 - Widget::angle/4)));
                }

            }

        }

    }

    //等面积规划
    else
    {
        //设置Z、焊接参数（等面积）
        for (int j = 0; j < Widget::piles; j++)
        {
            for (int k = Widget::mylayer[j].get_pass_f(); k <  Widget::mylayer[j].get_pass_l(); k++)
                {
                    model->setItem(k + 1, 3, new QStandardItem(QString::number(0.5 * Widget::edge + 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180))));

                    model->setItem(k + 1, 7, new QStandardItem(QString::number(Widget::mylayer[j].get_current())));
                    model->setItem(k + 1, 8, new QStandardItem(QString::number(Widget::mylayer[j].get_voltage())));
                    model->setItem(k + 1, 9, new QStandardItem(QString::number(Widget::mylayer[j].get_speed())));

                }
        }
        //设置X、P（等面积规划）
        for (int j = 0; j < Widget::piles; j++)
        {
            //设置偶数道层（等面积）
            if (Widget::mylayer[j].get_pass() % 2 == 0)
            {
                //左侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f(); k < Widget::mylayer[j].get_pass_f() + Widget::mylayer[j].get_pass()/ 2; k++)
                {
                    model->setItem(k + 1, 1, new QStandardItem(QString::number(height_1 * tan(angle_2 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap) + double((k - Widget::mylayer[j].get_pass_f() + 0.25)) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180)))));
                    model->setItem(k + 1, 5 , new QStandardItem(QString::number(45 + Widget::angle/4)));
                }
                //中间梯形
                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 2) / 2, 1, new QStandardItem(QString::number(height_1 * tan(angle_2 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap) + double(0.5 * Widget::mylayer[j].get_pass()) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180))
                                                                                                                                               + 0.5 * (Widget::gap + (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)
                                                                                                                                               - double(0.5 * (Widget::mylayer[j].get_pass() + 2)) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180))))));

                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 2) / 2, 5, new QStandardItem(QString::number(90)));
                //右侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_l(); k > Widget::mylayer[j].get_pass_l() - (Widget::mylayer[j].get_pass() - 2) / 2; k--)
                {
                    model->setItem(k, 1, new QStandardItem(QString::number(height_1 * tan(angle_2 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2) + Widget::gap + 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap) - double((Widget::mylayer[j].get_pass_l() - k + 0.25)) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180)))));
                    model->setItem(k, 5, new QStandardItem(QString::number(135 - Widget::angle/4)));
                }
            }

            //设置奇数道层（等面积）
            else
            {
                //左侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_f(); k < Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() - 1) / 2; k++)
                {
                    model->setItem(k + 1, 1, new QStandardItem(QString::number(height_1 * tan(angle_2 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap) + double((k - Widget::mylayer[j].get_pass_f() + 0.25)) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180)))));
                    model->setItem(k + 1, 5 , new QStandardItem(QString::number(45 + Widget::angle/4)));
                }
                //中间梯形
                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 1) / 2, 1,new QStandardItem(QString::number(0)));

                model->setItem(Widget::mylayer[j].get_pass_f() + (Widget::mylayer[j].get_pass() + 1) / 2, 5, new QStandardItem(QString::number(90)));
                //右侧平行四边形
                for (int k = Widget::mylayer[j].get_pass_l(); k > Widget::mylayer[j].get_pass_l() - (Widget::mylayer[j].get_pass() - 1) / 2; k--)
                {
                    model->setItem(k , 1, new QStandardItem(QString::number(height_1 * tan(angle_2 * PI/ 180) - (height_1/tan(angle_1 * PI/ 180) + Widget::gap/2) + Widget::gap + 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap) - double((Widget::mylayer[j].get_pass_l() - k + 0.25)) * per_area/(0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_l() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180) - 0.5 * (sqrt(Widget::gap * Widget::gap + 4 * tan(angle_2 * PI/ 180) * Widget::mylayer[j].get_pass_f() * per_area) - Widget::gap)/ tan(angle_2 * PI/ 180)))));
                    model->setItem(k , 5, new QStandardItem(QString::number(135 - Widget::angle/4)));
                }
            }
        }
    }
    */
    //设置参数表格
//    QStandardItemModel *model_2;
//    model_2 = new QStandardItemModel(4, 2);
//    ui->tableView_2->setModel(model_2);

//    //设置列宽
//    ui->tableView_2->setColumnWidth(0, 129);
//    ui->tableView_2->setColumnWidth(1, 129);

//    //设置行高
//    for (int j = 0; j < 5; j++)
//        ui->tableView_2->setRowHeight(j, 74);

//    //设置标题
//    ui->tableView_2->verticalHeader()->setVisible(false);
//    ui->tableView_2->horizontalHeader()->setVisible(false);
//    model_2->setItem(0, 0, new QStandardItem("厚度(mm)"));
//    model_2->setItem(1, 0, new QStandardItem("角度(°)"));
//    model_2->setItem(2, 0, new QStandardItem("间隙(mm)"));
//    model_2->setItem(3, 0, new QStandardItem("钝边(mm)"));


//    //设置数据
//    model_2->setItem(0, 1, new QStandardItem(QString::number(Widget::height)));
//    model_2->setItem(1, 1, new QStandardItem(QString::number(Widget::angle)));
//    model_2->setItem(2, 1, new QStandardItem(QString::number(Widget::gap)));
//    model_2->setItem(3, 1, new QStandardItem(QString::number(Widget::edge)));
}

void Widget::autoarrange()
{
    QString num_1 = ui->heightline->text();
    QString num_2 = ui->angleline->text();
    QString num_3 = ui->gapline->text();
    QString num_4 = ui->edgeline->text();
    height = num_1.toDouble();
    angle = num_2.toDouble();
    gap = num_3.toDouble();
    edge = num_4.toDouble();
    check = ui->radioButton->isChecked();
    int mode_1 = 1;
    if (check == true)
    {
        mode_1 = 1;
    }
    else
    {
        mode_1 = 2;
    }
    //从数据库中获取数据
    QSqlQuery sql_query;
    QString get_data = QString("SELECT layer, pass, X, Z, P, extension, current, speed, frequency, range, time FROM welding "
                               "WHERE height = %1 AND angle = %2 AND gap = %3 AND edge = %4 AND mode = %5")
            .arg(height).arg(angle).arg(gap).arg(edge).arg(mode_1);
    sql_query.exec(get_data);
    int sum_1 = 0;
    while (sql_query.next())
    {
        i = sql_query.value(0).toInt() - 1;
        mylayer[i].mypass.push_back(Pass(sql_query.value(6).toDouble(), sql_query.value(7).toDouble(), sql_query.value(8).toDouble(), sql_query.value(9).toDouble(),
                                         sql_query.value(10).toDouble(), sql_query.value(2).toDouble(), sql_query.value(3).toDouble(), sql_query.value(4).toDouble(), sql_query.value(5).toDouble()));
        mylayer[i].set_layer(sql_query.value(0).toInt());
        ++sum_1;
    }
    sum_pass = sum_1;
    piles = i + 1;
    mylayer[i].set_pass_l(sum_pass);
    mylayer[0].set_pass_f(0);
    for (int k = 0; k < piles; ++k)
        mylayer[k].set_pass(mylayer[k].mypass.size());
    for (int j = i; j > 0; --j)
    {
        mylayer[j - 1].set_pass_l(mylayer[j].get_pass_l() - mylayer[j].get_pass());
        mylayer[j].set_pass_f(mylayer[j - 1].get_pass_l());
    }


}
