#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h> 

#define boyut 128



typedef struct agacDugum {
    float frekans;
    char c;
    char code[boyut];
    struct agacDugum *sol;
    struct agacDugum *sag;

} agacDugums;

void minBulma(agacDugums *d[], float *min, int *minIndex, float *ikinciMin, int *ikinciMinIndex) {
    int i, k;
    k = 0;
    *minIndex = -1;

    while (k < 26 && d[k] == NULL) k++;

    *minIndex = k;
    *min = d[k]->frekans;

    for ( i = k ; i < 26; i ++ ) {
        if ( d[i] != NULL && d[i]->frekans < *min ) {
            *min = d[i]->frekans;
            *minIndex = i;
        }
    }

    k = 0;
    *ikinciMinIndex = -1;

    while ((k < 26 && d[k] == NULL) || (k == *minIndex && d[k] != NULL))
    k++;

    *ikinciMin = d[k]->frekans;
    *ikinciMinIndex = k;

    if (k == *minIndex)
    k ++;

    for ( i = k ; i < 26; i ++ ) {
        if ( d[i] != NULL && d[i]->frekans < *ikinciMin && i != *minIndex ) {
            *ikinciMin = d[i]->frekans;
            *ikinciMinIndex = i;
        }
    }
}

void encode(agacDugums *node, agacDugums **harf, char yon, short seviye, char* code) {
    int n;
    if ( node != NULL ) {
        if ((n = strlen(code)) < seviye) {
            if (yon == 'R') {
                strcat(code, "1");
            } else {
                if (yon == 'L') {
                    strcat(code, "0");
                }
            }
        } else {
            if (n >= seviye) {
                code[n - (n - seviye) - 1] = 0;
                if (yon == 'R') {
                    strcat(code, "1");
                } else {
                    if (yon == 'L') {
                        strcat(code, "0");
                    }
                }
            }
        }
        if (node->c >= 'A' && node->c <= 'Z') {
            strcpy(node->code, code);
            strcpy(harf[node->c - 'A']->code, code);
        }
        encode(node->sol, harf, 'L', seviye + 1, code);
        encode(node->sag, harf, 'R', seviye + 1, code);
    }
}

void huffman(char *inTxt ,char *outDosya ){
	
	char *key=malloc(1000);
	key[0]='>';
	agacDugums *d[26], *harf[26];
    char str[boyut];
    int kontrol;
    int  k, ind;
    float min, ikinciMin;
    int minIndex, ikinciMinIndex;
    int karakterSayi = 0;
	agacDugums *agac;

	FILE *in = fopen(inTxt, "r");
    FILE *out;

        int i;
        for (i = 'A'; i <= 'Z'; i++) {
            ind = i - 'A';
            d[ind] = NULL;
        }


        int strUzunluk;
        karakterSayi = 0;
        kontrol = fgets(str, boyut, in) != NULL;

		while(!feof(in) || kontrol) {
            strUzunluk = strlen(str);
                for (i = 0; i <strUzunluk; i ++ ) {
                str[i] = toupper(str[i]);
                if (str[i] >= 'A' && str[i] <= 'Z') {
                    karakterSayi ++;
                    ind = str[i] - 'A';
                    if (d[ind] == NULL) {
                        d[ind] = malloc(sizeof(agacDugums));
                        d[ind]->c = str[i];
                        d[ind]->frekans = 1;
                        d[ind]->sol = d[ind]->sag = NULL;
                    } else {
                        d[ind]->frekans += 1;
                    }
                }
            }
            if (kontrol) {
                kontrol = fgets(str, boyut, in) != NULL;
            }
        }

    fclose(in);

    for ( i = 0, strUzunluk = 0 ; i < 26 ; i ++ ) {
        harf[i] = d[i];
        if (d[i] != NULL) {
            d[i]->frekans /= karakterSayi;
            
            strUzunluk ++;
        }
    }

    int j = 1;
    do {
        minBulma(d, &min, &minIndex, &ikinciMin, &ikinciMinIndex);

        if (minIndex != -1 && ikinciMinIndex != -1 && minIndex != ikinciMinIndex) {
            agacDugums *gecici ;
            agac =malloc(sizeof(agacDugums));
            agac->frekans = d[minIndex]->frekans + d[ikinciMinIndex]->frekans;
            agac->c = j;
            agac->sol = d[minIndex];
            gecici =  malloc(sizeof(agacDugums));
            gecici ->c = d[ikinciMinIndex]->c;
            gecici ->frekans = d[ikinciMinIndex]->frekans;
            gecici ->sol = d[ikinciMinIndex]->sol;
            gecici ->sag = d[ikinciMinIndex]->sag;
            agac->sag = gecici ;

            d[minIndex] = agac;
            d[ikinciMinIndex] = NULL;
        }
        j ++;
    } while( j < strUzunluk );

    for ( i = 0 ; i < 26 ; i ++ ) {
        if (d[i] != NULL)  {
            char code[boyut];
            strcpy(code, "");
            encode(agac = d[i], harf, 0, 0, code);
            agacTara(d[i],key);
            break;
        }
    }
    in = fopen(inTxt, "r");
    out = fopen(outDosya, "w");
    kontrol = fgets(str, boyut, in) != NULL;
    while(!feof(in) || kontrol) {
        strUzunluk = strlen(str);
        for (i = 0; i < strUzunluk ; i ++ ) {
            str[i] = toupper(str[i]);
            if (str[i] >= 'A' && str[i] <= 'Z') {
                ind= str[i] - 'A';
                fputs(harf[ind]->code, out);
            }
        }
        if (kontrol) {
            kontrol = fgets(str, boyut, in) != NULL;
        }
    }

    fclose(in);
    fclose(out);

    //printf("\nbefore --> %d bit\n", karakterSayi * 8);

    out = fopen(outDosya, "r");
    kontrol = fgets(str, boyut, out) != NULL;
    karakterSayi = 0;
    while(!feof(out) || kontrol) {
        karakterSayi += strlen(str);
        if (kontrol) {
            kontrol = fgets(str, boyut, out) != NULL;
        }
    }
    fclose(out);

   // printf("after  --> %d bit\n", karakterSayi);

    out = fopen(outDosya, "r");
    kontrol = fgets(str, boyut, out) != NULL;
    while(!feof(out) || kontrol) {
        strUzunluk = strlen(str);
        if (kontrol) {
            kontrol = fgets(str, boyut, out) != NULL;
        }
    }
    fclose(out);
    
    FILE *savav = fopen(outDosya, "a");

	fputs(key, savav);
	fclose(savav);
}

void agacTara(agacDugums *node,char *key) {
	
    if ( node != NULL ) {
        if (node->c >= 'A' && node->c <= 'Z') {
           // printf("\t%c - \tkodu: %s\n", node->c, node->code);
			    strncat(key, &node->c, 1);   
			    strcat(key,node->code);
        }
        agacTara(node->sol,key);
        agacTara(node->sag,key);
    }
}

typedef struct Data{
int offset;
int length;
char ch;

}Data;

void yaz(Data sonuc,FILE* out){
	short int kayan=sonuc.offset<<6;
    short int off_len=kayan+sonuc.length;
    fwrite(&off_len, sizeof (off_len), 1, out);
    fwrite(&sonuc.ch, 1, sizeof(sonuc.ch),out);
 
}

float DosyaBoyut(char*metinTxt){
	
    float DosyaBoyut;

FILE *dosya = fopen(metinTxt, "rb");
fseek(dosya,0,SEEK_END);
DosyaBoyut=ftell(dosya);
fclose(dosya);
    return DosyaBoyut*0.000977;
}

Data encoder(char arama[],int aramaUzunluk,char ileri[],int ileriUzunluk){
    Data datas;
if(aramaUzunluk==0){
    datas.offset=0;
    datas.length=0;
    datas.ch=ileri[0];
    return datas;
}

if(ileriUzunluk==0){
    datas.offset=-1;
    datas.length=-1;
    datas.ch=' ';
    return datas;
}


int en_benzerlik=0;
int en_uzaklik=0;

int birlesimDiziUzunluk=aramaUzunluk+ileriUzunluk; // birlesim dizisi uzunluk
char *birlesimDizi = malloc(birlesimDiziUzunluk); // iki dizinin birlestirilecek dizi

//***********************************************************

//iki diziyi birlestirme islemi

    memcpy(birlesimDizi,arama,aramaUzunluk); 
    memcpy(birlesimDizi+aramaUzunluk,ileri,ileriUzunluk); 



//***********************************************************

//Arama dizisi ve ileri dizisi

//printf("search : ' ");
//int i;
//for(i=0; i<aramaUzunluk; i++)
//    printf("%c",arama[i]);
//printf(" ' ->");
//printf("look a head : ");
//for(i=0; i<ileriUzunluk; i++)
//    printf("%c",ileri[i]);

//***********************************************************

    int arama_indis=aramaUzunluk;
int p;
for(p=0; p<aramaUzunluk; p++ ){

    int benzerlik=0;
        // Benzerlik kontrol 
        while(birlesimDizi[p+benzerlik]==birlesimDizi[arama_indis+benzerlik]){

            benzerlik=benzerlik+1;

            if( (arama_indis+benzerlik)==birlesimDiziUzunluk){
                benzerlik=benzerlik-1;
                break;
            }
            if((p+benzerlik)>=arama_indis){
                break;
            }

        }
            // benzerlik ve uzaklik karsilastirma
            if(benzerlik>en_benzerlik){
                en_uzaklik=p;
                en_benzerlik=benzerlik;
        }


}           // Elde edilen verilen struck yapisi sayesinde coklu donduruluyor
            datas.offset=en_uzaklik;
            datas.length=en_benzerlik;
            datas.ch=birlesimDizi[arama_indis+en_benzerlik];
            return datas;
}

void lz77encoder(char *txt,char *outTxt){

        Data sonuclar;
        int x=16;
        int maxArama=1024;

        int maxlh = pow(2,(x-(log2(maxArama))));
        //***********************************************************
        FILE *metinBelgesi; // Okunacak metin belgesi
        metinBelgesi=fopen(txt,"r");

        FILE *yazilandosya; // Sikistirilmamis dosya
        yazilandosya=fopen(outTxt,"wb");

          int uzunluk=-1; // Txt Uzunlugu
            FILE *f;
            char ch;
            f=fopen(txt, "r");
            while(feof(f)==NULL){
                    ch=getc(f);
                    uzunluk++;
                }
            fclose(f);
        //***********************************************************
        // Metini karakter karakter okuma islemi ve diziye atama

        char  karakter;
        char* metin = malloc(uzunluk);
        karakter=fgetc(metinBelgesi);
        int id=0;
        while(karakter!=EOF){
            metin[id]=karakter;
            karakter=fgetc(metinBelgesi);
            id++;
        }
        fclose(metinBelgesi);
        //***********************************************************
        int aramaIterator=0;
        int ileriIterator=0;
        int control=0;
        int counter=0;
        while(ileriIterator<uzunluk){

            int aI=aramaIterator; // Dizilerdede baslangic yerini vermek icin bu degiskenlerde tutuyoruz
            int iL=ileriIterator; // Dizilerdede baslangic yerini vermek icin bu degiskenlerde tutuyoruz

            int bA=ileriIterator-aramaIterator; // Arama Dizisini olusturmak icin boyutunu tutuyoruz
            int bI=maxlh; // Arama Dizisini olusturmak icin boyutunu tutuyoruz


            if(bI>uzunluk){
                if(control==0){
                   bI=uzunluk;
                   control++;
                }
                else if(control>0){
                    bI=uzunluk-bA;
                }
            }
            else if(bI<=uzunluk){
                 control++;
                 if(control>0){
                 bI=uzunluk-bA;
                }
            }
            //**********************************************************************
            // Metin dizisinin içinden mevcut pozisyonlara gore arama dizisi ve ileri dizisini dolduruyoruz
            char*aramaDizisi=malloc(bA);
int k;
            for(k= 0; k<bA; k++, aI++){
                aramaDizisi[k]=metin[aI];
            }

            char *ileriDizi=malloc(bI);
int o;
            for(o= 0; o<bI; o++,iL++){
                ileriDizi[o]=metin[iL];
            }


            //**********************************************************************
            sonuclar=encoder(aramaDizisi,bA,ileriDizi,bI); // Lz77 icin arama islemi yapiliyor

           //printf("\n(%d - %d - %c)\n",sonuclar.offset,sonuclar.length,sonuclar.ch);

            yaz(sonuclar,yazilandosya);

            ileriIterator=ileriIterator+sonuclar.length+1;
            aramaIterator=ileriIterator-maxArama;

            if(aramaIterator<0){
                aramaIterator=0;
            }

        }

            fclose(yazilandosya);

}

void deflate(char *konum , char*inDeflate , char *deflateOut){
	
	huffman(konum,inDeflate);
	lz77encoder(inDeflate,deflateOut);
}

int main() {
		
		int testSayi=3;
		
 char konum[3][30] ={
	  "input\\alphabet.txt",
	  "input\\bible.txt",
	  "input\\a.txt"
	  };

 char outputKonum[3][30] ={
	  "output\\alphabet.LZFG",
      "output\\bible.LZFG",
      "output\\a.LZFG"
      };
  char inputDEFLATE[3][30] ={
	  "inputDeflate\\alphabet.txt",
      "inputDeflate\\bible.txt",
      "inputDeflate\\a.txt"
      };      
 char outputDEFLATE[3][30] ={
	  "outputDeflate\\alphabet.def",
      "outputDeflate\\bible.def",
      "outputDeflate\\a.def"
      };     
      
		int i;
		printf("#####################################################################################\n");
		printf("######################### WELCOME TO THE COMPRESSION TEST ##############################\n");
		printf("#####################################################################################\n");
		
		
		printf("++++++++++++++++++++++++++++++LZ77 TEST START++++++++++++++++++++++++++++++++++++\n+\n");
		for(i=0; i<testSayi; i++){
			printf("+\tTest %d test is starting -> File : %s , Size :%2.2f Kb\n",i+1,konum[i],DosyaBoyut(konum[i]));
            lz77encoder(konum[i],outputKonum[i]);
            float oran = 100-DosyaBoyut(outputKonum[i])/DosyaBoyut(konum[i])*100;
            printf("+\tCompression Rate : | %% %f |",oran);
            oran<0 ? printf("(Padding Process)\n") : printf("\n");
            printf("+\tTest %d Result -> File : %s , Size :%2.2f Kb\n+\n",i+1,outputKonum[i],DosyaBoyut(outputKonum[i]));		
		}
        printf("++++++++++++++++++++++++++++++LZSS TEST OVER++++++++++++++++++++++++++++++++++++++\n");
        
	    printf("####################################################################################\n");
	    
		printf("++++++++++++++++++++++++++++++DEFLATE TEST START++++++++++++++++++++++++++++++++\n+\n"); 
		for(i=0; i<testSayi; i++){
			printf("+\tTest %d test is starting -> File : %s , Size :%2.2f Kb\n",i+1,konum[i],DosyaBoyut(konum[i]));
            deflate(konum[i],inputDEFLATE[i],outputDEFLATE[i]);
            float oran = 100-DosyaBoyut(outputDEFLATE[i])/DosyaBoyut(konum[i])*100; 
            printf("+\tCompression Rate : | %% %f |",oran);
            oran<0 ? printf(" (Padding Process)\n") : printf("\n");
            printf("+\tTest %d Result -> File : %s , Size :%2.2f Kb\n+\n",i+1,outputDEFLATE[i],DosyaBoyut(outputDEFLATE[i]));	
		}
		printf("++++++++++++++++++++++++++++++DEFLATE TEST OVER++++++++++++++++++++++++++++++++++\n"); 	    
	
	getch();
    return 1;
}



