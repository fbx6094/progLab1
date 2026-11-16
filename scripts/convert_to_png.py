import sys
import os
import zlib
import struct

def parse_pnm(filepath):
    with open(filepath, 'r', encoding='latin-1') as f:
        content = f.read()
    
    tokens = []
    lines = content.splitlines()
    for line in lines:
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        tokens.extend(line.split())
    
    if not tokens:
        raise ValueError(f"Empty or invalid file: {filepath}")
    
    magic = tokens[0]
    width = int(tokens[1])
    height = int(tokens[2])
    maxval = int(tokens[3])
    data = [int(t) for t in tokens[4:]]
    
    return magic, width, height, maxval, data

def write_png(filename, width, height, rgb_rows):
    def png_chunk(chunk_type, data):
        c = chunk_type + data
        crc = zlib.crc32(c) & 0xffffffff
        return struct.pack('>I', len(data)) + c + struct.pack('>I', crc)

    raw_data = bytearray()
    for row in rgb_rows:
        raw_data.append(0)
        raw_data.extend(row)
    
    compressed = zlib.compress(bytes(raw_data), 9)
    
    png = bytearray(b'\x89PNG\r\n\x1a\n')
    ihdr = struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0)
    png.extend(png_chunk(b'IHDR', ihdr))
    png.extend(png_chunk(b'IDAT', compressed))
    png.extend(png_chunk(b'IEND', b''))
    
    with open(filename, 'wb') as f:
        f.write(png)

def convert_file(filepath):
    magic, w, h, maxval, data = parse_pnm(filepath)
    scale = 255.0 / maxval
    rgb_rows = []
    
    if magic == 'P2':
        idx = 0
        for y in range(h):
            row = bytearray()
            for x in range(w):
                val = int(data[idx] * scale)
                idx += 1
                row.extend([val, val, val])
            rgb_rows.append(row)
    elif magic == 'P3':
        idx = 0
        for y in range(h):
            row = bytearray()
            for x in range(w):
                r = int(data[idx] * scale)
                g = int(data[idx + 1] * scale)
                b = int(data[idx + 2] * scale)
                idx += 3
                row.extend([r, g, b])
            rgb_rows.append(row)
    else:
        print(f"Skipping unsupported format: {magic}")
        return

    out_png = os.path.splitext(filepath)[0] + '.png'
    write_png(out_png, w, h, rgb_rows)
    print(f"Converted: {filepath} -> {out_png}")

if __name__ == '__main__':
    files = sys.argv[1:]
    if not files:
        target_dir = 'output'
        if os.path.exists(target_dir):
            files = [os.path.join(target_dir, f) for f in os.listdir(target_dir) if f.endswith(('.ppm', '.pgm'))]
    for fp in files:
        try:
            convert_file(fp)
        except Exception as e:
            print(f"Error converting {fp}: {e}")
