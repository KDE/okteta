function init() {
    var object = struct({
        len: uint8(),
        dynArray : array(uint8(), 10, null, updateLength),
    },validateLength);
    return object;
}

function updateLength(mainStruct) {
    //set the array length to the correct length (no more than 100)
    var newLen = mainStruct.len.value;
    if (newLen > 100)
        newLen = 100;
    this.length = newLen;
}

function validateLength() {
    if (this.len.value > 100)
        this.len.validationError = "len must be no bigger than 100";
    else
        this.len.valid = true;
}
