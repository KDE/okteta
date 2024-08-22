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
#include <QMimeData>
#include <QThread>
// Std
#include <memory>

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
    /// the result of the thread, if successful
    std::unique_ptr<QMimeData> releaseData();

Q_SIGNALS:
    void generated(const QMimeData* data);

private:
    AbstractModelDataGenerator* const mGenerator;

    std::unique_ptr<QMimeData> mMimeData;
};

inline ModelDataGenerateThread::ModelDataGenerateThread(QObject* parent,
                                                        AbstractModelDataGenerator* generator)
    : QThread(parent)
    , mGenerator(generator)
{}

inline std::unique_ptr<QMimeData> ModelDataGenerateThread::releaseData() { return std::move(mMimeData); }

}

#endif
