var typeEnumValues = {
    char : 0,
    int16 :1,
    int32 : 2,
}

function init() {
    var object = struct({
        //should be no longer than 100
        len: uint8(),
        arrayType : enumeration("TypeEnum", uint8(), typeEnumValues),
        // length of this array is always equal to min(len.value, 100)
        dynArray : array(uint8(), 10),

        //this array changes type depending on the value of arrayType
        typeChangingArray : array(char(), 10),
    });
	object.validationFunc = validateLength;
	object.child("dynArray").updateFunc = updateLength;
	object.child("typeChangingArray").updateFunc = updateType;
    return object;
}

function updateLength(mainStruct) {
    //set the array length to the correct length (no more than 100)
    var newLen = mainStruct.len.value;
    if (newLen > 100)
        newLen = 100;
    this.length = newLen;
}

function updateType(mainStruct) {
    var type = mainStruct.arrayType.value;
    if (type == typeEnumValues.char && this.childType != char())
		this.childType = char();
    else if (type == typeEnumValues.int16 && this.childType != int16())
		this.childType = int16();
    else if (type == typeEnumValues.int32 && this.childType != int32())
		this.childType = int32();
}

function validateLength() {
    if (this.len.value > 100)
        this.len.validationError = "len must be no bigger than 100";
    else
        this.len.valid = true;
    var type = this.arrayType.value;
    if (type == 0 || type == 1 || type == 2)
        this.arrayType.valid = true;
    else
        this.arrayType.validationError = "value is not in enum";
}
