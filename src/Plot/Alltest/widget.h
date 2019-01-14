#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QLineEdit;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLabel* mLineCountLabel;
    QLabel* mPointCountLabel;
    QLineEdit* mLineCountEdit;
    QLineEdit* mPointCountEdit;
    int mLineCount;
    int mPointCount;
};

#endif // WIDGET_H
