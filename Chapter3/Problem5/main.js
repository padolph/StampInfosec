var BitArray = require('./bitarray');

var X = new BitArray(19, [1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1]),
    Y = new BitArray(22, [1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1]),
    Z = new BitArray(23, [1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0]);

var j, keyBits = [];
for (j=0; j < 32; j++) {
    keyBits.push(getKeyBit().toString());
}
console.log('KS:' + keyBits.join(''))
console.log('X: ' + X.toString());
console.log('Y: ' + Y.toString());
console.log('Z: ' + Z.toString());

function getKeyBit() {
    var m = maj([X.getAt(8), Y.getAt(10), Z.getAt(10)]);
    if (X.getAt(8) == m) {
        stepX();
    }
    if (Y.getAt(10) == m) {
        stepY();
    }
    if (Z.getAt(10) == m) {
        stepZ();
    }
    return X.getAt(18) ^ Y.getAt(21) ^ Z.getAt(22);
}

function stepX() {
    stepReg(X,[13,16,17,18]);
}

function stepY() {
    stepReg(Y,[20,21]);
}

function stepZ() {
    stepReg(Z,[7,20,21,22]);
}

function stepReg(ba, xorBits) {
    var t = xorBits.reduce(function(a, b) {
        return ba.getAt(a) ^ ba.getAt(b);
    }, 0);
    shiftReg(ba);
    ba.setAt(0, t);
}

function shiftReg(ba) {
    var i;
    for (i=ba.getLength()-1; i > 0; i--) {
        ba.setAt(i, ba.getAt(i-1));
    }
}

// function maj(a) {
//     var res = a.reduce(function(r, i) {
//         var newR = r;
//         if (r.cnt == 0) {
//             newR.cand = i;
//             newR.cnt = 1;
//         }
//         else {
//             (i == r.cand) ? newR.cnt++ : newR.cnt--;
//         }
//         return newR;
//     }, {cand: undefined, cnt: 0})
//     return res.cand;
// }

// optimized boolean maj from Problem 6
function maj(v) {
    var a=v[0], b=v[1], c=v[2];
    return (b && c) || (a && c) | (a && b);
}