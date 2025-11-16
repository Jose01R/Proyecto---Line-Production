#ifndef GENERALCLEANTHREADS_H
#define GENERALCLEANTHREADS_H

#include "generalthread.h"
#include "productioncontroller.h"
#include <QTimer>
#include <QCoreApplication>

class GeneralCleanThreads : public GeneralThread {
    Q_OBJECT

private:
    const ProductionController* controller;

public:
    // Constructor corregido con referencia al controller
    GeneralCleanThreads(const ProductionController* controller,
                        QObject* parent = nullptr)
        : GeneralThread("GeneralCleanThreads", parent),
        controller(controller) {}

protected:
    void run() override;

signals:
    void requestSystemReset();
};

#endif // GENERALCLEANTHREADS_H

