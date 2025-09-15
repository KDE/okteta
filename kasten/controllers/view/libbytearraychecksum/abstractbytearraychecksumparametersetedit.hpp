/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP
#define KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP

// Qt
#include <QWidget>
// Std
#include <memory>

class AbstractByteArrayChecksumParameterSet;

class AbstractByteArrayChecksumParameterSetEdit : public QWidget
{
    Q_OBJECT

protected:
    explicit AbstractByteArrayChecksumParameterSetEdit(QWidget* parent = nullptr);

public:
    ~AbstractByteArrayChecksumParameterSetEdit() override;

public: // API to be implemented
    virtual void setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet) = 0;
    /// default does nothing
    virtual void getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const = 0;
    /// default returns true
    [[nodiscard]]
    virtual bool isValid() const;

Q_SIGNALS:
    void validityChanged(bool isValid);
    // TODO: hack, see checksum source
    void valuesChanged();

private:
    const std::unique_ptr<class AbstractByteArrayChecksumParameterSetEditPrivate> d;
};

#endif
