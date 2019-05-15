#include "layers.h"
#include "widget.h"

void Layer::get_info()
{
    QString num_layer = b1->text();
    QString num_pass = b2->text();
    double num_1 = l1->text().toDouble();
    double num_2 = l2->text().toDouble();
    double num_3 = l3->text().toDouble();
    double num_4 = l4->text().toDouble();
    double num_5 = l5->text().toDouble();
    double num_6 = l6->text().toDouble();
    double num_7 = l7->text().toDouble();
    double num_8 = l8->text().toDouble();
    double num_9 = l9->text().toDouble();
    mypass.push_back(Pass(num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9));
    layer = num_layer.toInt();
    pass = mypass.size();
    current_pass = num_pass.toInt();
    if (Widget::check == true)
    {
        mode = 1;
    }
    else
    {
        mode = 2;
    }
    /*
    current = num_cur.toInt();
    voltage = num_vol.toInt();
    speed = num_spe.toInt();
    */
}
