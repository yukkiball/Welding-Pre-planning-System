#ifndef DATA_H
#define DATA_H

#include <QWidget>

namespace Ui {
class Data;
}

class Data : public QWidget
{
    Q_OBJECT

public:
    explicit Data(QWidget *parent = nullptr);
    ~Data();

private slots:
    void showdata();

private:
    Ui::Data *ui;
};

#endif // DATA_H
