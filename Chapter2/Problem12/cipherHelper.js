var fs = require('fs'),
    prompt = require('prompt');

var ciphertextFilename = process.argv[2];
var ciphertext = fs.readFileSync(ciphertextFilename).toString();
var freqTab = getFrequencyTable(ciphertext);

console.log('Ciphertext letter frequencies');
printSortedFreqTab(freqTab);

prompt.start();
prompt.message = '';
prompt.delimiter = '';

guess();

function guess() {
    var key;
    console.log('    abcdefghijklmnopqrstuvwxyz');
    prompt.get(['key'], function(err, result) {
        key = result.key;
        var decodeTab = makeDecodingTable(key);
        var cleartext = decode(ciphertext, decodeTab);
        console.log('----------------------------------------');
        output(ciphertext, cleartext);
        guess();
    });
}

function output(ciphertext, cleartext) {
    var lineLen = 80;
    var pos = 0;
    while (pos < ciphertext.length) {
        console.log(ciphertext.substring(pos, pos+lineLen));
        console.log(cleartext.substring(pos, pos+lineLen));
        pos += lineLen;
    }
}

function decode(ct, dt) {
    var result = '',
        i,
        decodedChar;
    for (i=0; i<ct.length; i++) {
        var cipherchar = ct.charAt(i);
        var clearchar = dt[cipherchar];
        if (!clearchar) {
            clearchar = '?';
        }
        result += clearchar;
    }
    return result;
}

function makeDecodingTable(key) {
    var i,
        decodeTab = {},
        alphabet = 'abcdefghijklmnopqrstuvwxyz';
    for (i=0; i < key.length; i++) {
        var keychar = key.charAt(i);
        if (keychar != '?') {
            decodeTab[key.charAt(i)] = alphabet.charAt(i);
        }
    }
    return decodeTab;
}

function getFrequencyTable(text) {
    var freqTab = {},
        i;
    for (i=0; i < text.length - 1; i++) {
        var character = text.charAt(i);
        if (freqTab[character]) {
            freqTab[character]++;
        } else {
            freqTab[character] = 1;
        }
    }
    return freqTab;
}

function printSortedFreqTab(tab) {
    var sortable = [],
        letter,
        i;
    for (letter in tab) {
        sortable.push([letter, tab[letter]])
    }
    sortable.sort(function(a,b) {
        return b[1] - a[1];
    })  
    for (i=0; i < sortable.length; i++) {
        console.log(sortable[i][0] + ": " + sortable[i][1]);
    }  
}


