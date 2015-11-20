#include <stdio.h>
#include <string.h>

unsigned char rcon[256] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

unsigned char sbox[256] = {
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

void __printblock(unsigned char *block) {
	for(int i = 0; i < 16; i++) {
		if(i%8 == 0) { printf("%s", "\n"); }
		else if(i%4 == 0) { printf("%s", "\t"); }
		printf("%x\t", block[i]);
	}
	printf("%c", '\n');
}

void addroundkey(unsigned char *state, unsigned char *roundkey) {
	for(int i = 0; i < 4; i++) {
		state[i*4] = state[i*4] ^ roundkey[i*4];
		state[i*4 + 1] = state[i*4 + 1] ^ roundkey[i*4 + 1];
		state[i*4 + 2] = state[i*4 + 2] ^ roundkey[i*4 + 2];
		state[i*4 + 3] = state[i*4 + 3] ^ roundkey[i*4 + 3];
	}
}

void getroundkey(unsigned char *full, unsigned char *piece, int which) {
	//destination, source, length
	memcpy(piece, full + which*16, 16);
}

void rotate(unsigned char *state0, unsigned char *state1, unsigned char *state2, unsigned char *state3) {
	unsigned char temp = *state0;
	*state0 = *state1;
	*state1 = *state2;
	*state2 = *state3;
	*state3 = temp;
}

void subbytes(unsigned char *state0, unsigned char *state1, unsigned char *state2, unsigned char *state3) {
	*state0 = sbox[*state0];
	*state1 = sbox[*state1];
	*state2 = sbox[*state2];
	*state3 = sbox[*state3];
}

void mixcolumns(unsigned char *state0, unsigned char *state1, unsigned char *state2, unsigned char *state3) {

}

void expandkey(unsigned char *key, unsigned char *result) {
	//copy the first 16 bytes
	for(int i = 0; i < 16; i++) {
		result[i] = key[i];
	}

	//work on a 16 bytes at a time
	for(int i = 1; i < 11; i++) {
		//calc first four bytes
		result[i*16 + 0] = result[i*16 - 4 + 0];
		result[i*16 + 1] = result[i*16 - 4 + 1];
		result[i*16 + 2] = result[i*16 - 4 + 2];
		result[i*16 + 3] = result[i*16 - 4 + 3];
		//printf("copy: \t%x\t%x\t%x\t%x\t\n", result[i*16+0], result[i*16+1], result[i*16+2], result[i*16+3]);
		rotate(&result[i*16 + 0], &result[i*16 + 1], &result[i*16 + 2], &result[i*16 + 3]);
		//printf("rot: \t%x\t%x\t%x\t%x\t\n", result[i*16+0], result[i*16+1], result[i*16+2], result[i*16+3]);
		subbytes(&result[i*16 + 0], &result[i*16 + 1], &result[i*16 + 2], &result[i*16 + 3]);
		//printf("sub: \t%x\t%x\t%x\t%x\t\n", result[i*16+0], result[i*16+1], result[i*16+2], result[i*16+3]);
		//printf("xor: \t%x\t%x\t%x\t%x\n", result[i*16 + 0], result[i*16 - 16 + 0], rcon[i], result[i*16 + 0] ^ result[i*16 - 16 + 0] ^ rcon[i]);
		//xor this + first 4 bytes of last block
		result[i*16 + 0] ^= result[i*16 - 16 + 0] ^ rcon[i];
		result[i*16 + 1] ^= result[i*16 - 16 + 1];
		result[i*16 + 2] ^= result[i*16 - 16 + 2];
		result[i*16 + 3] ^= result[i*16 - 16 + 3];

		//calc next 12 bytes
		//xor this + last 12 bytes of last block
		for(int j = 0; j < 12; j++) {
			//printf("xor: \t%x\t%x\t%x\n", result[i*16 + j], result[i*16 - 12 + j], result[i*16 + j] ^ result[i*16 - 12 + j]);
			result[i*16 + 4 + j] = result[i*16 + j] ^ result[i*16 - 12 + j];
		}

	}

	//for(int i = 0; i < 176; i++) {
	//	if(i%8 == 0) { printf("%c", '\n'); }
	//	else if(i%4 == 0) { printf("%s", "\t"); }
	//	printf("%x\t", result[i]);
	//}
	//printf("%c", '\n');
}

void encryptablock(unsigned char *state, unsigned char *key) {
	//__printblock(state);
	//__printblock(key);
	unsigned char expandedkey[176] = {0};
	unsigned char roundkey[16] = {0};
	expandkey(key, expandedkey);

	getroundkey(expandedkey, roundkey, 0);
	__printblock(roundkey);

	for(int i = 1; i < 10; i++) {
		subbytes(&state[0], &state[1], &state[2], &state[3]);
		subbytes(&state[4], &state[5], &state[6], &state[7]);
		subbytes(&state[8], &state[9], &state[10], &state[11]);
		subbytes(&state[12], &state[13], &state[14], &state[15]);

		rotate(&state[0], &state[1], &state[2], &state[3]);
		rotate(&state[4], &state[5], &state[6], &state[7]);
		rotate(&state[8], &state[9], &state[10], &state[11]);
		rotate(&state[12], &state[13], &state[14], &state[15]);

		//mixcolumns

		getroundkey(expandedkey, roundkey, i);
		addroundkey(state, roundkey);
	}

	subbytes(&state[0], &state[1], &state[2], &state[3]);
	subbytes(&state[4], &state[5], &state[6], &state[7]);
	subbytes(&state[8], &state[9], &state[10], &state[11]);
	subbytes(&state[12], &state[13], &state[14], &state[15]);

	rotate(&state[0], &state[1], &state[2], &state[3]);
	rotate(&state[4], &state[5], &state[6], &state[7]);
	rotate(&state[8], &state[9], &state[10], &state[11]);
	rotate(&state[12], &state[13], &state[14], &state[15]);

	getroundkey(expandedkey, roundkey, 11);
	addroundkey(state, roundkey);
}

int main() {
	printf("CSE 178 AES Implementation\n\n");

	unsigned char state[16] = {'\x04','\xe0','\x48','\x28','\x66','\xcb','\xf8','\x06','\x81','\x19','\xd3','\x26','\xe5','\x9a','\x7a','\x4c'};
	//unsigned char key[] = "abcdefhijklmnop";
	//unsigned char key[16] = {'\xa0','\x88','\x23','\x2a','\xfa','\x54','\xa3','\x6c','\xfe','\x2c','\x39','\x76','\x17','\xb1','\x39','\x05'};
	unsigned char key[16] = {'\x2b','\x7e','\x15','\x16','\x28','\xae','\xd2','\xa6','\xab','\xf7','\x15','\x88','\x09','\xcf','\x4f','\x3c'};

	encryptablock(state, key);

	//unsigned char word[4] = {0x8a, 0x84, 0xeb, 0x01};
	//for(int i = 0; i < 4; i++) {
	//	word[i] = word[i] ^ rcon[i+1];
	//}
	//rotate(&word[0], &word[1], &word[2], &word[3]);
	//__printblock(word);

	printf("%c", '\n');
	return 0;
}

/*

http://www.formaestudio.com/rijndaelinspector/archivos/Rijndael_Animation_v4_eng.swf

http://www.angelfire.com/biz7/atleast/mix_columns.pdf
https://en.wikipedia.org/wiki/Rijndael_mix_columns
http://www.samiam.org/mix-column.html

*/