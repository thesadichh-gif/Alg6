#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

struct Graf {
    int n;
    int** matr;
    int* metki;
};

struct Uzel {
    int versh;
    struct Uzel* sled;
};

struct Spisok {
    int n;
    int* metki;
    struct Uzel** golova;
};

struct Graf* sozd_graf(int n) {
    struct Graf* g = (struct Graf*)malloc(sizeof(struct Graf));
    g->n = n;
    g->matr = NULL;
    g->metki = NULL;
    if (n > 0) {
        g->metki = (int*)malloc(n * sizeof(int));
        g->matr = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            g->metki[i] = i + 1;
            g->matr[i] = (int*)calloc(n, sizeof(int));
        }
    }
    return g;
}

void ochist_graf(struct Graf* g) {
    if (!g) return;
    if (g->matr) {
        for (int i = 0; i < g->n; i++) free(g->matr[i]);
        free(g->matr);
    }
    if (g->metki) free(g->metki);
    free(g);
}

struct Graf* gen_graf(int n, int ver) {
    struct Graf* g = sozd_graf(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 100 < ver) {
                g->matr[i][j] = 1;
                g->matr[j][i] = 1;
            }
        }
    }
    return g;
}

void vivod_matr(struct Graf* g) {
    if (!g || g->n == 0) {
        printf("Граф пуст\n");
        return;
    }
    printf("Матрица смежности:\n     ");
    for (int j = 0; j < g->n; j++) printf("v%-3d", g->metki[j]);
    printf("\n");
    for (int i = 0; i < g->n; i++) {
        printf("v%-3d ", g->metki[i]);
        for (int j = 0; j < g->n; j++) {
            printf("%-4d", g->matr[i][j]);
        }
        printf("\n");
    }
}

struct Spisok* sozd_spisok(int n) {
    struct Spisok* s = (struct Spisok*)malloc(sizeof(struct Spisok));
    s->n = n;
    s->metki = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) s->metki[i] = i + 1;
    s->golova = (struct Uzel**)calloc(n, sizeof(struct Uzel*));
    return s;
}

void ochist_spisok(struct Spisok* s) {
    if (!s) return;
    for (int i = 0; i < s->n; i++) {
        struct Uzel* tek = s->golova[i];
        while (tek) {
            struct Uzel* tmp = tek;
            tek = tek->sled;
            free(tmp);
        }
    }
    free(s->golova);
    free(s->metki);
    free(s);
}

void dob_uzel(struct Spisok* s, int idx, int v) {
    struct Uzel* tek = s->golova[idx];
    while (tek) {
        if (tek->versh == v) return;
        tek = tek->sled;
    }
    struct Uzel* nov = (struct Uzel*)malloc(sizeof(struct Uzel));
    nov->versh = v;
    nov->sled = s->golova[idx];
    s->golova[idx] = nov;
}

struct Spisok* matr_v_spisok(struct Graf* g) {
    struct Spisok* s = sozd_spisok(g->n);
    for (int i = 0; i < g->n; i++) {
        s->metki[i] = g->metki[i];
    }
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->matr[i][j]) {
                dob_uzel(s, i, g->metki[j]);
            }
        }
    }
    return s;
}

void vivod_spiska(struct Spisok* s) {
    printf("Списки смежности:\n");
    for (int i = 0; i < s->n; i++) {
        printf("v%d: ", s->metki[i]);
        struct Uzel* tek = s->golova[i];
        if (!tek) printf("пусто");
        while (tek) {
            printf("-> v%d ", tek->versh);
            tek = tek->sled;
        }
        printf("\n");
    }
}

int poisk_ind_matr(struct Graf* g, int metka) {
    for (int i = 0; i < g->n; i++) {
        if (g->metki[i] == metka) return i;
    }
    return -1;
}

int poisk_ind_spisok(struct Spisok* s, int metka) {
    for (int i = 0; i < s->n; i++) {
        if (s->metki[i] == metka) return i;
    }
    return -1;
}

struct Graf* otozhd_matr(struct Graf* g, int u, int v) {
    int u_i = poisk_ind_matr(g, u);
    int v_i = poisk_ind_matr(g, v);
    if (u_i == -1 || v_i == -1 || u_i == v_i) {
        printf("Неверные вершины\n");
        return NULL;
    }
    if (u_i > v_i) { int t = u_i; u_i = v_i; v_i = t; }

    struct Graf* res = sozd_graf(g->n - 1);
    int* kuda = (int*)malloc(g->n * sizeof(int));
    int k = 0;

    for (int i = 0; i < g->n; i++) {
        if (i == v_i) {
            kuda[i] = kuda[u_i];
        }
        else {
            res->metki[k] = g->metki[i];
            kuda[i] = k++;
        }
    }

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->matr[i][j]) {
                res->matr[kuda[i]][kuda[j]] = 1;
            }
        }
    }

    free(kuda);
    return res;
}

struct Graf* styag_matr(struct Graf* g, int u, int v) {
    int u_i = poisk_ind_matr(g, u);
    int v_i = poisk_ind_matr(g, v);
    if (u_i == -1 || v_i == -1 || u_i == v_i) {
        printf("Неверные вершины\n");
        return NULL;
    }
    if (g->matr[u_i][v_i] == 0) {
        printf("Между вершинами нет ребра!\n");
        return NULL;
    }
    if (u_i > v_i) { int t = u_i; u_i = v_i; v_i = t; }

    struct Graf* res = sozd_graf(g->n - 1);
    int* kuda = (int*)malloc(g->n * sizeof(int));
    int k = 0;

    for (int i = 0; i < g->n; i++) {
        if (i == v_i) {
            kuda[i] = kuda[u_i];
        }
        else {
            res->metki[k] = g->metki[i];
            kuda[i] = k++;
        }
    }

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->matr[i][j]) {
                if ((i == u_i && j == v_i) || (i == v_i && j == u_i)) continue;
                res->matr[kuda[i]][kuda[j]] = 1;
            }
        }
    }

    free(kuda);
    return res;
}

struct Graf* rasschep_matr(struct Graf* g, int u) {
    int u_i = poisk_ind_matr(g, u);
    if (u_i == -1) {
        printf("Неверная вершина\n");
        return NULL;
    }

    struct Graf* res = sozd_graf(g->n + 1);
    int max_m = 0;
    for (int i = 0; i < g->n; i++) {
        res->metki[i] = g->metki[i];
        if (g->metki[i] > max_m) max_m = g->metki[i];
    }
    res->metki[g->n] = max_m + 1;

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            res->matr[i][j] = g->matr[i][j];
        }
    }

    int nov_v = g->n;
    for (int i = 0; i < g->n; i++) {
        if (g->matr[u_i][i]) {
            if (i == u_i) {
                res->matr[nov_v][nov_v] = 1;
            }
            else {
                res->matr[nov_v][i] = 1;
                res->matr[i][nov_v] = 1;
            }
        }
    }
    res->matr[u_i][nov_v] = 1;
    res->matr[nov_v][u_i] = 1;

    return res;
}

struct Spisok* otozhd_spisok(struct Spisok* s, int u, int v) {
    int u_i = poisk_ind_spisok(s, u);
    int v_i = poisk_ind_spisok(s, v);
    if (u_i == -1 || v_i == -1 || u_i == v_i) {
        printf("Неверные вершины\n");
        return NULL;
    }
    if (u_i > v_i) {
        int ti = u_i; u_i = v_i; v_i = ti;
        int tm = u; u = v; v = tm;
    }

    struct Spisok* res = sozd_spisok(s->n - 1);
    int k = 0;
    for (int i = 0; i < s->n; i++) {
        if (i != v_i) res->metki[k++] = s->metki[i];
    }

    for (int i = 0; i < s->n; i++) {
        int m_ot = (i == v_i) ? u : s->metki[i];
        int ind_ot = poisk_ind_spisok(res, m_ot);

        struct Uzel* tek = s->golova[i];
        while (tek) {
            int m_do = (tek->versh == v) ? u : tek->versh;
            dob_uzel(res, ind_ot, m_do);
            tek = tek->sled;
        }
    }
    return res;
}

struct Spisok* styag_spisok(struct Spisok* s, int u, int v) {
    int u_i = poisk_ind_spisok(s, u);
    int v_i = poisk_ind_spisok(s, v);
    if (u_i == -1 || v_i == -1 || u_i == v_i) {
        printf("Неверные вершины\n");
        return NULL;
    }

    int est_rebro = 0;
    struct Uzel* tek = s->golova[u_i];
    while (tek) {
        if (tek->versh == v) { est_rebro = 1; break; }
        tek = tek->sled;
    }
    if (!est_rebro) {
        printf("Между вершинами нет ребра!\n");
        return NULL;
    }

    if (u_i > v_i) {
        int ti = u_i; u_i = v_i; v_i = ti;
        int tm = u; u = v; v = tm;
    }

    struct Spisok* res = sozd_spisok(s->n - 1);
    int k = 0;
    for (int i = 0; i < s->n; i++) {
        if (i != v_i) res->metki[k++] = s->metki[i];
    }

    for (int i = 0; i < s->n; i++) {
        int m_ot = (i == v_i) ? u : s->metki[i];
        int ind_ot = poisk_ind_spisok(res, m_ot);

        tek = s->golova[i];
        while (tek) {
            if ((s->metki[i] == u && tek->versh == v) || (s->metki[i] == v && tek->versh == u)) {
                tek = tek->sled;
                continue;
            }
            int m_do = (tek->versh == v) ? u : tek->versh;
            dob_uzel(res, ind_ot, m_do);
            tek = tek->sled;
        }
    }
    return res;
}

struct Spisok* rasschep_spisok(struct Spisok* s, int u) {
    int u_i = poisk_ind_spisok(s, u);
    if (u_i == -1) {
        printf("Неверная вершина\n");
        return NULL;
    }

    struct Spisok* res = sozd_spisok(s->n + 1);
    int max_m = 0;
    for (int i = 0; i < s->n; i++) {
        res->metki[i] = s->metki[i];
        if (s->metki[i] > max_m) max_m = s->metki[i];
    }
    int nov_m = max_m + 1;
    int nov_i = s->n;
    res->metki[nov_i] = nov_m;

    for (int i = 0; i < s->n; i++) {
        struct Uzel* tek = s->golova[i];
        while (tek) {
            dob_uzel(res, i, tek->versh);
            tek = tek->sled;
        }
    }

    struct Uzel* tek = s->golova[u_i];
    while (tek) {
        if (tek->versh == u) {
            dob_uzel(res, nov_i, nov_m);
        }
        else {
            dob_uzel(res, nov_i, tek->versh);
            int sosed_i = poisk_ind_spisok(res, tek->versh);
            if (sosed_i != -1) dob_uzel(res, sosed_i, nov_m);
        }
        tek = tek->sled;
    }

    dob_uzel(res, u_i, nov_m);
    dob_uzel(res, nov_i, u);

    return res;
}

struct Graf* obedinenie(struct Graf* g1, struct Graf* g2) {
    int n = (g1->n > g2->n) ? g1->n : g2->n;
    struct Graf* res = sozd_graf(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int a = (i < g1->n && j < g1->n) ? g1->matr[i][j] : 0;
            int b = (i < g2->n && j < g2->n) ? g2->matr[i][j] : 0;
            res->matr[i][j] = (a || b) ? 1 : 0;
        }
    }
    return res;
}

struct Graf* peresechenie(struct Graf* g1, struct Graf* g2) {
    int n = (g1->n < g2->n) ? g1->n : g2->n;
    struct Graf* res = sozd_graf(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res->matr[i][j] = (g1->matr[i][j] && g2->matr[i][j]) ? 1 : 0;
        }
    }
    return res;
}

struct Graf* kolc_summa(struct Graf* g1, struct Graf* g2) {
    int max_n = (g1->n > g2->n) ? g1->n : g2->n;
    int** tmp = (int**)malloc(max_n * sizeof(int*));
    int* stepen = (int*)calloc(max_n, sizeof(int));
    for (int i = 0; i < max_n; i++) tmp[i] = (int*)calloc(max_n, sizeof(int));

    for (int i = 0; i < max_n; i++) {
        for (int j = 0; j < max_n; j++) {
            int a = (i < g1->n && j < g1->n) ? g1->matr[i][j] : 0;
            int b = (i < g2->n && j < g2->n) ? g2->matr[i][j] : 0;
            if (a ^ b) {
                tmp[i][j] = 1;
                stepen[i]++;
            }
        }
    }

    int ost[100], kol = 0;
    for (int i = 0; i < max_n; i++) {
        if (stepen[i] > 0) ost[kol++] = i;
    }

    struct Graf* res = sozd_graf(kol);
    for (int i = 0; i < kol; i++) {
        res->metki[i] = ost[i] + 1;
        for (int j = 0; j < kol; j++) {
            res->matr[i][j] = tmp[ost[i]][ost[j]];
        }
    }

    for (int i = 0; i < max_n; i++) free(tmp[i]);
    free(tmp);
    free(stepen);
    return res;
}

void dekart_proizv(struct Graf* g1, struct Graf* g2) {
    int n1 = g1->n, n2 = g2->n;
    int vsego = n1 * n2;
    int** m = (int**)malloc(vsego * sizeof(int*));
    for (int i = 0; i < vsego; i++) m[i] = (int*)calloc(vsego, sizeof(int));

    for (int i = 0; i < n1; i++) {
        for (int k = 0; k < n2; k++) {
            int r = i * n2 + k;
            for (int j = 0; j < n1; j++) {
                for (int l = 0; l < n2; l++) {
                    int c = j * n2 + l;
                    if ((i == j && g2->matr[k][l]) || (k == l && g1->matr[i][j])) {
                        m[r][c] = 1;
                    }
                }
            }
        }
    }

    printf("\nДекартово произведение G1 x G2:\n       ");
    for (int i = 0; i < n1; i++) {
        for (int k = 0; k < n2; k++) {
            printf("(%d,%d) ", g1->metki[i], g2->metki[k]);
        }
    }
    printf("\n");

    for (int i = 0; i < n1; i++) {
        for (int k = 0; k < n2; k++) {
            int r = i * n2 + k;
            printf("(%d,%d)  ", g1->metki[i], g2->metki[k]);
            for (int j = 0; j < n1; j++) {
                for (int l = 0; l < n2; l++) {
                    printf("%-6d", m[r][j * n2 + l]);
                }
            }
            printf("\n");
        }
    }

    for (int i = 0; i < vsego; i++) free(m[i]);
    free(m);
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    struct Graf* g1 = gen_graf(4, 50);
    struct Graf* g2 = gen_graf(4, 50);
    struct Spisok* s1 = matr_v_spisok(g1);
    struct Spisok* s2 = matr_v_spisok(g2);

    int vibor;
    while (1) {
        printf("\n1 Генерация G1 и G2\n");
        printf("2 Унарные операции (матрица G1)\n");
        printf("3 Унарные операции (списки G1)\n");
        printf("4 Бинарные операции (G1 и G2)\n");
        printf("5 Декартово произведение\n");
        printf("6 Вывод G1 и G2\n");
        printf("0 Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &vibor);

        if (vibor == 0) break;

        if (vibor == 1) {
            int n1, p1, n2, p2;
            printf("Введите кол-во вершин и вероятность для G1: ");
            scanf("%d %d", &n1, &p1);
            printf("Введите кол-во вершин и вероятность для G2: ");
            scanf("%d %d", &n2, &p2);

            ochist_graf(g1); ochist_graf(g2);
            ochist_spisok(s1); ochist_spisok(s2);

            g1 = gen_graf(n1, p1);
            g2 = gen_graf(n2, p2);
            s1 = matr_v_spisok(g1);
            s2 = matr_v_spisok(g2);

            printf("\nГраф G1:\n");
            vivod_matr(g1);
            vivod_spiska(s1);
            printf("\nГраф G2:\n");
            vivod_matr(g2);
            vivod_spiska(s2);
        }
        else if (vibor == 2) {
            int op, u, v;
            printf("1 - Отождествление, 2 - Стягивание, 3 - Расщепление: ");
            scanf("%d", &op);
            struct Graf* res = NULL;
            if (op == 1) {
                printf("Введите вершины u и v: ");
                scanf("%d %d", &u, &v);
                res = otozhd_matr(g1, u, v);
            }
            else if (op == 2) {
                printf("Введите вершины u и v: ");
                scanf("%d %d", &u, &v);
                res = styag_matr(g1, u, v);
            }
            else if (op == 3) {
                printf("Введите вершину u: ");
                scanf("%d", &u);
                res = rasschep_matr(g1, u);
            }
            if (res) {
                vivod_matr(res);
                ochist_graf(g1);
                g1 = res;
                ochist_spisok(s1);
                s1 = matr_v_spisok(g1);
            }
        }
        else if (vibor == 3) {
            int op, u, v;
            printf("1 - Отождествление, 2 - Стягивание, 3 - Расщепление: ");
            scanf("%d", &op);
            struct Spisok* res = NULL;
            if (op == 1) {
                printf("Введите вершины u и v: ");
                scanf("%d %d", &u, &v);
                res = otozhd_spisok(s1, u, v);
            }
            else if (op == 2) {
                printf("Введите вершины u и v: ");
                scanf("%d %d", &u, &v);
                res = styag_spisok(s1, u, v);
            }
            else if (op == 3) {
                printf("Введите вершину u: ");
                scanf("%d", &u);
                res = rasschep_spisok(s1, u);
            }
            if (res) {
                vivod_spiska(res);
                ochist_spisok(s1);
                s1 = res;
            }
        }
        else if (vibor == 4) {
            printf("\nОбъединение G1 и G2:\n");
            struct Graf* gu = obedinenie(g1, g2);
            vivod_matr(gu);
            ochist_graf(gu);

            printf("\nПересечение G1 и G2:\n");
            struct Graf* gi = peresechenie(g1, g2);
            vivod_matr(gi);
            ochist_graf(gi);

            printf("\nКольцевая сумма G1 и G2:\n");
            struct Graf* gk = kolc_summa(g1, g2);
            vivod_matr(gk);
            ochist_graf(gk);
        }
        else if (vibor == 5) {
            dekart_proizv(g1, g2);
        }
        else if (vibor == 6) {
            printf("\nГраф G1:\n");
            vivod_matr(g1);
            vivod_spiska(s1);
            printf("\nГраф G2:\n");
            vivod_matr(g2);
            vivod_spiska(s2);
        }
    }

    ochist_graf(g1); ochist_graf(g2);
    ochist_spisok(s1); ochist_spisok(s2);
    return 0;
}