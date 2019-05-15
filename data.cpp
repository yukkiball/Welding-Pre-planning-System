#include "data.h"
#include "widget.h"
#include "ui_data.h"
#include <QDebug>
#include <QStandardItemModel>
#include <cmath>
Data::Data(QWidget *parent) :
    QWidget (parent),
    ui(new Ui::Data)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(showdata()));
    ui->pushButton->setStyleSheet("QPushButton{background-color:rgb(112,128,144);\
                                    color: white;   border-radius: 10px;  border: 2px groove grey; border-style: outset;}" // 按键本色
                                    "QPushButton:hover{background-color:white; color: black;}"
                                    "QPushButton:pressed{background-color:rgb(30, 144, 255); border-style: inset; }");

}

Data::~Data()
{
    delete ui;
}




void Data::showdata()
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
    for (int j = 0; j < 11; j++)
        ui->tableView->setColumnWidth(j, 95);
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
    QStandardItemModel *model_2;
    model_2 = new QStandardItemModel(4, 2);
    ui->tableView_2->setModel(model_2);

    //设置列宽
    ui->tableView_2->setColumnWidth(0, 129);
    ui->tableView_2->setColumnWidth(1, 129);

    //设置行高
    for (int j = 0; j < 5; j++)
        ui->tableView_2->setRowHeight(j, 74);

    //设置标题
    ui->tableView_2->verticalHeader()->setVisible(false);
    ui->tableView_2->horizontalHeader()->setVisible(false);
    model_2->setItem(0, 0, new QStandardItem("厚度(mm)"));
    model_2->setItem(1, 0, new QStandardItem("角度(°)"));
    model_2->setItem(2, 0, new QStandardItem("间隙(mm)"));
    model_2->setItem(3, 0, new QStandardItem("钝边(mm)"));


    //设置数据
    model_2->setItem(0, 1, new QStandardItem(QString::number(Widget::height)));
    model_2->setItem(1, 1, new QStandardItem(QString::number(Widget::angle)));
    model_2->setItem(2, 1, new QStandardItem(QString::number(Widget::gap)));
    model_2->setItem(3, 1, new QStandardItem(QString::number(Widget::edge)));


}
