var bitarray = require('./bitarray.js');

var myBitArray = new bitarray.BitArray(null, 10);

console.log(
    myBitArray.toString() + " = " + myBitArray.toNumber() + "\n"
    + "Compliment: " + myBitArray.getCompliment().toString() + " = " + myBitArray.getCompliment().toNumber() + "\n"
    + "Union with Comp: " + bitarray.BitArray.getUnion(myBitArray, myBitArray.getCompliment()) + "\n"
    + "Intersect w Comp: " + bitarray.BitArray.getIntersection(myBitArray, myBitArray.getCompliment()) + "\n"
    + "Difference w Comp: " + bitarray.BitArray.getDifference(myBitArray, myBitArray.getCompliment())
)

