/*
    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textcharcodec.hpp"

// lib
#include <character.hpp>
#include <logging.hpp>
// Qt
#include <QTextCodec>

namespace Okteta {

// static const char QTextCodecWhiteSpace = 63;

static constexpr struct EncodingData
{
    const char* name;
}
encodingDataList[] =
{
    { "ISO-8859-1" },
    { "ISO-8859-2" },
    { "ISO-8859-3" },
    { "ISO-8859-4" },
    { "ISO-8859-5" },
    { "ISO-8859-6" },
    { "ISO-8859-7" },
    { "ISO-8859-8" },
//     { ISO8859_8_IEncoding, "ISO-8859-8-I" }, Qt (at least 5.15) delivers this variant already for ISO-8859-8
    { "ISO-8859-9" },
    { "ISO-8859-10" },
    { "TIS-620" }, // was: ISO-8859-11
    { "ISO-8859-13" },
    { "ISO-8859-14" },
    { "ISO-8859-15" },
    { "ISO-8859-16" },
    { "windows-1250" },
    { "windows-1251" },
    { "windows-1252" },
    { "windows-1253" },
    { "windows-1254" },
    { "windows-1255" },
    { "windows-1256" },
    { "windows-1257" },
    { "windows-1258" },
    { "IBM850" },
    { "IBM866" },
//     { IBM874Encoding, "IBM874" }, using our own IBM874CharCodec, see docs there
    { "KOI8-R" },
    { "KOI8-U" }
};
// TODO: WS2

static bool is8Bit(QTextCodec* codec)
{
    bool result = false;

    const QByteArray& codecName = codec->name();
    for (const auto& encodingData : encodingDataList) {
        if (codecName.compare(encodingData.name) == 0) {
            result = true;
            break;
        }
    }

    return result;
}

static QTextCodec* createLatin1()
{
    return QTextCodec::codecForName(encodingDataList[0].name);
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
            QTextCodec* const Codec = KCharsets::charsets()->codecForName( *it, Found );
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

std::unique_ptr<TextCharCodec> TextCharCodec::createLocalCodec()
{
    QTextCodec* codec = QTextCodec::codecForLocale();
    if (!is8Bit(codec)) {
        codec = createLatin1();
    }
    return std::make_unique<TextCharCodec>(codec, ConstructorTag());
}

std::unique_ptr<TextCharCodec> TextCharCodec::createCodec(const QString& codecName)
{
    bool isOk = false;
    QTextCodec* const codec = QTextCodec::codecForName(codecName.toLatin1());
    if (codec) {
        isOk = is8Bit(codec);
    }
    if (!isOk) {
        return {};
    }
    return std::make_unique<TextCharCodec>(codec, ConstructorTag());
}

const QStringList& TextCharCodec::codecNames()
{
    static QStringList textCodecNames;

    // first call?
    if (textCodecNames.isEmpty()) {
        for (const auto& encodingData : encodingDataList) {
            QTextCodec* const codec = QTextCodec::codecForName((encodingData.name));
            if (codec) {
                textCodecNames.append(QString::fromLatin1(codec->name()));
            }
        }
    }

    return textCodecNames;
}

TextCharCodec::TextCharCodec(QTextCodec* textCodec, TextCharCodec::ConstructorTag)
    : mCodec(textCodec)
    , mDecoder(textCodec->makeDecoder())
    , mEncoder(textCodec->makeEncoder())
{
}

TextCharCodec::~TextCharCodec() = default;

bool TextCharCodec::canEncode(QChar _char) const
{
    return mCodec->canEncode(_char);
}

bool TextCharCodec::encode(Byte* byte, QChar _char) const
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

QString TextCharCodec::name() const
{
    if (mName.isNull()) {
        mName = QString::fromLatin1(mCodec->name());
    }

    return mName;
}

}
