function get_files
{
    echo okteta.xml
}

function po_for_file
{
    case "$1" in
       okteta.xml)
           echo okteta_xml_mimetypes.po
       ;;
    esac
}

function tags_for_file
{
    case "$1" in
      okteta.xml)
           echo comment
       ;;
    esac
}
