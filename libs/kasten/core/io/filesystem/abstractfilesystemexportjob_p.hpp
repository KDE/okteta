/*
    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMEXPORTJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMEXPORTJOB_P_HPP

// lib
#include "abstractfilesystemexportjob.hpp"
#include <abstractexportjob_p.hpp>
// Qt
#include <QFile>
#include <QUrl>
// Std
#include <memory>

namespace Kasten {

class AbstractFileSystemExportJobPrivate : public AbstractExportJobPrivate
{
public:
    AbstractFileSystemExportJobPrivate(AbstractFileSystemExportJob* parent,
                                       AbstractModel* model, const AbstractModelSelection* selection, const QUrl& url);

    ~AbstractFileSystemExportJobPrivate() override;

public: // KJob API
    virtual void start();

public:
    void completeExport(bool success);

public:
    [[nodiscard]]
    AbstractModel* model() const;
    [[nodiscard]]
    const AbstractModelSelection* selection() const;
    [[nodiscard]]
    QFile* file() const;

public: // slot
    void exportToFile();

protected:
    AbstractModel* const mModel;
    const AbstractModelSelection* const mSelection;
    const QUrl mUrl;
    std::unique_ptr<QFile> mFile;
    QString mWorkFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemExportJob)
};

inline AbstractFileSystemExportJobPrivate::AbstractFileSystemExportJobPrivate(AbstractFileSystemExportJob* parent,
                                                                              AbstractModel* model, const AbstractModelSelection* selection,
                                                                              const QUrl& url)
    : AbstractExportJobPrivate(parent)
    , mModel(model)
    , mSelection(selection)
    , mUrl(url)
{}

inline AbstractFileSystemExportJobPrivate::~AbstractFileSystemExportJobPrivate() = default;

inline AbstractModel* AbstractFileSystemExportJobPrivate::model()                    const { return mModel; }
inline const AbstractModelSelection* AbstractFileSystemExportJobPrivate::selection() const { return mSelection; }
inline QFile* AbstractFileSystemExportJobPrivate::file()                             const { return mFile.get(); }

inline void AbstractFileSystemExportJobPrivate::start()
{
    Q_Q(AbstractFileSystemExportJob);

    QMetaObject::invokeMethod(q, "exportToFile", Qt::QueuedConnection);
}

}

#endif
