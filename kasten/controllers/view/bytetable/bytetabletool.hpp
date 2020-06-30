/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTETABLETOOL_HPP
#define KASTEN_BYTETABLETOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class ByteTableModel;
class ByteArrayView;

/**
 */
class ByteTableTool : public AbstractTool
{
    Q_OBJECT

public:
    ByteTableTool();
    ~ByteTableTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public:
    void insert(unsigned char byte, int count);

public:
    ByteTableModel* byteTableModel() const;
    bool hasWriteable() const;

Q_SIGNALS:
    void hasWriteableChanged(bool hasWriteable);

private Q_SLOTS:
    void onReadOnlyChanged(bool isReadOnly);

private:
    ByteTableModel* mByteTableModel;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

}

#endif
