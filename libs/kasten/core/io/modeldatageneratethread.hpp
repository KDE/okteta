/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELDATAGENERATETHREAD_HPP
#define KASTEN_MODELDATAGENERATETHREAD_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QThread>

class QMimeData;

namespace Kasten {

class AbstractModelDataGenerator;

// TODO: instead of doubling all data just read them from the job?
class KASTENCORE_EXPORT ModelDataGenerateThread : public QThread
{
    Q_OBJECT

public:
    ModelDataGenerateThread(QObject* parent,
                            AbstractModelDataGenerator* generator);

    ~ModelDataGenerateThread() override;

public: // QThread API
    void run() override;

public:
    /// the result of the thread, if successful. Needs to be deleted by the thread::run() caller
    QMimeData* data() const;

Q_SIGNALS:
    void generated(QMimeData* data);

private:
    AbstractModelDataGenerator* mGenerator;

    QMimeData* mMimeData = nullptr;
};

inline ModelDataGenerateThread::ModelDataGenerateThread(QObject* parent,
                                                        AbstractModelDataGenerator* generator)
    : QThread(parent)
    , mGenerator(generator)
{}

inline QMimeData* ModelDataGenerateThread::data() const { return mMimeData; }

}

#endif
