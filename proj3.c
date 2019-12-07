/**************************************************************
*                                                             *
*            IZP projekt 3 - bludiste                         *
*                   Babec Ondrej                              *
*                       2019                                  *
*                                                             *
***************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

int validate_file(char* filename) {
    return 0;
}

void print_mem_err() {
    fprintf(stderr, "Memory error!");
}

int initiate_map(Map* map) {
    map->cells = (unsigned char *) malloc(map->rows * map->cols * sizeof(char));
    /*if(map->cells == NULL) {
        print_mem_err();
        free_map(map);
        //return 1;
        exit(1);
    }*/
}

int free_map(Map* map) {
    free(map->cells);
}

int load_map(Map* map, char* filename) {
    char buff[2];
    validate_file(filename);
    FILE *labyrint;
    labyrint = fopen(filename, "r");
    fscanf(labyrint, "%s", buff);
    map->rows = atoi(buff);
    printf("%d", map->rows);
    fscanf(labyrint, "%s", buff);
    map->cols = atoi(buff);
    printf(" %d\n", map->cols);
    initiate_map(map);
    for (int i = 0; i < map->rows; i++) {
        for (int y = 0; y < map->cols; y++) {
            fscanf(labyrint, "%s", buff);
            *(&map->cells + i * map->cols + y) = (unsigned char) buff[0];
            printf("%c ", *(&map->cells + i * map->cols + y));
        }
        printf("\n");
    }
}

int find_type = 0;

bool isborder(Map *map, int r, int c, int border);

int start_border(Map *map, int r, int c, int leftright);

void print_help() {
    printf("Help for proj3\n\n");
    printf("Args:\n");
    printf("--help \t Printf help.\n");
    printf("--test \t Validates input file\n");
    printf("--rpath \t Finds way from labyrint with right hand rule.\n");
    printf("--lpath \t Finds way from labyrint with left hand rule.\n");
    printf("--shortest \t Find shortest way from labyrint.\n\n");

    printf("Input:\n");
    printf("labyrint.txt \t Text file with described labyrint map. This input is required!\n");
    printf("R \t Number of row where is located entrance to labyrint.\n \t Required for program to find way from labyrint.\n");
    printf("C \t Number of column where is located entrance to labyrint.\n \t Required for program to find way from labyrint.\n");
}

void print_arg_err() {
    fprintf(stderr, "Unexpected arguments!");
}

int parse_args(int argc, char* argv[]) {
    if (!strcmp(argv[1], "--help")) {
        print_help();
        return 0;
    } else if (!strcmp(argv[1], "--test")) {
        if (argc != 3) {
            print_arg_err();
            return 1;
        }
        return validate_file(argv[2]);
    } else if (argc == 5) {
        if (!strcmp(argv[1], "--rpath")) find_type = 1;
        else if (!strcmp(argv[1], "--lpath")) find_type = 2;
        else if (!strcmp(argv[1], "--shortest")) find_type = 3;
        else {
            print_arg_err();
            return 1;
        }
        // Nutno pridat volani hledani cesty!!!
    } else {
        print_arg_err();
        return 1;
    }

}

int main(int argc, char* argv[]) {
    Map map;
    load_map(&map, "bludiste.txt");
    printf("%s", map.cells + 1);
    //free_map(&map);
    return 0;
    //return parse_args(argc, argv);
}