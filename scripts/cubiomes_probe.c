#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "usage: cubiomes_probe SEED X Y Z\n");
        return 2;
    }

    int64_t signed_seed = strtoll(argv[1], NULL, 10);
    uint64_t seed = (uint64_t)signed_seed;
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    int z = atoi(argv[4]);

    Generator g;
    setupGenerator(&g, MC_26_2, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    int biome = getBiomeAt(&g, 1, x, y, z);
    if (biome < 0) {
        fprintf(stderr, "biome generation failed\n");
        return 3;
    }

    printf("{\"engine\":\"cubiomes\",\"minecraft_version\":\"26.2\",\"x\":%d,\"y\":%d,\"z\":%d,\"biome_id\":%d}\n",
           x, y, z, biome);
    return 0;
}
