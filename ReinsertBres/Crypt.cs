using System.IO;
using System.IO.Compression;
using Microsoft.Win32;

namespace KrkrReinsertBres;

public class Crypt {
	static private uint rotl32(uint x, int n) {
		return (x << n) | (x >> (-n & 31));
	}

	static private void quarter_round(uint[] x, int a, int b, int c, int d) {
		x[a] += x[b];
		x[d] = rotl32(x[d] ^ x[a], 16);
		x[c] += x[d];
		x[b] = rotl32(x[b] ^ x[c], 12);
		x[a] += x[b];
		x[d] = rotl32(x[d] ^ x[a], 8);
		x[c] += x[d];
		x[b] = rotl32(x[b] ^ x[c], 7);
	}

	static private void block(uint[] i, uint[] o) {
		uint[] x = (uint[])i.Clone();
		for (int l = 0; l < 4; l++) {
			quarter_round(x, 0, 4, 8, 12);
			quarter_round(x, 1, 5, 9, 13);
			quarter_round(x, 2, 6, 10, 14);
			quarter_round(x, 3, 7, 11, 15);
			quarter_round(x, 0, 5, 10, 15);
			quarter_round(x, 1, 6, 11, 12);
			quarter_round(x, 2, 7, 8, 13);
			quarter_round(x, 3, 4, 9, 14);
		}
		for (int l = 0; l < 16; l++) {
			o[l] = x[l] + i[l];
		}
	}

	static unsafe public byte[] Chacha20(byte[] b, uint[] s) {
		byte[] buf = (byte[])b.Clone();
		uint[] ks = new uint[16];
		uint fc = s[12];
		for (int l = 0; l < buf.Length; l++) {
			if ((l & 63) == 0) {
				s[12] = (uint)(l >> 6) ^ fc;
				block(s, ks);
			}
			fixed (uint* ksp = ks) {
				buf[l] ^= ((byte*)ksp)[l & 63];
			}
		}
		return buf;
	}

	static public byte[] ZlibDec(byte[] b) {
		MemoryStream compressed = new MemoryStream(b);
		MemoryStream uncompressed = new MemoryStream();
		compressed.Seek(8, SeekOrigin.Begin);
		ZLibStream stream = new ZLibStream(compressed, CompressionMode.Decompress);
		stream.CopyTo(uncompressed);
		return uncompressed.ToArray();
	}

	static public byte[] ZlibEnc(byte[] b) {
		MemoryStream compressed = new MemoryStream();
		ZLibStream stream = new ZLibStream(compressed, CompressionMode.Compress);
		stream.Write(b, 0, b.Length);
		MemoryStream result = new MemoryStream();
		BinaryWriter bw = new BinaryWriter(result);
		bw.Write((uint)compressed.Length);
		bw.Write((uint)b.Length);
		compressed.Position = 0;
		compressed.CopyTo(result);
		return result.ToArray();
	}
}