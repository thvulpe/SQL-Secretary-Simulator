// Realizat de Theodor Vulpe - 315CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task1.h"

#define MAX_STUDENTI 500
#define MAX_MATERII 500
#define MAX_INROLARI 500

#define DIMENSIUNE_LINIE 256

// Verifica daca o linie este header
// e.g. [STUDENTI]
int linia_este_header(char* line) {
    return (line[0] == '[');
}

// Aloca dinamic un secretariat
secretariat *aloca_secretariat(secretariat **s) {
    *s = malloc(sizeof(secretariat));
    if (*s == NULL) {
        fprintf(stderr, "Eroare alocare memorie");
        return NULL;
    }
    (*s)->studenti = malloc(MAX_STUDENTI * sizeof(student));
    if ((*s)->studenti == NULL) {
        free(*s);
        fprintf(stderr, "Eroare alocare memorie");
        return NULL;
    }
    (*s)->materii = malloc(MAX_MATERII * sizeof(materie));
    if ((*s)->materii == NULL) {
        free((*s)->studenti);
        free(*s);
        fprintf(stderr, "Eroare alocare memorie");
        return NULL;
    }

    (*s)->inrolari = malloc(MAX_INROLARI * sizeof(inrolare));
    if ((*s)->inrolari == NULL) {
        free((*s)->studenti);
        free((*s)->materii);
        free(*s);
        fprintf(stderr, "Eroare alocare memorie");
        return NULL;
    }

    return *s;
}


// Ia o linie din fisierul de intrare si extrage datele despre un student
void proceseaza_student(char* line, int index, secretariat* s) {
    line = strtok(line, ", ");
    // ID
    int id = atoi(line);
    s->studenti[index].id = id;
    line = strtok(NULL, ", ");

    // nume complet
    char nume[MAX_STUDENT_NAME];
    snprintf(nume, MAX_STUDENT_NAME, "%s", line);
    line = strtok(NULL, ", ");
    snprintf(nume + strlen(nume), MAX_STUDENT_NAME, "%s", " ");
    snprintf(nume + strlen(nume), MAX_STUDENT_NAME, "%s", line);
    snprintf(s->studenti[index].nume, MAX_STUDENT_NAME, "%s", nume);
    line = strtok(NULL, ", ");

    // an studiu
    int an = atoi(line);
    s->studenti[index].an_studiu = an;
    line = strtok(NULL, ", ");

    // statut
    s->studenti[index].statut = line[0];

    // medie generala
    s->studenti[index].medie_generala = 0.0f;
}

// Ia o linie din fisierul de intrare si extrage datele despre o materie
void proceseaza_materie(char* line, int index, secretariat* s) {
    line = strtok(line, ", ");
    // ID
    int id = atoi(line);
    s->materii[index].id = id;
    line = strtok(NULL, ", ");

    // nume
    char nume[MAX_STUDENT_NAME];
    snprintf(nume, MAX_STUDENT_NAME, "%s", line);

    s->materii[index].nume = malloc(sizeof(char) * MAX_STUDENT_NAME);
    snprintf(s->materii[index].nume, MAX_STUDENT_NAME, "%s", nume);
    line = strtok(NULL, ", ");

    // titular
    char titular[MAX_STUDENT_NAME];
    snprintf(titular, MAX_STUDENT_NAME, "%s", line);
    line = strtok(NULL, ", ");
    snprintf(titular + strlen(titular), MAX_STUDENT_NAME, "%s", " ");
    snprintf(titular + strlen(titular), MAX_STUDENT_NAME, "%s", line);
    titular[strlen(titular) - 1] = '\0';

    s->materii[index].nume_titular = malloc(sizeof(char) * MAX_STUDENT_NAME);
    snprintf(s->materii[index].nume_titular, MAX_STUDENT_NAME, "%s", titular);
}

// Ia o linie din fisierul de intrare si extrage datele despre o inrolare
void proceseaza_inrolare(char* line, int index, secretariat* s) {
    line = strtok(line, ", ");
    // ID student
    int id_s = atoi(line);
    s->inrolari[index].id_student = id_s;
    line = strtok(NULL, ", ");

    // ID materie
    int id_m = atoi(line);
    s->inrolari[index].id_materie = id_m;
    line = strtok(NULL, ", ");

    // Note
    float nota1 = (float)atof(line);
    line = strtok(NULL, ", ");
    float nota2 = (float)atof(line);
    line = strtok(NULL, ", ");
    float nota3 = (float)atof(line);

    s->inrolari[index].note[0] = nota1;
    s->inrolari[index].note[1] = nota2;
    s->inrolari[index].note[2] = nota3;

    // Adauga la media generala a studentului
    s->studenti[id_s].medie_generala = nota1 + nota2 + nota3;
}

// Intoarce numarul de inrolari ale unui student (pentru m.g.)
int obtine_nr_inrolari(inrolare* inr, int nr_inrolari, int student_id) {
    int c = 0;
    for (int i = 0; i < nr_inrolari; i++) {
        if (inr[i].id_student == student_id) {
            c++;
        }
    }
    return c;
}

// Rotunjire zecimale
float rotunjire(float a) {
    // Necesar pentru clang-tidy
    const float y = 0.5f;
    const float z = 100.0f;

    // Caz particular de rotunjire medie
    const float fix = 0.0001f;
    a += fix;

    a = (float)((int)(a * z + y));
    return (a / z);
}

// Obtine indexul din vectorul studenti bazat pe id
int index_din_id(secretariat* s, int id) {
    for (int i = 0; i < s->nr_studenti; i++) {
        if (s->studenti[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Actualizeaza media generala a studentilor
void actualizeaza_mg(secretariat* s) {
    int nr_studenti = s->nr_studenti;

    // Recalculez suma notelor pentru fiecare student
    for (int i = 0; i < nr_studenti; i++) {
        s->studenti[i].medie_generala = 0.0f;
    }
    for (int i = 0; i < s->nr_inrolari; i++) {
        inrolare inr = s->inrolari[i];
        int index = index_din_id(s, inr.id_student);
        // Verific daca studentul nu a fost sters
        if (index != -1) {
            s->studenti[index].medie_generala += inr.note[0] + inr.note[1] + inr.note[2];
        }
    }

    // Impart la numarul de materii
    for (int i = 0; i < nr_studenti; i++) {
        int nr_inrolari = obtine_nr_inrolari(s->inrolari, s->nr_inrolari, s->studenti[i].id);
        if (nr_inrolari) {
            s->studenti[i].medie_generala /= (float)nr_inrolari;
            s->studenti[i].medie_generala = rotunjire(s->studenti[i].medie_generala);
        }
    }
}

secretariat *citeste_secretariat(const char *nume_fisier) {
    // TODO(student): 1.1
    // Aloc dinamic structura
    secretariat *s = NULL;
    aloca_secretariat(&s);

    // Deschid fisierul
    FILE *in = fopen(nume_fisier, "rt");
    if (in == NULL) {
        fprintf(stderr, "EROARE: Nu poate deschide fisierul %s", nume_fisier);
        return NULL;
    }

    // Linia citita
    char line[DIMENSIUNE_LINIE];

    // Citeste studentii
    int nr_studenti = 0;
    // Trec de prima linie
    fgets(line, sizeof(line), in);
    fgets(line, sizeof(line), in);
    while (!linia_este_header(line)) {
        proceseaza_student(line, nr_studenti, s);
        nr_studenti++;
        // Citeste urmatoarea linie
        fgets(line, sizeof(line), in);
    }
    s->nr_studenti = nr_studenti;

    // Citeste materiile
    int nr_materii = 0;
    // Trec de header
    fgets(line, sizeof(line), in);
    while (!linia_este_header(line)) {
        proceseaza_materie(line, nr_materii, s);
        nr_materii++;
        // Citeste urmatoarea linie
        fgets(line, sizeof(line), in);
    }
    s->nr_materii = nr_materii;

    // Citeste inrolari
    int nr_inrolari = 0;
    while (fgets(line, DIMENSIUNE_LINIE, in) != NULL) {
        proceseaza_inrolare(line, nr_inrolari, s);
        nr_inrolari++;
    }
    s->nr_inrolari = nr_inrolari;

    actualizeaza_mg(s);

    // Inchide fisierul de intrare
    fclose(in);

    return s;
}

void adauga_student(secretariat *s, int id, char *nume, int an_studiu, char statut, float medie_generala) {
    // TODO(student): 1.2
    student* studenti = s->studenti;
    int nr_studenti = s->nr_studenti;

    studenti[nr_studenti].id = id;
    snprintf(studenti[nr_studenti].nume, MAX_STUDENT_NAME, "%s", nume);
    studenti[nr_studenti].an_studiu = an_studiu;
    studenti[nr_studenti].statut = statut;
    studenti[nr_studenti].medie_generala = medie_generala;
    (s->nr_studenti)++;
}

void elibereaza_secretariat(secretariat **s) {
    // TODO(student): 1.3
    free((*s)->studenti);
    for (int i = 0; i < (*s)->nr_materii; i++) {
        free((*s)->materii[i].nume);
        free((*s)->materii[i].nume_titular);
    }
    free((*s)->materii);
    free((*s)->inrolari);
    free(*s);
    *s = NULL;
}
