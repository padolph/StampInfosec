// TODO: run actual encrypt and use test vectors to check,
// like https://en.wikipedia.org/wiki/RC4#Test_vectors

var key = Buffer.from([0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x77]),
//var key = new Buffer([0x4b, 0x65, 0x79]),  // "Key"
//var key = new Buffer([0x57, 0x69, 0x6b, 0x69]),  // "Wiki"
    N = key.length,
    S = Buffer.allocUnsafe(256),
    K = Buffer.allocUnsafe(256),
    i = 0,
    j = 0;

initS();

/*
for (var c = 0; c < 10; c++) {
    var byte = getKeystreamByte();
    console.log(byteToHex(byte));
}
*/

console.log('Initial S:');
printS();

console.log('\nDiscarding 1 keystream byte');
discardKeystreamBytes(1);
printIndeces();
console.log('S after the first 1 keystream byte generated:');
printS();

console.log('\nDiscarding 99 keystream bytes');
discardKeystreamBytes(99);
printIndeces();
console.log('S after the first 100 keystream bytes generated:');
printS();

console.log('\nDiscarding 900 keystream bytes');
discardKeystreamBytes(900);
printIndeces();
console.log('S after the first 1000 keystream bytes generated:');
printS();

function printIndeces() {
    console.log('i = ' + i + '  j = ' + j);
}

function discardKeystreamBytes(n) {
    var k;
    for (k = 0; k < n; k++) {
        getKeystreamByte();
    }
}

function initS() {
    var k, m=0;
    for (k=0; k < 256; k++) {
        S[k] = k;
        K[k] = key[k % N];
    }
    for (k=0; k < 256; k++) {
        m = (((m + S[k]) % 256) + K[k]) % 256;
        swapS(k, m);
    }
}

function swapS(a, b) {
    var tmp = S[a];
    S[a] = S[b];
    S[b] = tmp;
}

function printS() {
    var r, c, row, idx, byte;
    for (r=0; r < 16; r++) {
        row = '';
        for (c=0; c < 16; c++) {
            idx = r * 16 + c;
            row += byteToHex(S[idx].toString(16)) + ' ';
        }
        console.log(row);
    }
}

function byteToHex(d, padding) {
    var hex = d.toString(16);
    padding = typeof (padding) === "undefined" || padding === null ? padding = 2 : padding;
    while (hex.length < padding) {
        hex = "0" + hex;
    }
    return hex;
}

function getKeystreamByte() {
    var t;
    // NOTE: using global i, j
     i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    swapS(i, j);
    t = (S[i] + S[j]) % 256
    return S[t];
}