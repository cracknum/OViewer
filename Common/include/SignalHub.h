#ifndef SIGNAL_HUB_H
#define SIGNAL_HUB_H
#include <QObject>

class SignalHub final : public QObject
{
public:
  explicit SignalHub(QObject* parent = nullptr)
    :QObject(parent)
  {}
  signals:

};

#endif //SIGNAL_HUB_H
