Resource file packer

----------------------------------------------------------------------------------------------------

File format (version 1)

Byte order is Little Endian

0x0: File format version (1 byte)
0x1 ... 0xN: Resource objects

Resource object:

0x0: Type (2 bytes)
0x2: Data length in bytes (2 bytes)
0x4 ... 0xN: Type specific data

Resource object 'image':

Type = 0
Data =
0x0: Width in pixels (4 bytes)
0x4: Height in pixels (4 bytes)
0x8 ... 0xN: 32bit RGBA pixel 0xRRGGBBAA. Pixel i is x+y*width so first pixel is x=0 y=0, second
    pixel would be x=1 y=0 etc

----------------------------------------------------------------------------------------------------