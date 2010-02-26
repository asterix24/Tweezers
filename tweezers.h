#ifndef TWEEZERS_H
#define TWEEZERS_H

#include "tag_converter.h"

#include <QMainWindow>

namespace Ui {
    class Tweezers;
}

class Tweezers : public QMainWindow {
    Q_OBJECT
public:
    Tweezers(QWidget *parent = 0);
    ~Tweezers();

protected:
    void changeEvent(QEvent *e);

private slots:
    void open();
    void preview();
    void cleanTable();
    void selectDirectory();

private:
    TagConverter tag;
    QString select_dir;
    QStringList glob_exp;
    QStringList file_list;

    void createActions();
    Ui::Tweezers *ui;
};

#endif // TWEEZERS_H
