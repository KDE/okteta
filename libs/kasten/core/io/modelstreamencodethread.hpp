/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELSTREAMENCODETHREAD_HPP
#define KASTEN_MODELSTREAMENCODETHREAD_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QThread>

class QIODevice;

namespace Kasten {

class AbstractModelStreamEncoder;
class AbstractModel;
class AbstractModelSelection;

// TODO: instead of doubling all data just read them from the job?
class KASTENCORE_EXPORT ModelStreamEncodeThread : public QThread
{
    Q_OBJECT

public:
    ModelStreamEncodeThread(QObject* parent,
                            QIODevice* ioDevice,
                            AbstractModel* model, const AbstractModelSelection* selection,
                            AbstractModelStreamEncoder* encoder);
    ModelStreamEncodeThread() = delete;

    ~ModelStreamEncodeThread() override;

public: // QThread API
    void run() override;

public:
    bool success() const;

Q_SIGNALS:
    void modelExported(bool success);

private:
    QIODevice* mIODevice;
    AbstractModel* mModel;
    const AbstractModelSelection* mSelection;
    AbstractModelStreamEncoder* mEncoder;

    bool mSuccess = false;
};

inline ModelStreamEncodeThread::ModelStreamEncodeThread(QObject* parent,
                                                        QIODevice* ioDevice,
                                                        AbstractModel* model, const AbstractModelSelection* selection,
                                                        AbstractModelStreamEncoder* encoder)
    : QThread(parent)
    , mIODevice(ioDevice)
    , mModel(model)
    , mSelection(selection)
    , mEncoder(encoder)
{}

inline bool ModelStreamEncodeThread::success() const { return mSuccess; }

}

#endif
