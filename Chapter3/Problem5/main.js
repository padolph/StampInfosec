var BitArray = require('./bitarray');

var myBitArray = new BitArray(null, 10);

console.log(
    myBitArray.toString() + " = " + myBitArray.toNumber() + "\n"
    + "Compliment: " + myBitArray.getCompliment().toString() + " = " + myBitArray.getCompliment().toNumber() + "\n"
    + "Union with Comp: " + BitArray.getUnion(myBitArray, myBitArray.getCompliment()) + "\n"
    + "Intersect w Comp: " + BitArray.getIntersection(myBitArray, myBitArray.getCompliment()) + "\n"
    + "Difference w Comp: " + BitArray.getDifference(myBitArray, myBitArray.getCompliment())
)

