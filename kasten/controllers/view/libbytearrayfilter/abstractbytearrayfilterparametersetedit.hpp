/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSETEDIT_HPP

// Qt
#include <QWidget>

class AbstractByteArrayFilterParameterSet;

class AbstractByteArrayFilterParameterSetEdit : public QWidget
{
    Q_OBJECT

protected:
    explicit AbstractByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);

public:
    ~AbstractByteArrayFilterParameterSetEdit() override;

public: // API to be implemented
    virtual void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) = 0;
    /// default does nothing
    virtual void setCharCodec(const QString& charCodecName);
    virtual void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const = 0;
    /// default returns true
    virtual bool isValid() const;
    /// default is a noop
    virtual void rememberCurrentSettings();

Q_SIGNALS:
    void validityChanged(bool isValid);

private:
    QScopedPointer<class AbstractByteArrayFilterParameterSetEditPrivate> const d;
};

#endif
