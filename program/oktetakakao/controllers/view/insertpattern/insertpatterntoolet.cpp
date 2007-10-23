void InsertPatternToolet::insertPattern()
{
    // ensure dialog
    if( !mInsertPatternDialog )
    {
        mInsertPatternDialog = new KInsertPatternDialog( mMainWindow );
        connect( mInsertPatternDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }
    InsertPatternDialog->setCharCode( HexEdit->encodingName() );

    InsertPatternDialog->show();
}

void InsertPatternToolet::onOkClicked()
{
    InsertPatternDialog->hide();

    const QByteArray Pattern = InsertPatternDialog->pattern();
    const int PatternNumber = InsertPatternDialog->number();
    const int PatternSize = Pattern.size();

    const int InsertDataSize = PatternNumber * Pattern.size();
    QByteArray InsertData( InsertDataSize, '\0' );

    for( int i=0; i < InsertDataSize; i+= PatternSize )
        memcpy( &InsertData.data()[i], Pattern.constData(), PatternSize );

    //TODO: support insert to selection
    HexEdit->insert( InsertData );
}

InsertPatternToolet::~InsertPatternToolet() {}
