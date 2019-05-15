#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QPalette>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <vector>
#include "layers.h"
#include "data.h"
#define PI 3.14159265

namespace Ui {
class Widget;
}
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    static Layer mylayer[100];
    static QBrush mybrush[10000];
    static double height;
    static double angle;
    static double gap;
    static double edge;
    static int piles;
    static int sum_pass;
    static bool check;
    static QSqlDatabase database;
    QSqlTableModel *models;

private slots:
    void submit();
    void showcontent();
    void clearcontent();
    void input();
    void deleteinput();
    void showpage();
    void autoarrange();

private:
    void set_pen(int width, const QColor & color);
    void draw_steel();
    void draw_para();
    void draw_para_1();
    void paint_first();
    void draw_odd(int l, int p);
    void paint_odd(int l, int p);
    void draw_even(int l, int p);
    void paint_even(int l, int p);
    void paint_first_1();
    void draw_odd_1(int l, int p);
    void paint_odd_1(int l, int p);
    void draw_even_1(int l, int p);
    void paint_even_1(int l, int p);
    QPolygonF paint_steel_l();
    QPolygonF paint_steel_r();
    QFont timesFont;
    Ui::Widget *ui;
    QGraphicsScene * scene_1;
    QPen pen;   
    QPalette pal;
    Data* d;

};



#endif // WIDGET_H
