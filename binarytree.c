#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct puusolmu_t {
  int luku;
  short tila; /* tasapainoilmaisin */
  struct puusolmu_t *vasen, *oikea;
} puusolmu, *puuosoitin;

void lisaa_solmu(puuosoitin *, int, int *);
void oikea_kierto(puuosoitin *, int *);
void tulosta_puu(puuosoitin, int);
void vasen_kierto(puuosoitin *, int *);

int main()
{
  int etp = 0, i, luvut[] = {2, 4, 6, 8, 10, 12, 14, 30, 28};
  puuosoitin puu = NULL;
  
  for(i = 0; luvut[i] != 0; i++)
  {
    lisaa_solmu(&puu, luvut[i], &etp);
/*     tulosta_puu(puu, 0);
    printf("\n"); */
  }
  tulosta_puu(puu, 0);
  printf("\n");

  return 0;
}

void lisaa_solmu(puuosoitin *emo, int luku, int *etp)
{
  if(!(*emo))
  {
    *etp = 1;
    if(!(*emo = (puuosoitin)malloc(sizeof(puusolmu))))
    {
      perror("malloc");
      exit(1);
    }
    (*emo)->vasen = (*emo)->oikea = NULL;
    (*emo)->tila = 0;
    (*emo)->luku = luku;
  } else if(luku < (*emo)->luku)
  {
    lisaa_solmu(&(*emo)->vasen, luku, etp);
    if(*etp)
    {
      switch((*emo)->tila)
      {
        case -1:
          (*emo)->tila = 0;
          *etp = 0;
          break;
        case 0:
          (*emo)->tila = 1;
          break;
        case 1:
          vasen_kierto(emo, etp);
      }
    }
  } else if(luku > (*emo)->luku)
  {
    lisaa_solmu(&(*emo)->oikea, luku, etp);
    if(*etp)
    {
      switch((*emo)->tila)
      {
        case 1:
          (*emo)->tila = 0;
          *etp = 0;
          break;
        case 0:
          (*emo)->tila = -1;
           break;
        case -1:
          oikea_kierto(emo, etp);
      }
    }
  } else
  {
    *etp = 0;
    printf("Luku %d on jo puussa\n", luku);
  }
}

int laske_syvyys(puuosoitin solmu, int arvo) {
    int i = 0;
    while (solmu->luku != arvo) {
        if (arvo < solmu->luku) {
            solmu = solmu->vasen;
        } else if (arvo > solmu->luku) {
            solmu = solmu->oikea;
        } else if (arvo == solmu->luku) {
            return i;
            printf("Syvyys %d", i);
        }
        i++;
    }

    return -1;
}

void tulosta_puu(puuosoitin solmu, int height)
{
  if(!solmu) {
        return;
    }
    height++;
  tulosta_puu(solmu->oikea, height);
    for(int i = 0; i < height; i++) {
        printf("    ");
    }
  printf("%d\n", solmu->luku);
  tulosta_puu(solmu->vasen, height);
}

void vasen_kierto(puuosoitin *emo, int *etp)
{
  puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->vasen;
  if(lapsi->tila == 1) /* LL-kierto */
  {
    (*emo)->vasen = lapsi->oikea;
    lapsi->oikea = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
  } else /* LR-kierto */
  {
    lapsenlapsi = lapsi->oikea;
    lapsi->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = lapsi;
    (*emo)->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = *emo;
    switch(lapsenlapsi->tila)
    {
      case 1:
        (*emo)->tila = -1;
        lapsi->tila = 0;
        break;
      case 0:
        (*emo)->tila = lapsi->tila = 0;
        break;
      case -1:
        (*emo)->tila = 0;
        lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}

void oikea_kierto(puuosoitin *emo, int *etp)
{
    puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->oikea;
  if(lapsi->tila == -1) /* RR-kierto */
  {
    (*emo)->oikea = lapsi->vasen;
    lapsi->vasen = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
  } else /* RL-kierto */
  {
    lapsenlapsi = lapsi->vasen;
    lapsi->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = lapsi;
    (*emo)->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = *emo;
    switch(lapsenlapsi->tila)
    {
      case 1:
        (*emo)->tila = -1;
        lapsi->tila = 0;
        break;
      case 0:
        (*emo)->tila = lapsi->tila = 0;
        break;
      case -1:
        (*emo)->tila = 0;
        lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}