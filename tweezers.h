#ifndef TWEEZERS_H
#define TWEEZERS_H

#include "tag_converter.h"

#include <QMainWindow>
#include <QFile>

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
    void openDir();
    void namePreview(bool show=true);
    void cleanTable();
    void selectDirectory();
    void renameAll();
    void renameSelection();
    void undoRename();

private:
    TagConverter tag;
    QString curr_path;
    QStringList glob_exp;

    QHash<QString, QString> backup;

    void createActions();
    Ui::Tweezers *ui;
};

#endif // TWEEZERS_H
