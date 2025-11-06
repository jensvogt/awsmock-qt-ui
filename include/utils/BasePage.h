#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QObject>
#include <QWidget>
#include <QTimer>

class BasePage : public QWidget
{
    Q_OBJECT

public:
    BasePage(QWidget *parent);

    void StartAutoUpdate();

    void StopAutoUpdate() const;

    virtual void LoadContent() = 0;

private:
    QTimer *autoUpdateTimer;

};

#endif // BASEPAGE_H
