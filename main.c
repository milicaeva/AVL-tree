#include <stdio.h>
#include <stdlib.h>

typedef struct Cvor {
    int info,balans;
    struct cvor* levi;
    struct cvor* desni;
}cvor;

typedef struct CvorNaSteku {
    struct Cvor* cvor;
    struct CvorNaSteku* sled;
}cvorNaSteku;

typedef struct Stek {
    cvorNaSteku* top;
}stek;

void levaRotacija(cvor* x) {
    cvor* y, *pom;
    y=x->desni;
    pom=y->levi;
    y->levi=x;
    x->desni=pom;
}

void desnaRotacija(cvor* x) {
    cvor* y, *pom;
    y=x->levi;
    pom=y->desni;
    y->desni=x;
    x->levi=pom;
}

stek* napraviStek() {
    stek* stek=malloc(sizeof(stek));
    stek->top=NULL;
    return stek;
}

int prazan(stek* stek) {
    if(stek->top==NULL) return 1;
    else return 0;
}

void obrisiStek(stek* stek) {
    cvorNaSteku* cvor=stek->top;
    while(cvor!=NULL) {
        cvorNaSteku* pom=cvor;
        cvor=cvor->sled;
        free(pom);
    }
}

void push(stek* stek, cvor* cvor) {
    cvorNaSteku* noviCvor=malloc(sizeof(cvorNaSteku));
    noviCvor->cvor=cvor;
    noviCvor->sled=stek->top;
    stek->top=noviCvor;
}

cvor* pop(stek* stek) {
    if(stek->top==NULL) return NULL;
    cvorNaSteku* topCvor=stek->top;
    cvor* cvor=topCvor->cvor;
    stek->top=topCvor->sled;
    free(topCvor);
    return cvor;
}

cvor* dodajCvor(cvor *koren,int kljuc){
    cvor *p,*x,*y,*q,*otacX,*otacP;
    int fl;
    otacX=otacP=NULL;
    p=x=koren;

    while(p!=NULL){
        if(kljuc==p->info) return koren;
        else if(kljuc<p->info) q=p->levi;
        else q=p->desni;
        if(q!=NULL && q->balans!=0){
            otacX=p;
            x=q;
        }
        otacP=p;
        p=q;
    }

    q=malloc(sizeof(cvor));
    q->info=kljuc;
    q->balans=0;
    q->levi=q->desni=NULL;

    if(koren==NULL){
        koren=q;
        return koren;
    }

    if (kljuc>otacP->info) otacP->desni=q;
    else otacP->levi=q;

    if(kljuc>x->info) p=x->desni;
    else p=x->levi;
    y=p;

    while(p!=q){
        if(kljuc>p->info){
            p->balans=-1;
            p=p->desni;
        }
        else{
            p->balans=1;
            p=p->levi;
        }
    }

    if(kljuc>x->info) fl=-1;
    else fl=1;
    if(x->balans==0){
        x->balans=fl;
        return koren;
    }
    if(x->balans!=fl){
        x->balans=0;
        return koren;
    }
    if(y->balans==fl){
        p=y;
        if(fl==-1) levaRotacija(x);
        else desnaRotacija(x);
        x->balans=y->balans=0;
    }
    else{
        if(fl==1){
            p=y->desni;
            x->levi=p;
            levaRotacija(y);
            desnaRotacija(x);
        }
        else{
            p=y->levi;
            x->desni=p;
            desnaRotacija(y);
            levaRotacija(x);
        }
        if(p->balans==0) x->balans=y->balans=0;
        else{
            if(p->balans==fl){
                x->balans=-fl;
                y->balans=0;
            }
            else{
                x->balans=0;
                y->balans=fl;
            }
            p->balans=0;
        }
    }

    if(otacX==NULL) koren=p;
    else{
        if(x==otacX->desni) otacX->desni=p;
        else otacX->levi=p;
    }
    return koren;
}

int proveriPripadnost(cvor* koren, int kljuc){
    cvor *p=koren;
    while(p!=NULL && kljuc!=p->info){
        if(kljuc<p->info) p=p->levi;
        else p=p->desni;
    }
    if(p==NULL) return -1;
    else return 1;
}

int izracunajVisinuCvora(cvor* koren, int kljuc) {
    cvor *p=koren;
    int visina=0;
    if(p->info==kljuc) return visina;
    while(p!=NULL && kljuc!=p->info){
        if(kljuc<p->info) p=p->levi;
        else p=p->desni;
        visina++;
    }
    return visina;
}

void ispisiStablo(cvor* koren){
    cvor* p=koren;
    stek* stek=napraviStek();
    while(1){
        while(p!=NULL){
            push(stek,p);
            p=p->desni;
        }
        if(!prazan(stek)){
            p=pop(stek);
            int visina= izracunajVisinuCvora(koren,p->info);
            for(int i=1;i<=visina;i++) printf("      ");
            printf("%d\n",p->info);
            p=p->levi;
        }
        else{
            obrisiStek(stek);
            return;
        }
    }
}

void obrisiStablo(cvor *koren){
    cvor* p=koren,*obrisan;
    stek* stek=napraviStek();
    while(1){
        while(p!=NULL){
            push(stek,p);
            p=p->levi;
        }
        if(!prazan(stek)){
            p=pop(stek);
            obrisan=p;
            p=p->desni;
            free(obrisan);
        }
        else{
            obrisiStek(stek);
            return;
        }
    }
}

void simetricnaRazlika(){
    int a,b,element,ind;
    cvor *skupA=NULL, *skupB=NULL,*stablo=NULL,*p;

    printf("Unesite broj elemenata skupa A: ");
    scanf("%d",&a);
    for(int i=0;i<a;i++){
        printf("Element %d skupa A: ",i+1);
        scanf("%d",&element);
        skupA=dodajCvor(skupA,element);
        if(i==a-1) putchar('\n');
    }
    printf("Unesite broj elemenata skupa B: ");
    scanf("%d",&b);
    for(int i=0;i<b;i++){
        printf("Element %d skupa B: ",i+1);
        scanf("%d",&element);
        skupB=dodajCvor(skupB,element);
        if(i==b-1) putchar('\n\n');
    }

    p=skupA;
    stek* stek=napraviStek();
    while(1){
        while(p!=NULL){
            push(stek,p);
            p=p->levi;
        }
        if(!prazan(stek)){
            p=pop(stek);
            ind=proveriPripadnost(skupB,p->info);
            if(ind==-1) stablo=dodajCvor(stablo,p->info);
            p=p->desni;
        }
        else break;
    }
    p=skupB;
    while(1){
        while(p!=NULL){
            push(stek,p);
            p=p->levi;
        }
        if(!prazan(stek)){
            p=pop(stek);
            ind=proveriPripadnost(skupA,p->info);
            if(ind==-1) stablo=dodajCvor(stablo,p->info);
            p=p->desni;
        }
        else break;
    }
    printf("Stablo A: \n\n");
    ispisiStablo(skupA);
    printf("\nStablo B: \n\n");
    ispisiStablo(skupB);
    printf("\nSimetricna razlika stabla A i B: \n\n");
    ispisiStablo(stablo);
    obrisiStek(stek);
    putchar('\n');
}

int main() {
    cvor* koren;
    int br,brElem,element,zaPripadnost;
    while(1){
        printf("1. Napravi prazan skup\n2. Dodaj element/elemente u skup\n3. Proveri pripadnost elementa skupu\n4. Ispisi AVL stablo\n5. Obrisi stablo\n6. Izracunaj simetricnu razliku skupa A i B\n0. Prekini izvrsavanje programa\n\nIzaberi:");
        scanf("%d",&br);
        putchar('\n');
        if(br<0 || br>6) {
            printf("Izabrao/la si opciju koja ne postoji!\nPokusaj ponovo!\n");
            continue;
        }
        if(br==0){
            obrisiStablo(koren);
            exit(0);
        }
        if(br==1) {
            koren=NULL;
            printf("\nKreiran je prazan skup!\n\n");
        }
        if(br==2) {
            printf("Broj elemenata koje zelite da unesete: ");
            scanf("%d",&brElem);
            for(int i=0;i<brElem;i++){
                printf("Element %d: ",i+1);
                scanf("%d",&element);
                koren=dodajCvor(koren,element);
                if(i==brElem-1) putchar('\n');
            }
        }
        if(br==3) {
            printf("Unesite element: ");
            scanf("%d",&zaPripadnost);
            int ind=proveriPripadnost(koren, zaPripadnost);
            if(ind==-1) printf("\nBroj %d ne pripada skupu!\n\n",zaPripadnost);
            else printf("\nBroj %d pripada skupu!\n\n",zaPripadnost);

        }
        if(br==4){
            putchar('\n');
            ispisiStablo(koren);
            putchar('\n');
        }
        if(br==5) {
            printf("Stablo je obrisano!\n\n");
            obrisiStablo(koren);
        }
        if(br==6) {
            simetricnaRazlika();
        }
    }
    return 0;
}