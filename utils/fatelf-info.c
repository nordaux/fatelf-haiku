/**
 * FatELF; support multiple ELF binaries in one file.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon.
 */

#define FATELF_UTILS 1
#include "fatelf-utils.h"

static int fatelf_info(const char *fname)
{
    const int fd = xopen(fname, O_RDONLY, 0755);
    FATELF_header *header = xread_fatelf_header(fname, fd);
    unsigned int i = 0;

    printf("%s: FatELF format version %d\n", fname, (int) header->version);
    printf("%d binaries.\n", (int) header->num_binaries);

    for (i = 0; i < header->num_binaries; i++)
    {
        const FATELF_binary_info *bin = &header->binaries[i];
        const fatelf_machine_info *machine = get_machine_by_id(bin->machine);
        const fatelf_osabi_info *osabi = get_osabi_by_id(bin->osabi);

        printf("Binary at index #%d:\n", i);
        printf("  OSABI %u (%s%s%s) version %u,\n",
                (unsigned int) bin->osabi, osabi ? osabi->name : "???",
                osabi ? ": " : "", osabi ? osabi->desc : "",
                (unsigned int) bin->osabi_version);
        printf("  Machine %u (%s%s%s)\n",
                (unsigned int) bin->machine, machine ? machine->name : "???",
                machine ? ": " : "", machine ? machine->desc : "");
        printf("  Offset %llu\n", (unsigned long long) bin->offset);
        printf("  Size %llu\n", (unsigned long long) bin->size);
    } // for

    xclose(fname, fd);
    free(header);

    return 0;  // success.
} // fatelf_info


int main(int argc, const char **argv)
{
    xfatelf_init(argc, argv);
    if (argc != 2)  // this could stand to use getopt(), later.
        xfail("USAGE: %s <fname>", argv[0]);
    return fatelf_info(argv[1]);
} // main

// end of fatelf-info.c ...

