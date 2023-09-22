#include <math.h>
#include "list.c"


List *page_hinkley(List *dados, T k, T h){
    List *alert = newList();
    Item *it = dados->first;

    T gt=0, Gt=0, zt, mu, sd; // mu = média; sd = desvio padrão
    T S=0, Sq=0, n = 0, i; // S = Soma dos itens; Sq = soma dos quadrados dos itens; n = quantidade de itens

    while (it != NULL){
        i = it->value;

        n++;
        S += i;
        Sq += i*i;
        mu = S/n;
        sd = sqrt(Sq/n-mu*mu);

        zt = sd?((i-mu)/sd):0;
        gt = gt + zt - k;
        Gt = fmin(gt, Gt);

        if ((gt - Gt) > h){
            list_append(alert, n);
            Gt = 0;
            n = 0;
            S = 0;
            Sq = 0;
        }

        it = it->next;
    }

    return alert;
}