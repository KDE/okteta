/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTJOB_P_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTJOB_P_HPP

// lib
#include "abstractfilesystemexportjob_p.hpp"
#include "modelencoderfilesystemexportjob.hpp"

namespace Kasten {

class ModelEncoderFileSystemExportJobPrivate : public AbstractFileSystemExportJobPrivate
{
public:
    ModelEncoderFileSystemExportJobPrivate(ModelEncoderFileSystemExportJob* parent,
                                           AbstractModel* model,
                                           const AbstractModelSelection* selection,
                                           const QUrl& url,
                                           AbstractModelStreamEncoder* encoder);

    ~ModelEncoderFileSystemExportJobPrivate() override;

public: // AbstractFileSystemExportJob API
    void startExportToFile();

public:
    AbstractModelStreamEncoder* encoder() const;

private:
    Q_DECLARE_PUBLIC(ModelEncoderFileSystemExportJob)

private:
    AbstractModelStreamEncoder* const mEncoder;
};

inline ModelEncoderFileSystemExportJobPrivate::ModelEncoderFileSystemExportJobPrivate(ModelEncoderFileSystemExportJob* parent,
                                                                                      AbstractModel* model,
                                                                                      const AbstractModelSelection* selection,
                                                                                      const QUrl& url,
                                                                                      AbstractModelStreamEncoder* encoder)
    : AbstractFileSystemExportJobPrivate(parent, model, selection, url)
    , mEncoder(encoder)
{}

inline ModelEncoderFileSystemExportJobPrivate::~ModelEncoderFileSystemExportJobPrivate() = default;

inline AbstractModelStreamEncoder* ModelEncoderFileSystemExportJobPrivate::encoder() const { return mEncoder; }

}

#endif
