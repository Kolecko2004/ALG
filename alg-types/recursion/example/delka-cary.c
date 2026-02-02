// 1. Geometrické pravidlo(Dělení)
// Základem je trojúhelník $T$ se stranami seřazenými podle délky
//     : $ |
//       AB | \le | BC | \le | AC | $.Vždy se dělí nejdelší strana($AC$)
// na tři stejné díly body $D$ a $E$.Z původního trojúhelníka vzniknou 3 nové
//     : $T(1) = ABD$,
//       $T(2) = BCE$ a $T(3) = BDE$ .2. Rekurzivní proces(Množina $M$) Množina trojúhelníků $M(T, p)
// $ se tvoří následovně : Zastavovací podmínka
//     : Pokud je obvod aktuálního trojúhelníka $\le p$,
//       trojúhelník se už dál nedělí a stává se prvkem výsledné množiny.Větvení
//     : Pokud je obvod $ > p$,
//       trojúhelník zaniká a proces se opakuje pro jeho tři
//       potomky($T(1), T(2), T(3) $) .3. Cíl úlohy(Výstup)
// Musíš určit dvě hodnoty : Mohutnost $ | M |
//                           $ : Celkový počet "listových" trojúhelníků(těch, které už nešly dál dělit)
//                                   .Délka množiny
//     : Součet délek všech unikátních čar v obrázku.Výpočetní vzorec : $L = \frac{1} {
//     2
// }
// \text{obvod}(T_{původní}) + \frac{1} { 2 }
// \sum \text{obvody všech prvků v} M$
//         .Klíčové technické detailyVstup : Souřadnice tří bodů a hodnota
//                                           $p$(vše double)
//         .Přesnost : Obvody se nebudou blížit k $p$ více než o $10
//     ^ { -4 } $,
//     což eliminuje problémy s plovoucí řádovou čárkou při porovnávání.Řazení
//     : I když má trojúhelník dvě strany stejně dlouhé,
//     výsledek(počet a délka) vyjde vždy stejně,
//     pokud dodržíš pravidlo o nejdelší straně.

#include <math.h>
#include <stdio.h>

    /* Struktura pro reprezentaci bodu v 2D rovině */
    typedef struct {
    double x, y;
} Point;

/* Struktura pro předávání výsledků rekurze:
   počet listových trojúhelníků a součet jejich obvodů */
typedef struct {
    int count;
    double total_perimeter_sum;
} Result;

/* Euklidovská vzdálenost mezi dvěma body: sqrt((x2-x1)^2 + (y2-y1)^2) */
double dist(Point a, Point b) { return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)); }

/* Hlavní rekurzivní funkce pro zpracování dělení trojúhelníka */
Result solve_recursive(Point p1, Point p2, Point p3, double p_limit) {
    // 1. Vypočteme délky všech tří stran aktuálního trojúhelníka
    double d12 = dist(p1, p2);
    double d23 = dist(p2, p3);
    double d13 = dist(p1, p3);

    // 2. Identifikace vrcholů A, B, C podle zadání (|AB| <= |BC| <= |AC|)
    // Nejdříve najdeme nejdelší stranu a označíme ji jako AC.
    Point A, B, C;
    if (d12 >= d23 && d12 >= d13) {
        // Nejdelší je strana mezi p1 a p2
        C = p1;
        A = p2;
        B = p3;
    } else if (d23 >= d12 && d23 >= d13) {
        // Nejdelší je strana mezi p2 a p3
        C = p2;
        A = p3;
        B = p1;
    } else {
        // Nejdelší je strana mezi p1 a p3
        C = p3;
        A = p1;
        B = p2;
    }

    // 3. Kontrola zbývající podmínky |AB| <= |BC|
    // Pokud po určení nejdelší strany AC neplatí |AB| <= |BC|, prohodíme A a C.
    // Tím se strana AC nezmění, ale AB a BC se prohodí.
    if (dist(A, B) > dist(B, C)) {
        Point temp = A;
        A = C;
        C = temp;
    }

    // 4. Výpočet obvodu aktuálního trojúhelníka
    double sideAB = dist(A, B);
    double sideBC = dist(B, C);
    double sideAC = dist(A, C);
    double perimeter = sideAB + sideBC + sideAC;

    // 5. Bázový případ rekurze: Pokud je obvod pod limitem, končíme dělení
    if (perimeter <= p_limit) {
        Result res = {1, perimeter};
        return res;
    } else {
        // 6. Rekurzivní krok: Rozdělení strany AC na třetiny body D a E
        Point D, E;
        // Bod D leží v 1/3 cesty z A do C
        D.x = A.x + (C.x - A.x) / 3.0;
        D.y = A.y + (C.y - A.y) / 3.0;
        // Bod E leží ve 2/3 cesty z A do C
        E.x = A.x + 2.0 * (C.x - A.x) / 3.0;
        E.y = A.y + 2.0 * (C.y - A.y) / 3.0;

        // Definujeme tři nové trojúhelníky: T(1)=ABD, T(2)=BCE, T(3)=BDE
        Result r1 = solve_recursive(A, B, D, p_limit);
        Result r2 = solve_recursive(B, C, E, p_limit);
        Result r3 = solve_recursive(B, D, E, p_limit);

        // Sloučíme výsledky z podstromů rekurze
        Result final_res;
        final_res.count = r1.count + r2.count + r3.count;
        final_res.total_perimeter_sum =
            r1.total_perimeter_sum + r2.total_perimeter_sum + r3.total_perimeter_sum;
        return final_res;
    }
}

int main() {
    Point A, B, C;
    double p_limit;

    // Načtení vstupu (souřadnice 3 bodů a limit p)
    // Formát: Ax Ay Bx By Cx Cy p
    if (scanf("%lf %lf %lf %lf %lf %lf %lf", &A.x, &A.y, &B.x, &B.y, &C.x, &C.y, &p_limit) != 7) {
        return 0;
    }

    // Výpočet obvodu původního trojúhelníka pro závěrečný vzorec
    double initial_perimeter = dist(A, B) + dist(B, C) + dist(A, C);

    // Spuštění výpočtu
    Result result = solve_recursive(A, B, C, p_limit);

    // 7. Výpočet celkové délky čar (L) dle definice v zadání:
    // L = (obvod původního T / 2) + (součet obvodů všech koncových trojúhelníků / 2)
    double total_length = (initial_perimeter / 2.0) + (result.total_perimeter_sum / 2.0);

    // Výstup: mohutnost (celé číslo) a délka (na 2 desetinná místa)
    printf("%d %.2f\n", result.count, total_length);

    return 0;
}