
/** initializer function: returns an object representing the new structure */
function init() {
	var obj = struct({
		/* equivalent to writing member1 : new uint32() */
		member1 : uint32(),
		/* Following in C++:
		 * union member2 {
		 *	float aFloat;
		 *	char charArray[4];
		 *	uint32 unsignedInt;
		 *	unsigned bitfield : 10;
		 * };
		 */
		member2 : union({
			aFloat : float(),
			charArray : array(char(),4),
			unsignedInt : uint32(),
			bitField : bitfield("unsigned",10),
			}),
		member3 : double(),
		/* C++: struct {char a; charb;} member4[10] */
		member4 : array(struct({
				    a : char(),
				    b: char()
			    }), 10),
		/** bool member5 : 1; */
		member5 : bitfield("bool",1),
	});
	return obj
}
var q = init() //just to start debugger, otherwise it would just check for syntax errors