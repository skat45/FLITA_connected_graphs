#include <stdio.h>
#include <stdlib.h>

typedef struct Edge_t {
    int from, to;
    struct Edge_t *next_edge;
} Edge_t;

Edge_t *constructor(Edge_t *head_P) {
    Edge_t *new_edge_P;
    new_edge_P = (Edge_t*) malloc(sizeof(Edge_t) * 1);
    new_edge_P->next_edge = NULL;
    if (head_P != NULL) {
        Edge_t *this_edge_P;
        this_edge_P = head_P;
        while (this_edge_P->next_edge != NULL)
            this_edge_P = this_edge_P->next_edge;
        this_edge_P->next_edge = new_edge_P;
    }
    return new_edge_P;
}

void destructor(Edge_t *edge_P) {
    if (edge_P->next_edge != 0)
        destructor(edge_P->next_edge);
    free(edge_P);
}

Edge_t *init_db(int number) {
    Edge_t *head_P;
    head_P = constructor(NULL);
    for (int i = 0; i < number - 1; ++i) {
        constructor(head_P);
    }
    return head_P;
}

void make_file(Edge_t *head_P, FILE *fp) {
    fprintf(fp, "digraph G {\n");
    Edge_t *this_edge_P;
    this_edge_P = head_P;
    while (1) {
        fprintf(fp, "\t%d -> %d; \n", this_edge_P->from, this_edge_P->to);
        if (this_edge_P->next_edge == NULL)
            break;
        else
            this_edge_P = this_edge_P->next_edge;
    }

    fprintf(fp, "}");

    fclose(fp);
}

void parse_data(FILE *fp, Edge_t *head_P) {
    Edge_t *this_edge_P;
    this_edge_P = head_P;

    short less_zero = 0;
    int vertex_num = 1;

    while (1) {
        char c;
        c = getc(fp);
        if (c == EOF)
            break;

        switch (c) {
            case '-':
                less_zero = 1;
                break;

            case '1':
                if (less_zero) {
                    this_edge_P->to = vertex_num;
                    less_zero = 0;
                }
                else
                    this_edge_P->from = vertex_num;

                this_edge_P = this_edge_P->next_edge;
                break;

            case '0':
                this_edge_P = this_edge_P->next_edge;
                break;

            case '\n':
                this_edge_P = head_P;
                vertex_num++;
        }
    }
}

int main() {
    FILE *fp;
    fp = fopen("../example.txt", "r");
    int d;
    char c;
    int digit_counter = 0;
    int new_line_char_counter = 0;
    while (fscanf(fp,"%d", &d) == 1)
        ++digit_counter;
    fseek(fp, 0, SEEK_SET);
    while (fscanf(fp, "%c", &c) == 1)
        if (c == '\n')
            ++new_line_char_counter;
    fseek(fp, 0, SEEK_SET);
    int vertex_num = new_line_char_counter + 1;
    int edges_num = digit_counter / vertex_num;

    Edge_t *head_P;
    head_P = init_db(edges_num);

    parse_data(fp, head_P);
    fclose(fp);

    fp = fopen("../graph.dot", "w");
    make_file(head_P, fp);
    system("dot -Tpng ../graph.dot -o ../output.png");

    destructor(head_P);

    if (edges_num > (vertex_num - 1) * (vertex_num - 2) / 2)
        printf("The graph is connected");
    else
        printf("The graph is NOT connected");

    return 0;
}
