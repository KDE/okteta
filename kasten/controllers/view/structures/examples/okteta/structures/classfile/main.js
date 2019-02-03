var constantType = {
    'Class': 7,
    'Fieldref': 9,
    'Methodref': 10,
    'InterfaceMethodref': 11,
    'String': 8,
    'Integer': 3,
    'Float': 4,
    'Long': 5,
    'Double': 6,
    'NameAndType': 12,
    'Utf8': 1,
    'MethodHandle': 15,
    'MethodType': 16,
    'InvokeDynamic': 18
};

var classAccessFlags = {
    'public': 0x0001,
    'final': 0x0010,
    'super': 0x0020,
    'interface': 0x0200,
    'abstract': 0x0400,
    'synthetic': 0x1000,
    'annotation': 0x2000,
    'enum': 0x4000
};

/** represents an index into the constant pool array (actually just a uint16) */
function cpIndex() {
    function validateCpIndex(root) {
        return this.value < root.constant_pool.length;
    }
    //TODO custom to string
    var ret = uint16();
    ret.typeName = "cp_index";
    ret.validationFunc = validateCpIndex;
    return ret;
}

/* create an object corresponding to the class file cp_info union */
function cp_info() {
    //the list of possible interpretations of this structure:
    var cpAlternatives = [
    //this is the really verbose way of defining an alternative in the tagged union:
    {
        structName: 'CONSTANT_Class_info',
        selectIf: function(root) {
            return this.tag.value == constantType.Class;
        },
        fields: {
            name_index: cpIndex()
        }
    },
    // we can also use a less verbose version using the predefined alternative() function
    alternative(function(root) { return this.tag.value == constantType.String; }, //selectIf
            { name_index: cpIndex() }, //fields
            'CONSTANT_String_info'), //structName

    //it can be even shorter since there is only one value in this tagged union that can be used to decide
    //therefore just passing the value as an argument is enough
    alternative(constantType.Integer, { value : int32() }, 'CONSTANT_Integer_info'),

    //obviously this also works if we define the object manually
    {
        structName: 'CONSTANT_Float_info',
        selectIf: constantType.Float,
        fields: { value: float() }
    },
    //just use the shortest syntax for the remaining alternatives
    alternative(constantType.Long, {
            value : int64()
        }, 'CONSTANT_Long_info'),

    alternative(constantType.Double, {
            value : double()
        }, 'CONSTANT_Double_info'),

    alternative(constantType.Fieldref, {
            class_index: cpIndex(),
            name_and_type_index: cpIndex()
        }, 'CONSTANT_Fieldref_info'),

    alternative(constantType.Methodref, {
            class_index: cpIndex(),
            name_and_type_index: cpIndex()
        }, 'CONSTANT_Methodref_info'),

    alternative(constantType.InterfaceMethodref, {
            class_index: cpIndex(),
            name_and_type_index: cpIndex()
        }, 'CONSTANT_InterfaceMethodref_info'),

    alternative(constantType.NameAndType, {
            name_index: cpIndex(),
            descriptor_index: cpIndex()
        }, 'CONSTANT_NameAndType_info'),

    alternative(constantType.MethodHandle, {
            reference_kind: uint8(),
            reference_index: cpIndex()
        }, 'CONSTANT_MethodHandle_info'),

    alternative(constantType.MethodType, {
            descriptor_index: cpIndex()
        }, 'CONSTANT_MethodType_info'),

    alternative(constantType.InvokeDynamic, {
            bootstrap_method_attr_index: cpIndex(),
            name_and_type_index: cpIndex()
        }, 'CONSTANT_InvokeDynamic_info'),

    alternative(constantType.Utf8, {
            length: uint16(),
            bytes: string('utf8').set({
                maxByteCount: 0,
                updateFunc: function() { this.maxByteCount = this.parent.length.value; }
            })
        }, 'CONSTANT_Utf8_info')
    ];
    /* first argument is the fixed elements, second is the alternatives, and last is the default fields (if none of the alternatives match) */
    return taggedUnion({ tag: enumeration('ConstantType', uint8(), constantType)}, cpAlternatives, { dummy: uint8() });
}

function init() {
    //from https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html
    /*
    ClassFile {
        u4             magic;
        u2             minor_version;
        u2             major_version;
        u2             constant_pool_count;
        cp_info        constant_pool[constant_pool_count-1];
        u2             access_flags;
        u2             this_class;
        u2             super_class;
        u2             interfaces_count;
        u2             interfaces[interfaces_count];
        //TODO
        u2             fields_count;
        field_info     fields[fields_count];
        u2             methods_count;
        method_info    methods[methods_count];
        u2             attributes_count;
        attribute_info attributes[attributes_count];
    }
    */

    var classFile = struct({
        magic: uint32(),
        minor_version: uint16(),
        major_version: uint16(),
        constant_pool_count: uint16(),
        /* length is always constant_pool_count - 1 */
        constant_pool: array(cp_info(), function(root) { return root.constant_pool_count.value - 1; }),
        access_flags: flags('ClassAccessFlags', uint16(), classAccessFlags),
        this_class: cpIndex(),
        super_class: cpIndex(),
        interfaces_count: uint16(),
        interfaces: array(cpIndex(), function(root) { return root.interfaces_count.value; }),

    // interfaces[interfaces_count] : uint32(),
    // fields_count : uint32(),
    // field_info fields[fields_count];
    // methods_count : uint32(),
    // method_info methods[methods_count];
    // attributes_count : uint32(),
    // attribute_info attributes[attributes_count];
    });
    //classfile is always stored in big endian
    classFile.byteOrder = "bigEndian";
    return classFile;
}
