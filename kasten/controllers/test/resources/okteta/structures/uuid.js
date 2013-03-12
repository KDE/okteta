function UUID() {
    function uuidToString() {
        function padNumber(number, numDigits) {
            var ret = number;
            while (ret.length < numDigits) {
                ret = '0' + ret;
            }
            return ret;
        }
        var ret = "{" + padNumber(this.data1.value.toString(16), 8) + "-"
                + padNumber(this.data2.value.toString(16), 4) + "-"
                + padNumber(this.data3.value.toString(16), 4) + "-"
                + padNumber(this.data4[0].value.toString(16), 2)
                + padNumber(this.data4[1].value.toString(16), 2) + "-";
        for (var i = 2; i < 8; i++) {
            ret += padNumber(this.data4[i].value.toString(16), 2);
        }
        return ret + "}";
    }
    //TODO endianess
    var ret = struct({data1: uint32(), data2: uint16(), data3: uint16(), data4: array(uint8(), 8)});
    ret.byteOrder = "bigEndian";
    ret.toStringFunc = uuidToString;
    ret.typeName = "UUID";
    return ret;
}

function GUID() {
    var ret = UUID();
    //GUID uses native byte order for data1-3, but since AFAIK most code using it is on x86 use little endian
    ret.fields.data1.byteOrder = "littleEndian";
    ret.fields.data2.byteOrder = "littleEndian";
    ret.fields.data3.byteOrder = "littleEndian";
    ret.typeName = "GUID";
    return ret;
}
