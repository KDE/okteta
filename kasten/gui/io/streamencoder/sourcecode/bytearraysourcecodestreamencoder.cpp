/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysourcecodestreamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

static constexpr  const char* PrimitiveDataTypeName[] = {
    "char",
    "unsigned char",
    "short",
    "unsigned short",
    "int",
    "unsigned int",
    "float",
    "double"
};

static constexpr int SizeOfPrimitiveDataType[] =
{
    sizeof(char),
    sizeof(unsigned char),
    sizeof(short),
    sizeof(unsigned short),
    sizeof(int),
    sizeof(unsigned int),
    sizeof(float),
    sizeof(double)
};

static constexpr int NoOfPrimitiveDataTypes = 8;

inline QString decimalFormattedNumberPlaceHolder() { return QStringLiteral("%1"); }
inline QString hexadecimalFormattedNumberPlaceHolder() { return QStringLiteral("0x%1"); }

using TextStreamFunction = QTextStream& (*)(QTextStream&);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
constexpr TextStreamFunction endl = Qt::endl;
#else
constexpr TextStreamFunction endl = ::endl;
#endif

SourceCodeStreamEncoderSettings::SourceCodeStreamEncoderSettings()
    : variableName(QStringLiteral("array"))
{}

ByteArraySourceCodeStreamEncoder::ByteArraySourceCodeStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "C Array"), QStringLiteral("text/x-csrc"))
{}

ByteArraySourceCodeStreamEncoder::~ByteArraySourceCodeStreamEncoder() = default;

const char* const* ByteArraySourceCodeStreamEncoder::dataTypeNames() const { return PrimitiveDataTypeName; }
int ByteArraySourceCodeStreamEncoder::dataTypesCount() const { return NoOfPrimitiveDataTypes; }

bool ByteArraySourceCodeStreamEncoder::encodeDataToStream(QIODevice* device,
                                                          const ByteArrayView* byteArrayView,
                                                          const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                          const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView)

    bool success = true;

    // encode
    QTextStream textStream(device);

//     textStream << "// from File , selection \n";

    const int size = range.width();
    const int dataTypeSize = SizeOfPrimitiveDataType[static_cast<int>(mSettings.dataType)];
    const int sizeOfArray = (size + dataTypeSize - 1) / dataTypeSize;

    textStream << "const " << QLatin1String(PrimitiveDataTypeName[static_cast<int>(mSettings.dataType)]) << ' '
               << mSettings.variableName << '[' << sizeOfArray << "] =" << endl
               << '{' << endl;

    int elementAddedOnLine = 0;
    for (Okteta::Address i = range.start(); i <= range.end(); i += dataTypeSize) {
        if (elementAddedOnLine == 0) {
            textStream << "   "; // just 3, one space before every datum
        }
        textStream << ' ' << printFormatted(byteArrayModel, i, range.end() - i + 1);
        if (i + dataTypeSize <= range.end()) {
            textStream << ',';
        }

        if (++elementAddedOnLine >= mSettings.elementsPerLine) {
            textStream << endl;
            elementAddedOnLine = 0;
        }
    }

    if (elementAddedOnLine > 0) {
        textStream << endl;
    }

    textStream << "};" << endl;

    return success;
}

QString ByteArraySourceCodeStreamEncoder::printFormatted(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                                         unsigned int dataSize) const
{
    QString result;
    switch (mSettings.dataType)
    {
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Char:
    {
        char e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin<size_t>(sizeof(e), dataSize));
        constexpr int fieldWidth = 4;
        result = decimalFormattedNumberPlaceHolder().arg((int)e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedChar:
    {
        unsigned char e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 2 : 3;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Short:
    {
        short e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 6;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedShort:
    {
        unsigned short e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 4 : 5;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Integer:
    {
        int e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 11;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedInteger:
    {
        unsigned int e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 8 : 10;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Float:
    {
        float e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Double:
    {
        double e = 0;
        byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    }

    return result;
}

}
