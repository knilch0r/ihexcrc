#include "crc32.h"
#include <stdio.h>

typedef unsigned char ui8;

static inline unsigned init_crc32(void)
{
    return ~0U;
}

static inline unsigned update_crc32(ui8 data, unsigned crc)
{
    return crc32_tab[(crc ^ data) & 0xFF] ^ (crc >> 8);
}

static inline unsigned final_crc32(unsigned crc)
{
    return crc ^ ~0U;
}

static unsigned hex2dec(ui8 h)
{
    if (('0' <= h) && (h <= '9')) return h - '0';
    if (('a' <= h) && (h <= 'f')) return h - 'a' + 10;
    if (('A' <= h) && (h <= 'F')) return h - 'A' + 10;
    return ~0U;
}

static int parsebyte(ui8 * dest, ui8 a, ui8 b)
{
    unsigned temp = (hex2dec(a) << 4) | hex2dec(b);
    *dest = temp & 0xFFU;
    return ((temp & (~0xFFU)) == 0);
}

#define BUFSZ 1024

int main(int argc, char**argv)
{
    unsigned crc;
    ui8 linebuf[BUFSZ] = {0};
    unsigned lineno = 0;

    if (argc > 1)
    {
        (void) argv; /* we don't actually care what arguments we're given */
        puts("calculate CRC32 over all data records of an intel hex file\n");
        puts("usage:\n\tihexcrc < hexfile > crcfile");
        return 1;
    }

    /* parse intel hex records:
     * ":llaaaarrdd...cc"
     * ':' RECORD MARK, 'll' RECLEN, 'aaaa' LOAD OFFSET, 'rr' RECTYP, 'dd...' DATA, 'cc' CHKSUM
     * we only use data records (rr == "00") and use them in the order they appear (ie we ignore
     * embedded address information). we do check for a valid checksum, however. */
    crc = init_crc32();
    while (fgets((char*)linebuf, BUFSZ, stdin))
    {
        ui8 reclen;
        ui8 temp, temp2;
        ui8 rectype;
        ui8 chksum;
        ui8 reccs;
        int i;

        lineno++;
        if ((linebuf[0] == '\n') || (linebuf[0] == '\r')) continue;
        if (linebuf[BUFSZ-1] != 0)
        {
            fprintf(stderr, "line %u: overlong line (> %d bytes), aborting\n", lineno, BUFSZ);
            return 2;
        }
        if (linebuf[0] != ':')
        {
            fprintf(stderr, "line %u ignored: unsupported format\n", lineno);
            continue;
        }
        if (!parsebyte(&reclen, linebuf[1], linebuf[2]))
        {
            fprintf(stderr, "line %u ignored: could not parse RECLEN\n", lineno);
            continue;
        }
        if (   (!parsebyte(&temp, linebuf[3], linebuf[4]))
            || (!parsebyte(&temp2, linebuf[5], linebuf[6]))
           )
        {
            fprintf(stderr, "line %u ignored: could not parse LOAD OFFSET\n", lineno);
            continue;
        }
        if (!parsebyte(&rectype, linebuf[7], linebuf[8]))
        {
            fprintf(stderr, "line %u ignored: could not parse RECTYP\n", lineno);
            continue;
        }
        if (rectype != 0)
        {
            fprintf(stderr, "line %u ignored: irrelevant record type %02x\n", lineno, rectype);
            continue;
        }
        if (reclen == 0)
        {
            fprintf(stderr, "line %u ignored: zero lenght data record\n", lineno);
            continue;
        }
        /* if we ended up here, we've got a data record. we now read <reclen> data bytes,
         * add them to our CRC, and for good measure also check the record checksum */
        reccs = reclen + temp + temp2 + rectype;
        for (i = 0; i < reclen; i++)
        {
            ui8 data;
            if (!parsebyte(&data, linebuf[9+i*2], linebuf[10+i*2]))
            {
                fprintf(stderr, "line %u: could not parse data item byte %d, aborting %x\n", lineno, i, data);
                return 3;
            }
            crc = update_crc32(data, crc);
            reccs += data;
        }
        if (!parsebyte(&chksum, linebuf[9+i*2], linebuf[10+i*2]))
        {
            fprintf(stderr, "line %u ignored: warning, could not parse chksum\n", lineno);
            continue;
        }
        reccs += chksum;
        if (reccs != 0)
        {
            fprintf(stderr, "line %u: warning, incorrect chksum (%d != 0)\n", lineno, reccs);
            continue;
        }
    }
    crc = final_crc32(crc);

    printf("0x%08X", crc);
    return 0;
}
