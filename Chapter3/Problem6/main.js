for (i=0; i < 2; i++) {
	for (j=0; j < 2; j++) {
		for (k=0; k < 2; k++) {
			console.log(i + ' ' + j + ' ' + k + ' :' + maj(i,j,k));
		}
	}
}

// Karnaugh map reduction of maj truth table
function maj(a, b, c) {
	return (b && c) || (a && c) | (a && b);
}
