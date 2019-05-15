#ifndef LAYERS_H
#define LAYERS_H
#include <QSpinBox>
#include <QLineEdit>
#include <vector>
class Pass
{
private:
    double current;
    double speed;
    double frequency;
    double range;
    double time;
    double X;
    double Z;
    double P;
    double extension;


public:
    Pass(double c, double s, double f, double r, double t, double x, double z, double p, double e):
        current(c), speed(s), frequency(f), range(r), time(t), X(x),
        Z(z), P(p), extension(e){}
    double get_current(){return current;}
    double get_frequency(){return frequency;}
    double get_speed(){return speed;}
    double get_range(){return  range;}
    double get_extension(){return extension;}
    double get_time(){return time;}
    double get_X(){return X;}
    double get_Z(){return Z;}
    double get_P(){return P;}
    QPolygonF polygon;
    QPainterPath path;

};

class Layer
{
private:
    int current_pass;
    int layer;
    int pass;
    int pass_f;
    int pass_l;
    int mode;
public:
    std::vector<Pass>mypass;
    void get_info();
    int get_current_pass(){return current_pass;}
    int get_layer(){return layer;}
    int get_pass(){return pass;}
    int get_pass_f(){return pass_f;}
    int get_pass_l(){return pass_l;}
    int get_mode(){return mode;}
    void set_pass(int p){pass = p;}
    void set_pass_f(int pf){pass_f = pf;}
    void set_pass_l(int pl){pass_l = pl;}
    void set_layer(int l){layer = l;}
    void set_mode(int m){mode = m;}
    QSpinBox * b1;
    QSpinBox * b2;
    QLineEdit * l1;
    QLineEdit * l2;
    QLineEdit * l3;
    QLineEdit * l4;
    QLineEdit * l5;
    QLineEdit * l6;
    QLineEdit * l7;
    QLineEdit * l8;
    QLineEdit * l9;
    QLineEdit * l10;

};

#endif // LAYERS_H
