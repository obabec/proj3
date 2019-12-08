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

#define CHAR_CONV 48
#define MIDDLE_DIR 6
#define RIGHT_DIR 0
#define LEFT_DIR 3

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;
int find_type = 0;

int get_cell_val(Map* map, int r, int c) {
        return (map->cells[(r - 1) * map->cols + (c - 1)]) - CHAR_CONV;
}

int validate_file(char* filename) {
    return 0;
}

void print_mem_err() {
    fprintf(stderr, "Memory error!");
}

void free_map(Map* map) {
    free(map->cells);
}

int initiate_map(Map* map) {
    map->cells = malloc(map->rows * map->cols * sizeof(char));
    if(map->cells == NULL) {
        print_mem_err();
        free_map(map);
        return 1;
    }
    return 0;
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
    if(initiate_map(map)) return 1;
    
    for (int y = 0; y < map->cols * map->rows; y++) {
        fscanf(labyrint, "%s", buff);
        map->cells[y] = (unsigned char) buff[0];
        printf("%c ", map->cells[y]);
        if((y + 1 )% map->cols == 0) {
            printf("\n");
        }
    }
    fclose(labyrint);
    printf("\n");
    printf("\n");
    return 0;

}


bool isborder(Map *map, int r, int c, int border) {

    int cell_val = get_cell_val(map, r, c);
    if (border == 4 && cell_val >= 4) {
        return true;
    }

    if (border == 1 && (cell_val == 1 || cell_val % 2 != 0)) {
        return true;
    }

    if (border == 2 && (cell_val == 2 || cell_val == 3 || cell_val == 6 || cell_val == 7)) {
        return true;
    }
    return false;
}

int start_border(Map *map, int r, int c) {
    if ((r == 1 || r == map->rows) && !isborder(map, r, c, 4)) {
        return MIDDLE_DIR;
    } else if (c == 1 && !isborder(map, r, c, 1)) {
        return RIGHT_DIR;
    } else if (c == map->cols && !isborder(map, r, c, 2)) {
        return LEFT_DIR;
    }
    return 0;
}

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

int get_tri_type(int r, int c) {
    return !((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 != 0));
}

void find_next_pos(Map* map, int directions[2][9], int *direction, int next_pos[2], int r, int c) {
    int x = *direction + 3;
    int tri_type = get_tri_type(r, c);
    for (int i = *direction; i < x; i++) {

        if (!isborder(map, r, c, directions[tri_type][i])) {
            if (directions[tri_type][i] == 1) {
                *direction = LEFT_DIR;
                next_pos[0] = r;
                next_pos[1] = c - 1;
                break;
            } 
            if (directions[tri_type][i] == 2) {
                *direction = RIGHT_DIR;
                next_pos[0] = r;
                next_pos[1] = c + 1;
                break;
            } 
            if (directions[tri_type][i] == 4) {
                *direction = MIDDLE_DIR;
                r = (tri_type ? r + 1 : r - 1);
                next_pos[0] = r;
                next_pos[1] = c;
                break;
            } 
        }

    }
}

int find_way(Map* map, char* filename, int r, int c) {

        int left_hand_dir[2][9] = {{4, 2, 1, 1, 4, 2, 2, 1, 4}, {2, 4, 1, 4, 1, 2, 1, 2, 4}};
        int right_hand_dir[2][9] = {{2, 4, 1, 4, 1, 2, 1, 2, 4}, {4, 2, 1, 1, 4, 2, 2, 1, 4}};

        load_map(map, filename);
        int direction = start_border(map, r, c);
        int last_pos[2] = {r, c};
        int next_pos[2] = {0, 0};
        while(!(last_pos[0] > map->rows || last_pos[0] < 1 || last_pos[1] > map->cols || last_pos[1] < 1)) {
            
            if (find_type == 1) {
                find_next_pos(map, left_hand_dir, &direction, next_pos, last_pos[0], last_pos[1]);
            } else {
                find_next_pos(map, right_hand_dir, &direction, next_pos, last_pos[0], last_pos[1]);
            }

            printf("%c,%c\n", last_pos[0] + CHAR_CONV, last_pos[1] + CHAR_CONV);
            last_pos[0] = next_pos[0];
            last_pos[1] = next_pos[1];
        }
        free_map(map);
        return 0;
}


int parse_args(Map* map, int argc, char* argv[]) {
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
        if (!strcmp(argv[1], "--rpath")) find_type = 2;
        else if (!strcmp(argv[1], "--lpath")) find_type = 1;
        else if (!strcmp(argv[1], "--shortest")) find_type = 3;
        else {
            print_arg_err();
            return 1;
        }
        return find_way(map, argv[4], atoi(argv[2]), atoi(argv[3]));
    } else {
        print_arg_err();
        return 1;
    }

}

int main(int argc, char* argv[]) {
    Map map;
    return parse_args(&map, argc, argv);
}