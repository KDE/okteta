/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "poddelegate.h"

// tool
#include "typeeditors/binary8editor.h"
#include "typeeditors/octal8editor.h"
#include "typeeditors/hexadecimal8editor.h"
#include "typeeditors/float32editor.h"
#include "typeeditors/float64editor.h"
#include "typeeditors/char8editor.h"
#include "typeeditors/utf8editor.h"
#include "typeeditors/sintspinbox.h"
#include "typeeditors/uintspinbox.h"
#include "poddecodertool.h"
#include "types/sint8.h"
#include "types/sint16.h"
#include "types/sint32.h"
#include "types/sint64.h"
#include "types/uint8.h"
#include "types/uint16.h"
#include "types/uint32.h"
#include "types/uint64.h"


// TODO: Stranger that you are reading this: please help out and show how to add QVariant::Types for custom datatypes,
// so that instead of this unflexible maintanance mess^WWWunlooped code QItemEditorCreator can be used!

namespace Kasten2
{

PODDelegate::PODDelegate( PODDecoderTool* tool, QObject* parent )
  : QStyledItemDelegate( parent ),
    mTool( tool ),
    mEditor( 0 )
{
    qRegisterMetaType<Binary8>();
    qRegisterMetaType<Octal8>();
    qRegisterMetaType<Hexadecimal8>();
    qRegisterMetaType<SInt8>();
    qRegisterMetaType<SInt16>();
    qRegisterMetaType<SInt32>();
    qRegisterMetaType<SInt64>();
    qRegisterMetaType<UInt8>();
    qRegisterMetaType<UInt16>();
    qRegisterMetaType<UInt32>();
    qRegisterMetaType<UInt64>();
    qRegisterMetaType<Float32>();
    qRegisterMetaType<Float64>();
    qRegisterMetaType<Char8>();
    qRegisterMetaType<Utf8>();

    connect( mTool, SIGNAL(readOnlyChanged(bool)), SLOT(onReadOnlyChanged(bool)) );
}

// make sure only editors are created which have a readOnly property
// also beware that for subclasses of QLineEdit (all float editors) the signal
// editingFinished() is only emitted if validator() returns QValidator::Acceptable
// so onEditorDone() is not reached if QValidator::Intermediate
QWidget* PODDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QWidget* result;

    QVariant data = index.data();
    if( data.canConvert<Binary8>() )
    {
        Binary8Editor* editor = new Binary8Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Octal8>() )
    {
        Octal8Editor* editor = new Octal8Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Hexadecimal8>() )
    {
        Hexadecimal8Editor* editor = new Hexadecimal8Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<SInt8>() )
    {
        SIntSpinBox* editor = SIntSpinBox::createSInt8Spinbox( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<SInt16>() )
    {
        SIntSpinBox* editor = SIntSpinBox::createSInt16Spinbox( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<SInt32>() )
    {
        SIntSpinBox* editor = SIntSpinBox::createSInt32Spinbox( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<SInt64>() )
    {
        SIntSpinBox* editor = SIntSpinBox::createSInt64Spinbox( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<UInt8>() )
    {
        UIntSpinBox* editor = UIntSpinBox::createUInt8Spinbox( parent );
        editor->setBase( mTool->isUnsignedAsHex() ? 16 : 10 );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<UInt16>() )
    {
        UIntSpinBox* editor = UIntSpinBox::createUInt16Spinbox( parent );
        editor->setBase( mTool->isUnsignedAsHex() ? 16 : 10 );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<UInt32>() )
    {
        UIntSpinBox* editor = UIntSpinBox::createUInt32Spinbox( parent );
        editor->setBase( mTool->isUnsignedAsHex() ? 16 : 10 );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<UInt64>() )
    {
        UIntSpinBox* editor = UIntSpinBox::createUInt64Spinbox( parent );
        editor->setBase( mTool->isUnsignedAsHex() ? 16 : 10 );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Float32>() )
    {
        Float32Editor* editor = new Float32Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Float64>() )
    {
        Float64Editor* editor = new Float64Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Char8>() )
    {
        Char8Editor* editor = new Char8Editor( mTool->charCodec(), parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else if( data.canConvert<Utf8>() )
    {
        Utf8Editor* editor = new Utf8Editor( parent );
        connect( editor, SIGNAL(editingFinished()),
                 SLOT(onEditorDone()) );
        result = editor;
    }
    else
        result = QStyledItemDelegate::createEditor( parent, option, index );

    mEditor = result;
    onReadOnlyChanged( mTool->isReadOnly() );

    return result;
}

void PODDelegate::setEditorData( QWidget* editor, const QModelIndex& index ) const
{
    QVariant data = index.data();

    if( data.canConvert<Binary8>() )
    {
        Binary8 binary8 = data.value<Binary8>();
        Binary8Editor* binary8Editor = qobject_cast<Binary8Editor*>( editor );
        binary8Editor->setData( binary8 );
    }
    else if( data.canConvert<Octal8>() )
    {
        Octal8 octal8 = data.value<Octal8>();
        Octal8Editor* octal8Editor = qobject_cast<Octal8Editor*>( editor );
        octal8Editor->setData( octal8 );
    }
    else if( data.canConvert<Hexadecimal8>() )
    {
        Hexadecimal8 hexadecimal8 = data.value<Hexadecimal8>();
        Hexadecimal8Editor* hexadecimal8Editor = qobject_cast<Hexadecimal8Editor*>( editor );
        hexadecimal8Editor->setData( hexadecimal8 );
    }
    else if( data.canConvert<SInt8>() )
    {
        SInt8 sInt8 = data.value<SInt8>();
        SIntSpinBox* sInt8Editor = qobject_cast<SIntSpinBox*>( editor );
        sInt8Editor->setValue( sInt8.value );
    }
    else if( data.canConvert<SInt16>() )
    {
        SInt16 sInt16 = data.value<SInt16>();
        SIntSpinBox* sInt16Editor = qobject_cast<SIntSpinBox*>( editor );
        sInt16Editor->setValue( sInt16.value );
    }
    else if( data.canConvert<SInt32>() )
    {
        SInt32 sInt32 = data.value<SInt32>();
        SIntSpinBox* sInt32Editor = qobject_cast<SIntSpinBox*>( editor );
        sInt32Editor->setValue( sInt32.value );
    }
    else if( data.canConvert<SInt64>() )
    {
        SInt64 sInt64 = data.value<SInt64>();
        SIntSpinBox* sInt64Editor = qobject_cast<SIntSpinBox*>( editor );
        sInt64Editor->setValue( sInt64.value );
    }
    else if( data.canConvert<UInt8>() )
    {
        UInt8 uInt8 = data.value<UInt8>();
        UIntSpinBox* uInt8Editor = qobject_cast<UIntSpinBox*>( editor );
        uInt8Editor->setValue( uInt8.value );
    }
    else if( data.canConvert<UInt16>() )
    {
        UInt16 uInt16 = data.value<UInt16>();
        UIntSpinBox* uInt16Editor = qobject_cast<UIntSpinBox*>( editor );
        uInt16Editor->setValue( uInt16.value );
    }
    else if( data.canConvert<UInt32>() )
    {
        UInt32 uInt32 = data.value<UInt32>();
        UIntSpinBox* uInt32Editor = qobject_cast<UIntSpinBox*>( editor );
        uInt32Editor->setValue( uInt32.value );
    }
    else if( data.canConvert<UInt64>() )
    {
        UInt64 uInt64 = data.value<UInt64>();
        UIntSpinBox* uInt64Editor = qobject_cast<UIntSpinBox*>( editor );
        uInt64Editor->setValue( uInt64.value );
    }
    else if( data.canConvert<Float32>() )
    {
        Float32 float32 = data.value<Float32>();
        Float32Editor* float32Editor = qobject_cast<Float32Editor*>( editor );
        float32Editor->setData( float32 );
    }
    else if( data.canConvert<Float64>() )
    {
        Float64 float64 = data.value<Float64>();
        Float64Editor* float64Editor = qobject_cast<Float64Editor*>( editor );
        float64Editor->setData( float64 );
    }
    else if( data.canConvert<Char8>() )
    {
        Char8 char8 = data.value<Char8>();
        Char8Editor* char8Editor = qobject_cast<Char8Editor*>( editor );
        char8Editor->setData( char8 );
    }
    else if( data.canConvert<Utf8>() )
    {
        Utf8 utf8 = data.value<Utf8>();
        Utf8Editor* utf8Editor = qobject_cast<Utf8Editor*>( editor );
        utf8Editor->setData( utf8 );
    }
    else
        QStyledItemDelegate::setEditorData( editor, index );
}

void PODDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const
{
    if( mTool->isReadOnly() )
        return;

    QVariant data = index.data();

    if( data.canConvert<Binary8>() )
    {
        Binary8Editor* binary8Editor = qobject_cast<Binary8Editor*>( editor );
        model->setData( index, QVariant::fromValue(binary8Editor->data()) );
    }
    else if( data.canConvert<Octal8>() )
    {
        Octal8Editor* octal8Editor = qobject_cast<Octal8Editor*>( editor );
        model->setData( index, QVariant::fromValue(octal8Editor->data()) );
    }
    else if( data.canConvert<Hexadecimal8>() )
    {
        Hexadecimal8Editor* hexadecimal8Editor = qobject_cast<Hexadecimal8Editor*>( editor );
        model->setData( index, QVariant::fromValue(hexadecimal8Editor->data()) );
    }
    else if( data.canConvert<SInt8>() )
    {
        SIntSpinBox* editor = qobject_cast<SIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( SInt8(editor->value()) ) );
    }
    else if( data.canConvert<SInt16>() )
    {
        SIntSpinBox* editor = qobject_cast<SIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( SInt16(editor->value()) ) );
    }
    else if( data.canConvert<SInt32>() )
    {
        SIntSpinBox* editor = qobject_cast<SIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( SInt32(editor->value()) ) );
    }
    else if( data.canConvert<SInt64>() )
    {
        SIntSpinBox* editor = qobject_cast<SIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( SInt64(editor->value()) ) );
    }
    else if( data.canConvert<UInt8>() )
    {
        UIntSpinBox* editor = qobject_cast<UIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( UInt8(editor->value()) ) );
    }
    else if( data.canConvert<UInt16>() )
    {
        UIntSpinBox* editor = qobject_cast<UIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( UInt16(editor->value()) ) );
    }
    else if( data.canConvert<UInt32>() )
    {
        UIntSpinBox* editor = qobject_cast<UIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( UInt32(editor->value()) ) );
    }
    else if( data.canConvert<UInt64>() )
    {
        UIntSpinBox* editor = qobject_cast<UIntSpinBox*>( editor );
        model->setData( index, QVariant::fromValue( UInt64(editor->value()) ) );
    }
    else if( data.canConvert<Float32>() )
    {
        Float32Editor* float32Editor = qobject_cast<Float32Editor*>( editor );
        model->setData( index, QVariant::fromValue(float32Editor->data()) );
    }
    else if( data.canConvert<Float64>() )
    {
        Float64Editor* float64Editor = qobject_cast<Float64Editor*>( editor );
        model->setData( index, QVariant::fromValue(float64Editor->data()) );
    }
    else if( data.canConvert<Char8>() )
    {
        Char8Editor* char8Editor = qobject_cast<Char8Editor*>( editor );
        model->setData( index, QVariant::fromValue(char8Editor->data()) );
    }
    else if( data.canConvert<Utf8>() )
    {
        Utf8Editor* utf8Editor = qobject_cast<Utf8Editor*>( editor );
        model->setData( index, QVariant::fromValue(utf8Editor->data()) );
    }
    else
        QStyledItemDelegate::setModelData( editor, model, index );
}

QString PODDelegate::displayText( const QVariant& data, const QLocale& locale ) const
{
    QString result;

    if( data.canConvert<Binary8>() )
    {
        Binary8 binary8 = data.value<Binary8>();
        result = binary8.toString();
    }
    else if( data.canConvert<Octal8>() )
    {
        Octal8 octal8 = data.value<Octal8>();
        result = octal8.toString();
    }
    else if( data.canConvert<Hexadecimal8>() )
    {
        Hexadecimal8 hexadecimal8 = data.value<Hexadecimal8>();
        result = hexadecimal8.toString();
    }
    else if( data.canConvert<SInt8>() )
    {
        SInt8 sInt8 = data.value<SInt8>();
        result = sInt8.toString();
    }
    else if( data.canConvert<SInt16>() )
    {
        SInt16 sInt16 = data.value<SInt16>();
        result = sInt16.toString();
    }
    else if( data.canConvert<SInt32>() )
    {
        SInt32 sInt32 = data.value<SInt32>();
        result = sInt32.toString();
    }
    else if( data.canConvert<SInt64>() )
    {
        SInt64 sInt64 = data.value<SInt64>();
        result = sInt64.toString();
    }
    else if( data.canConvert<UInt8>() )
    {
        UInt8 uInt8 = data.value<UInt8>();
        result = uInt8.toString( mTool->isUnsignedAsHex() );
    }
    else if( data.canConvert<UInt16>() )
    {
        UInt16 uInt16 = data.value<UInt16>();
        result = uInt16.toString( mTool->isUnsignedAsHex() );
    }
    else if( data.canConvert<UInt32>() )
    {
        UInt32 uInt32 = data.value<UInt32>();
        result = uInt32.toString( mTool->isUnsignedAsHex() );
    }
    else if( data.canConvert<UInt64>() )
    {
        UInt64 uInt64 = data.value<UInt64>();
        result = uInt64.toString( mTool->isUnsignedAsHex() );
    }
    else if( data.canConvert<Float32>() )
    {
        Float32 float32 = data.value<Float32>();
        result = float32.toString();
    }
    else if( data.canConvert<Float64>() )
    {
        Float64 float64 = data.value<Float64>();
        result = float64.toString();
    }
    else if( data.canConvert<Char8>() )
    {
        Char8 char8 = data.value<Char8>();
        result = char8.toString();
    }
    else if( data.canConvert<Utf8>() )
    {
        Utf8 utf8 = data.value<Utf8>();
        result = utf8.toString();
    }
    else
        result = QStyledItemDelegate::displayText( data, locale );

    return result;
}

// QSize PODDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const
// {
//      editor->setGeometry(option.rect);
// }

void PODDelegate::onEditorDone()
{
    QWidget* editor = qobject_cast<QWidget*>( sender() );
    mEditor = 0;
    emit commitData( editor );
    emit closeEditor( editor );
}

void PODDelegate::onReadOnlyChanged( bool isReadOnly ) const
{
    if( mEditor )
        // going by property is slower, but saves writing this call for every widget
        mEditor->setProperty( "readOnly", isReadOnly );
}

PODDelegate::~PODDelegate() {}

}
