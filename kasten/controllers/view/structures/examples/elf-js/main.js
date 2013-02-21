
function validateIdentifier() {
    //check the magic values
    var magic = this["Magic"];
    magic.children[0].valid = magic.children[0].value.uint8 == 0x7f;
    magic.children[1].valid = magic.children[1].value.uint8 == 0x45; // 'E'
    magic.children[2].valid = magic.children[2].value.uint8 == 0x4c; // 'L'
    magic.children[3].valid = magic.children[3].value.uint8 == 0x46; // 'F'
}

/** initializer function: returns an object representing the new structure */
function init() {
    //define some enums first:
    var objectTypeEnumValues =  {
        "unknown" : 0,
        "relocatable" : 1,
        "executable" : 2,
        "shared object" : 3,
        "core file" : 4,
    };
    // type of the enum (first parameter) can also be a String
    var architectureEnumValues = {
        "No machine" : 0,
        "AT&T WE 32100" : 1,
        "SUN SPARC" : 2,
        "Intel 80386" : 3,
        "Motorola m68k family" : 4,
        "Motorola m88k family" : 5,
        "Intel 80860" : 7,
        "MIPS R3000 big-endian" : 8,
        "IBM System/370" : 9,
        "MIPS R3000 little-endian" : 10,
        "HPPA" : 15,
        "Fujitsu VPP500" : 17,
        "Sun's v8plus" : 18,
        "Intel 80960" : 19,
        "PowerPC" : 20,
        "PowerPC 64-bit" : 21,
        "IBM S390" : 22,
        "NEC V800 series" : 36,
        "Fujitsu FR20" : 37,
        "TRW RH-32" : 38,
        "Motorola RCE" : 39,
        "ARM" : 40,
        "Digital Alpha" : 41,
        "Hitachi SH" : 42,
        "SPARC v9 64-bit" : 43,
        "Siemens Tricore" : 44,
        "Argonaut RISC Core" : 45,
        "Hitachi H8/300" : 46,
        "Hitachi H8/300H" : 47,
        "Hitachi H8S" : 48,
        "Hitachi H8/500" : 49,
        "Intel Merced" : 50,
        "Stanford MIPS-X" : 51,
        "Motorola Coldfire" : 52,
        "Motorola M68HC12" : 53,
        "Fujitsu MMA Multimedia Accelerator" : 54,
        "Siemens PCP" : 55,
        "Sony nCPU embeeded RISC" : 56,
        "Denso NDR1 microprocessor" : 57,
        "Motorola Start*Core processor" : 58,
        "Toyota ME16 processor" : 59,
        "STMicroelectronic ST100 processor" : 60,
        "Advanced Logic Corp. Tinyj emb.fam" : 61,
        "AMD x86-64 architecture" : 62,
        "Sony DSP Processor" : 63,
        "Siemens FX66 microcontroller" : 66,
        "STMicroelectronics ST9+ 8/16 mc" : 67,
    };
    var classEnumValues = {
        "None" : 0,
        "32 bit" : 1,
        "64 bit" : 2,
    };

    var endianessEnumValues = {
        "None" : 0,
        "little endian" : 1,
        "big endian" : 2,
    };

    var ABIEnumValues = {
        "UNIX System V ABI" : 0,
        "HP-UX" : 1,
        "NetBSD" : 2,
        "Linux" : 3,
        "Sun Solaris" : 6,
        "IBM AIX" : 7,
        "SGI Irix" : 8,
        "FreeBSD" : 9,
        "Compaq TRU64 UNIX" : 10,
        "Novell Modesto" : 11,
        "OpenBSD" : 12,
        "ARM" : 97,
        "Standalone (embedded) application" : 255,
    };
    // now define the real object

    /** the same for 32 and 64 bit */
    var ident = struct({
        Magic : struct({
            Magic1 : char(),
            Magic2 : char(),
            Magic3 : char(),
            Magic4 : char(),
        }),
        ArchitectureClass : enumeration("ClassEnum", uint8(), classEnumValues),
        Endianess : enumeration("EndianessEnum", "UInt8", endianessEnumValues),
        Version : enumeration("ElfVersionEnum", uint8(), { NONE : 0, CURRENT : 1, NUM : 2}),
        ABI : enumeration("ABIEnum", uint8(), ABIEnumValues),
        ABI_Version : uint8(),
        Padding : array(uint8(), 7),
    }).setValidation(validateIdentifier);



    var phoff = pointer(uint32(), struct({
        "Section Headers": array(
            struct({
                p_type: uint32(),
                p_offset: pointer(uint32(), {
                    Data: array(uint8(), function() { return this.parent.parent.PHSize.value; })
                }),
                p_vaddr: uint32(),
                p_paddr: uint32(),
                p_filesz: uint32(),
                p_memsz: uint32(),
                p_flags: uint32(),
                p_align: uint32()
            }).set({name: "Elf32_Phdr"}), function(root) { return root.PHCount.value; }),
    }));
    var shoff = pointer(uint32(), struct({
        "Section Headers": array(
            struct({
                sh_name: uint32(),
                sh_type: uint32(),
                sh_flags: uint32(),
                sh_addr: uint32(),
                sh_offset: pointer(uint32(), {
                    Data: array(uint8(), function() { return this.parent.parent.parent.SHSize.value; })
                }),
             }).set({name: "Elf32_Phdr"}), function(root) { return root.SHCount.value; }),
    }));

    var elf32 = struct({
        Identifier : ident,
        ObjectType : enumeration("ObjectTypeEnum", uint16(), objectTypeEnumValues),
        Architecture : enumeration("ArchitectureEnum", uint16(), architectureEnumValues),
        Version : uint32(),
        EntryAddr : uint32(),
        PHOffset : phoff,
        SHOffset : shoff,
        Flags : uint32(),
        HeaderSize : uint16(),
        PHSize : uint16(),
        PHCount : uint16(),
        SHSize : uint16(),
        SHCount : uint16(),
        SHStringIndex : uint16(),
    });
    return elf32;
}
