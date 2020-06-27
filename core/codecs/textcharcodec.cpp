/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textcharcodec.hpp"

// lib
#include <character.hpp>
// KF
#include <KCharsets>
// Qt
#include <QTextCodec>

namespace Okteta {

// static const char QTextCodecWhiteSpace = 63;

static constexpr struct EncodingData
{
    CharCoding encodingId;
    const char* name;
}
encodingDataList[] =
{
    { ISO8859_1Encoding, "ISO-8859-1" },
    { ISO8859_2Encoding, "ISO-8859-2" },
    { ISO8859_3Encoding, "ISO-8859-3" },
    { ISO8859_4Encoding, "ISO-8859-4" },
    { ISO8859_5Encoding, "ISO-8859-5" },
    { ISO8859_6Encoding, "ISO-8859-6" },
    { ISO8859_7Encoding, "ISO-8859-7" },
    { ISO8859_8Encoding, "ISO-8859-8" },
    { ISO8859_8_IEncoding, "ISO-8859-8-I" },
    { ISO8859_9Encoding, "ISO-8859-9" },
    { ISO8859_10Encoding, "ISO-8859-10" },
    { ISO8859_11Encoding, "TIS-620" }, // was: ISO-8859-11
    { ISO8859_13Encoding, "ISO-8859-13" },
    { ISO8859_14Encoding, "ISO-8859-14" },
    { ISO8859_15Encoding, "ISO-8859-15" },
    { ISO8859_16Encoding, "ISO-8859-16" },
    { CP1250Encoding, "windows-1250" },
    { CP1251Encoding, "windows-1251" },
    { CP1252Encoding, "windows-1252" },
    { CP1253Encoding, "windows-1253" },
    { CP1254Encoding, "windows-1254" },
    { CP1255Encoding, "windows-1255" },
    { CP1256Encoding, "windows-1256" },
    { CP1257Encoding, "windows-1257" },
    { CP1258Encoding, "windows-1258" },
    { IBM850Encoding, "IBM850" },
    { IBM866Encoding, "IBM866" },
    { IBM874Encoding, "IBM874" },
    { KOI8_REncoding, "KOI8-R" },
    { KOI8_UEncoding, "KOI8-U" }
};
// TODO: WS2

static bool is8Bit(QTextCodec* codec)
{
    bool result = false;

    const QByteArray& codecName = codec->name();
    for (auto& encodingData : encodingDataList) {
        if (qstrcmp(codecName, encodingData.name) == 0) {
            result = true;
            break;
        }
    }

    return result;
}

static QTextCodec* createLatin1()
{
    return KCharsets::charsets()->codecForName(QLatin1String(encodingDataList[0].name));
}

/* heuristic seems to be doomed :(
static bool is8Bit( QTextCodec *Codec )
{
    bool Result = true;

    // first test different for 0
    unsigned char c[4];
    c[0] = 0;
    c[1] = c[2] = c[3] = 230;
    QString S = Codec->toUnicode( (const char*)&c,4 );
    int Length = 1;
    QCString CS = Codec->fromUnicode( S, Length );
    //qCDebug(LOG_OKTETA_CORE) << Codec->name() << " "<<Length ;
    if( Length > 0 )
        Result = false;
    // test if all chars survive the recoding
    else
    do {
        ++c[0];
        S = Codec->toUnicode( (const char*)&c,4 );
        Length = 1;
        CS = Codec->fromUnicode( S, Length );
        //qCDebug(LOG_OKTETA_CORE) << Codec->name() << " "<<c[0]<<"->"<<CS[0]<<":"<<Length ;
        if( Length != 1 || (CS[0] != (char)c[0] && CS[0] != QTextCodecWhiteSpace) ) {
            Result = false;
            break;
        }
    } while( c[0] < 255 );
    return Result;
}
const QStringList &TextCharCodec::codecNames()
    {
    // first call?
    if( CodecNames.isEmpty() ) {
        const QStringList &CharSets = KCharsets::charsets()->availableEncodingNames();

        for( QStringList::ConstIterator it = CharSets.begin(); it != CharSets.end(); ++it ) {
            bool Found = true;
            QTextCodec* Codec = KCharsets::charsets()->codecForName( *it, Found );
            if( Found && is8Bit(Codec) )
                CodecNames.append( QString::fromLatin1(Codec->name()) );
        }
    }

    return CodecNames;
}

QString TextCharCodec::nameOfEncoding( CharCoding _char )
{
    TextCharCodec *Codec = 0;

    const char* N = 0;
    for( unsigned int i=0; i<NoOfEncodings; ++i )
    {
        if( EncodingNames[i].Encoding == _char )
        {
            N = EncodingNames[i].Name;
            break;
        }
    }

    if( N != 0 )
    {
        QString CodeName = QString::fromLatin1( N );
    }
    return Codec;
}
 */

TextCharCodec* TextCharCodec::createLocalCodec()
{
    QTextCodec* codec = QTextCodec::codecForLocale();
    if (!is8Bit(codec)) {
        codec = createLatin1();
    }
    return new TextCharCodec(codec);
}

TextCharCodec* TextCharCodec::createCodec(const QString& codecName)
{
    bool isOk = false;
    QTextCodec* codec = KCharsets::charsets()->codecForName(codecName, isOk);
    if (isOk) {
        isOk = is8Bit(codec);
    }
    return isOk ? new TextCharCodec(codec) : nullptr;
}

const QStringList& TextCharCodec::codecNames()
{
    static QStringList textCodecNames;

    // first call?
    if (textCodecNames.isEmpty()) {
        KCharsets* charsets = KCharsets::charsets();
        for (auto& encodingData : encodingDataList) {
            bool isCodecFound = false;
            const QString codecName = QString::fromLatin1(encodingData.name);
            QTextCodec* codec = charsets->codecForName(codecName, isCodecFound);
            if (isCodecFound) {
                textCodecNames.append(QString::fromLatin1(codec->name()));
            }
        }
    }

    return textCodecNames;
}

TextCharCodec::TextCharCodec(QTextCodec* textCodec)
    : mCodec(textCodec)
    , mDecoder(textCodec->makeDecoder())
    , mEncoder(textCodec->makeEncoder())
{
}

TextCharCodec::~TextCharCodec()
{
    delete mDecoder;
    delete mEncoder;
}

bool TextCharCodec::canEncode(const QChar& _char) const
{
    return mCodec->canEncode(_char);
}

bool TextCharCodec::encode(Byte* byte, const QChar& _char) const
{
    if (!mCodec->canEncode(_char)) { // TODO: do we really need the codec?
        return false;
    }

    const QByteArray encoded = mEncoder->fromUnicode(QString(_char));
    if (encoded.size() > 0) {
        *byte = encoded.at(0);
        return true;
    }

    return false;
}

Character TextCharCodec::decode(Byte byte) const
{
    // QTextCodecs "use this codepoint when input data cannot be represented in Unicode." (Qt docs)
    constexpr QChar replacementChar = QChar(QChar::ReplacementCharacter);
    const QString string =
        mDecoder->toUnicode(reinterpret_cast<const char*>(&byte), 1);
    const QChar qchar = string.at(0);
    const bool isDecoded = (qchar != replacementChar);
    return {qchar, !isDecoded};
}

const QString& TextCharCodec::name() const
{
    if (mName.isNull()) {
        mName = QString::fromLatin1(mCodec->name());
    }

    return mName;
}

}
