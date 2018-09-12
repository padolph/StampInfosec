// TODO: run actual encrypt and use test vectors to check

var key = new Buffer([0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x77]),
    N = key.length,
    S = new Buffer(256),
    K = new Buffer(256),
    i = 0,
    j = 0;

initS();
console.log('Initial S:');
printS();

console.log('\nDiscarding 100 keystream bytes');
discardKeystreamBytes(100);
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
    var i;
    for (i = 0; i < 100; i++) {
        getKeystreamByte();
    }
}

function initS() {
    var i, j=0;
    for (i=0; i < 256; i++) {
        S[i] = i;
        K[i] = key[i % N];
    }
    for (i=0; i < 256; i++) {
        j = (j + S[i] + K[i]) % 256;
        swapS(i, j);
    }
}

function swapS(i, j) {
    var tmp = S[i];
    S[i] = S[j];
    S[j] = tmp;
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