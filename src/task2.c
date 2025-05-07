// Realizat de Theodor Vulpe - 315CB

#include "task2.h"
#include "task1.h"

#define MAX_QUERY_LENGTH 256
#define MAX_FILE_NAME 256

#define DIMENSIUNE_OPERATOR 3
#define NR_MAX_CONDITII 2

#define MAX_VALORI_SETARE 3

typedef struct {
    char camp[MAX_QUERY_LENGTH];
    char op[DIMENSIUNE_OPERATOR];
    char val[MAX_QUERY_LENGTH];
} conditie;

typedef struct {
    char camp[MAX_QUERY_LENGTH];
    char val[MAX_VALORI_SETARE][MAX_QUERY_LENGTH];
} setare;

int este_litera_mare(char a) {
    return (a >= 'A' && a <= 'Z');
}

int verifica_camp_sir(conditie conditie, char valoare_camp[]) {
    // Daca coincide campul
    return strcmp(valoare_camp, conditie.val);
}

int verifica_camp_caracter(conditie conditie, char valoare_camp) {
    // Daca coincide campul
    return (valoare_camp == conditie.val[0]);
}

int verifica_camp_float(conditie conditie, float valoare_camp) {
    float val1 = valoare_camp, val2 = (float)atof(conditie.val);
    // Verific operatorul
    if (!strcmp(conditie.op, "=")) {
        return (val1 == val2);
    } else if (!strcmp(conditie.op, "!=")) {
        return (val1 != val2);
    } else if (!strcmp(conditie.op, "<")) {
        return (val1 < val2);
    } else if (!strcmp(conditie.op, ">")) {
        return (val1 > val2);
    } else if (!strcmp(conditie.op, "<=")) {
        return (val1 <= val2);
    } else if (!strcmp(conditie.op, ">=")) {
        return (val1 >= val2);
    }
    return 0;
}

int verifica_camp_intreg(conditie conditie, int valoare_camp) {
    int val1 = valoare_camp, val2 = atoi(conditie.val);
    // Verific operatorul
    if (!strcmp(conditie.op, "=")) {
        return (val1 == val2);
    } else if (!strcmp(conditie.op, "!=")) {
        return (val1 != val2);
    } else if (!strcmp(conditie.op, "<")) {
        return (val1 < val2);
    } else if (!strcmp(conditie.op, ">")) {
        return (val1 > val2);
    } else if (!strcmp(conditie.op, "<=")) {
        return (val1 <= val2);
    } else if (!strcmp(conditie.op, ">=")) {
        return (val1 >= val2);
    }
    return 0;
}

// Verifica daca se respecta condtiile de dupa WHERE
int respecta_conditiile(conditie conditii[], int nr_conditii, char tabel[],
    secretariat *s, int j) {
        // Daca nu sunt conditii, atunci returneaza 1
        if (nr_conditii == 0) {
            return 1;
        }

        for (int c = 0; c < nr_conditii; c++) {
            if (strcmp(tabel, "studenti") == 0) {
                if (!strcmp(conditii[c].camp, "id")) {
                    if (!verifica_camp_intreg(conditii[c], s->studenti[j].id)) {
                            return 0;
                    }
                }
                if (!strcmp(conditii[c].camp, "nume")) {
                    if (!verifica_camp_sir(conditii[c],
                        s->studenti[j].nume)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "an_studiu")) {
                    if (!verifica_camp_intreg(conditii[c],
                        s->studenti[j].an_studiu)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "statut")) {
                    if (!verifica_camp_caracter(conditii[c],
                        s->studenti[j].statut)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "medie_generala")) {
                    if (!verifica_camp_float(conditii[c],
                        s->studenti[j].medie_generala)) {
                            return 0;
                        }
                }
            } else if (strcmp(tabel, "materii") == 0) {
                if (!strcmp(conditii[c].camp, "id")) {
                    if (!verifica_camp_intreg(conditii[c],
                        s->materii[j].id)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "nume")) {
                    if (!verifica_camp_sir(conditii[c],
                        s->materii[j].nume)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "nume_titular")) {
                    if (!verifica_camp_sir(conditii[c],
                        s->materii[j].nume_titular)) {
                            return 0;
                        }
                }
            } else if (strcmp(tabel, "inrolari") == 0) {
                if (!strcmp(conditii[c].camp, "id_student")) {
                    if (!verifica_camp_intreg(conditii[c],
                        s->inrolari[j].id_student)) {
                            return 0;
                        }
                }
                if (!strcmp(conditii[c].camp, "id_materie")) {
                    if (!verifica_camp_intreg(conditii[c],
                        s->inrolari[j].id_materie)) {
                            return 0;
                        }
                }
            }
        }

        return 1;
}

// Executa operatia de stergere dintr-un secretariat
void sterge_intrare(secretariat *s, char tabel[], int index) {
    if (strcmp(tabel, "studenti") == 0) {
        for (int i = index; i < s->nr_studenti - 1; i++) {
            s->studenti[i] = s->studenti[i + 1];
        }
        s->nr_studenti--;
    } else if (strcmp(tabel, "materii") == 0) {
        for (int i = index; i < s->nr_materii - 1; i++) {
            s->materii[i] = s->materii[i + 1];
        }
        s->nr_materii--;
    } else if (strcmp(tabel, "inrolari") == 0) {
        for (int i = index; i < s->nr_inrolari - 1; i++) {
            s->inrolari[i] = s->inrolari[i + 1];
        }
        s->nr_inrolari--;
    }
}

int main(int argc, char *argv[]) {
    // TODO(student): Task 2
    // Obtin database-ul si il citesc intr-un struct de secretariat
    secretariat* s = citeste_secretariat(argv[1]);

    int N = 0;
    // Citeste numarul de query-uri
    scanf("%d", &N);
    // Golesc buffer-ul
    while ((getchar()) != '\n') {}

    for (int r = 0; r < N; r++) {
        char query[MAX_QUERY_LENGTH];
        fgets(query, MAX_QUERY_LENGTH, stdin);

        // Retine fiecare cuvant din query
        char cuv[MAX_QUERY_LENGTH][MAX_QUERY_LENGTH];
        char *p = NULL;
        int k = 0;

        p = strtok(query, "\";, ");
        while (p) {
            if (strcmp(p, "\n") == 0) {
                break;
            }
            snprintf(cuv[k++], MAX_QUERY_LENGTH, "%s", p);
            p = strtok(NULL, "\";, ");
        }

        // Separarea instructiunilor
        char instructiune[MAX_QUERY_LENGTH];
        snprintf(instructiune, MAX_QUERY_LENGTH, "%s", cuv[0]);

        char campuri[MAX_QUERY_LENGTH][MAX_QUERY_LENGTH];
        int nr_campuri = 0, i = 0;

        char tabel[MAX_QUERY_LENGTH];

        conditie conditii[NR_MAX_CONDITII];
        int nr_conditii = 0;

        setare setare;

        if (!strcmp(instructiune, "UPDATE")) {
            i++;  // tabel
            snprintf(tabel, MAX_QUERY_LENGTH, "%s", cuv[i]);
            i += 2;  // camp
            snprintf(setare.camp, MAX_QUERY_LENGTH, "%s", cuv[i]);
            i++;  // =
            i++;  // valoare
            int nr_valori = 0;
            while (i < k) {
                if (este_litera_mare(cuv[i][1]))
                    break;
                snprintf(setare.val[nr_valori++], MAX_QUERY_LENGTH, "%s", cuv[i]);
                i++;
            }
            i--;
        } else {
            for (i = 1; i < k; i++) {
                if (este_litera_mare(cuv[i][0])) {
                    break;
                }
                snprintf(campuri[nr_campuri++], MAX_QUERY_LENGTH, "%s", cuv[i]);
            }
            i++;

            snprintf(tabel, MAX_QUERY_LENGTH, "%s", cuv[i]);

            // Am obtinut instructiune; campuri; tabel
        }

        i++;
        if (i < k) {
            // Plasez indicele pe "WHERE"
            snprintf(conditii[nr_conditii].camp, MAX_QUERY_LENGTH, "%s", cuv[i + 1]);
            snprintf(conditii[nr_conditii].op, MAX_QUERY_LENGTH, "%s", cuv[i + 2]);
            snprintf(conditii[nr_conditii].val, MAX_QUERY_LENGTH, "%s", cuv[i + 3]);
            nr_conditii++;
        }

        // Verific daca e filtrare complexa
        i += 4;
        if (i < k) {
            // Plasez indicele pe "AND"
            snprintf(conditii[nr_conditii].camp, MAX_QUERY_LENGTH, "%s", cuv[i + 1]);
            snprintf(conditii[nr_conditii].op, MAX_QUERY_LENGTH, "%s", cuv[i + 2]);
            snprintf(conditii[nr_conditii].val, MAX_QUERY_LENGTH, "%s", cuv[i + 3]);
            nr_conditii++;
        }

        // Am obtinut conditii

        // Retin daca am afisat ceva
        int afis = 0;

        if (!strcmp(instructiune, "SELECT")) {
            if (strcmp(tabel, "studenti") == 0) {
                for (int j = 0; j < s->nr_studenti; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    // Retin daca e primul camp afisat din rand
                    int first = 1;
                    for (int x = 0; x < nr_campuri; x++) {
                        if (strcmp(campuri[x], "id") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%d", s->studenti[j].id);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "nume") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%s", s->studenti[j].nume);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "an_studiu") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%d", s->studenti[j].an_studiu);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "statut") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%c", s->studenti[j].statut);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "medie_generala") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%.2f", s->studenti[j].medie_generala);
                                    afis = 1;
                        }
                    }
                    if (afis == 1)
                        printf("\n");
                }
            } else if (strcmp(tabel, "materii") == 0) {
                for (int j = 0; j < s->nr_materii; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    // Retin daca e primul camp afisat din rand
                    int first = 1;
                    for (int x = 0; x < nr_campuri; x++) {
                        if (strcmp(campuri[x], "id") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%d", s->materii[j].id);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "nume") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%s", s->materii[j].nume);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "nume_titular") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%s", s->materii[j].nume_titular);
                                    afis = 1;
                        }
                    }
                    if (afis == 1)
                        printf("\n");
                }
            } else if (strcmp(tabel, "inrolari") == 0) {
                for (int j = 0; j < s->nr_inrolari; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    // Retin daca e primul camp afisat din rand
                    int first = 1;
                    for (int x = 0; x < nr_campuri; x++) {
                        if (strcmp(campuri[x], "id_student") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%d", s->inrolari[j].id_student);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "id_materie") == 0 || strcmp(campuri[x], "*") == 0) {
                                    if (first == 1) first = 0;
                                    else
                                        printf(" ");
                                    printf("%d", s->inrolari[j].id_materie);
                                    afis = 1;
                        }
                        if (strcmp(campuri[x], "note") == 0 || strcmp(campuri[x], "*") == 0) {
                            if (first == 1) first = 0;
                            else
                                printf(" ");
                            printf("%.2f %.2f %.2f", s->inrolari[j].note[0],
                                s->inrolari[j].note[1], s->inrolari[j].note[2]);
                                afis = 1;
                        }
                    }
                    if (afis == 1)
                        printf("\n");
                }
            }
        } else if (!strcmp(instructiune, "DELETE")) {
            if (strcmp(tabel, "studenti") == 0) {
                int j = 0;
                while (j < s->nr_studenti) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j)) {
                        j++;
                        continue;
                    }
                    sterge_intrare(s, tabel, j);
                }
            } else if (strcmp(tabel, "materii") == 0) {
                int j = 0;
                while (j < s->nr_materii) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j)) {
                        j++;
                        continue;
                    }

                    // Eliberez memoria aferenta
                    free(s->materii[j].nume);
                    free(s->materii[j].nume_titular);

                    sterge_intrare(s, tabel, j);
                }
            } else if (strcmp(tabel, "inrolari") == 0) {
                int j = 0;
                while (j < s->nr_inrolari) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j)) {
                        j++;
                        continue;
                    }
                    sterge_intrare(s, tabel, j);
                    actualizeaza_mg(s);
                }
            }
        } else if (!strcmp(instructiune, "UPDATE")) {
            if (strcmp(tabel, "studenti") == 0) {
                for (int j = 0; j < s->nr_studenti; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    if (strcmp(setare.camp, "nume") == 0) {
                        char nume[MAX_QUERY_LENGTH];
                        snprintf(nume, 2 * MAX_QUERY_LENGTH, "%s %s", setare.val[0], setare.val[1]);
                        snprintf(s->studenti[j].nume, MAX_QUERY_LENGTH, "%s", nume);
                    }
                    if (strcmp(setare.camp, "an_studiu") == 0) {
                        int an_studiu = atoi(setare.val[0]);
                        s->studenti[j].an_studiu = an_studiu;
                    }
                    if (strcmp(setare.camp, "statut") == 0) {
                        s->studenti[j].statut = setare.val[0][0];
                    }
                    if (strcmp(setare.camp, "medie_generala") == 0) {
                        float mg = (float)atof(setare.val[0]);
                        s->studenti[j].medie_generala = mg;
                    }
                }
            } else if (strcmp(tabel, "materii") == 0) {
                for (int j = 0; j < s->nr_materii; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    if (strcmp(setare.camp, "nume") == 0) {
                        char nume[MAX_QUERY_LENGTH];
                        snprintf(nume, 2 * MAX_QUERY_LENGTH, "%s %s", setare.val[0], setare.val[1]);
                        snprintf(s->materii[j].nume, MAX_QUERY_LENGTH, "%s", nume);
                    }
                    if (strcmp(setare.camp, "nume_titular") == 0) {
                        char nume[MAX_QUERY_LENGTH];
                        snprintf(nume, 2 * MAX_QUERY_LENGTH, "%s %s", setare.val[0], setare.val[1]);
                        snprintf(s->materii[j].nume_titular, MAX_QUERY_LENGTH, "%s", nume);
                    }
                }
            } else if (strcmp(tabel, "inrolari") == 0) {
                for (int j = 0; j < s->nr_inrolari; j++) {
                    // Verifica toate conditiile impuse
                    if (!respecta_conditiile(conditii, nr_conditii, tabel, s, j))
                        continue;
                    if (strcmp(setare.camp, "id_student") == 0) {
                        int id_student = atoi(setare.val[0]);
                        s->inrolari[j].id_student = id_student;
                    }
                    if (strcmp(setare.camp, "id_materie") == 0) {
                        int id_materie = atoi(setare.val[0]);
                        s->inrolari[j].id_materie = id_materie;
                    }
                    if (strcmp(setare.camp, "note") == 0) {
                        float nota0 = (float)atof(setare.val[0]);
                        float nota1 = (float)atof(setare.val[1]);
                        float nota2 = (float)atof(setare.val[2]);
                        s->inrolari[j].note[0] = nota0;
                        s->inrolari[j].note[1] = nota1;
                        s->inrolari[j].note[2] = nota2;
                        actualizeaza_mg(s);
                    }
                }
            }
        }
    }

    elibereaza_secretariat(&s);
    return 0;
}
