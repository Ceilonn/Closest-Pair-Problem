#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct nokta{
    int x; //x koordinati
    int y; // y koordinati
}nokta;

int line_count(char[]);
int partitionX(nokta*, int, int);
int partitionY(nokta*, int, int);
void quickSortX(nokta*, int, int);
void quickSortY(nokta*, int, int);
void swap(nokta*,nokta*);
float enYakin (nokta*, int);
float enYakinDallanma(nokta*, nokta*, int);
float min (float, float);
float uzaklik (nokta, nokta);
float bruteForce(nokta*,int);

int main(){
    int i,j; // dongu degiskenleri
    FILE *fp; // file pointer
    int satir_sayisi;
    float minUzaklik;
    satir_sayisi = line_count("sample.txt");
    nokta *noktalar;
	noktalar = (nokta*)malloc(satir_sayisi*sizeof(nokta));
    
    fp= fopen("sample.txt","r");
    
    for(i=0;i<satir_sayisi;i++){
    	fscanf(fp,"%d %d",&noktalar[i].x,&noktalar[i].y);
	}
	
	printf("sample.txt dosyasindaki noktalar okundu.\n");
	
	fclose(fp);
	
	minUzaklik = enYakin(noktalar, satir_sayisi);
	
	printf("Verilen noktalar arasindaki en yakin 2 nokta bulundu.\n");
	printf("En yakin uzaklik: %f\n",minUzaklik);
	 
}

/* 2 nokta arasindaki uzakligi float cinsinden donen fonksiyon */
float uzaklik (nokta a, nokta b){
	float mesafe;
	mesafe = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return mesafe;
}

/* verilen arraydeki noktalar arasindaki en kisa mesafeyi brute force
   yontemle cozen algoritma */
float bruteForce(nokta* noktalar, int n){
	int i,j;
	float min = FLT_MAX;
	
	for(i=0;i<n;i++){
		for(j=i+1;j<n;++j){
			if(uzaklik(noktalar[i],noktalar[j])<min){
				min = uzaklik(noktalar[i],noktalar[j]);
			}
		}
	}
	return min;
}

/* min float degeri donen fonksiyon */
float min (float a, float b){
	if(a<b){
		return a;
	}
	else{
		return b;
	}
}

/* 2'ye ayirdigimiz kisimda d mesafesinden yakin olan noktalar kendi icinde
   birbirine belirledigimiz min mesafesinden daha yakin olabilirler. Onlar icin
   ayri bir hesaplama yapmamiz gerek. Yapacagimiz hesaplama brute force yontem
   fakat bu belirlenen alanda en fazla 6 noktayi kontrol etmemiz yeterli. */
float dortgenEnYakin(nokta *noktalar, int n, float d){
	
	int i,j;
	float min = d;
	
	for(i = 0; i<n; i++){
		for(j = i+1; j< n && (noktalar[j].y - noktalar[i].y)<min; j++){
			if(uzaklik(noktalar[i], noktalar[j])<min){
				min = uzaklik(noktalar[i], noktalar[j]);
			}
		}
	}
	
	return min;
}

/* dallanmayi yaptigimiz ana fonksiyon. En kisa mesafeyi bulur. noktaX, X'e göre siralanmis
   noktalar arrayi; noktaY, Y'ye gore siralanmis noktalar arrayi. */
float enYakinDallanma(nokta* noktaX, nokta* noktaY, int n){
	int i,j;
	// eger 2 veya 3 nokta varsa bruteForce cozum ideali
	if(n<=3){
		return bruteForce(noktaX,n);
	}
	
	//orta noktayi bulma
	int mid = n/2;
	nokta ortaNokta = noktaX[mid];
	
	/* cektigimiz cizgiden itibaren 2 farkli bolgede noktalari inceleyecegimiz icin
	   2 farkli subarray */
	nokta *noktaYL = (nokta*)malloc(mid*sizeof(nokta));
	nokta *noktaYR = (nokta*)malloc((n-mid)*sizeof(nokta));
	int li=0, ri=0; //sag ve sol indexler
	
	
	for(i = 0;i<n;i++){
		if(noktaY[i].x<ortaNokta.x || (noktaY[i].x == ortaNokta.x && noktaY[i].y < ortaNokta.y) && li<mid){
			noktaYL[li] = noktaY[i];
			li++;
		}
		else{
			noktaYR[ri] = noktaY[i];
			ri++;
		}
	}
	
	/* ortadan cektigimiz cizginin bir noktadan gectigini varsayarak
	   2 taraf icin de en kisa mesafeyi ariyoruz */
	float dl = enYakinDallanma(noktaX,noktaYL,mid);
	float dr = enYakinDallanma(noktaX+mid, noktaYR, n-mid);
	
	float d = min(dl,dr);
	
	/* cektigimiz noktadaki dikdortgendeki noktalarda sag ve sol karsilastirmasi yapmiyoruz
	   onu yapacagimiz bir nokta arrayi olusturup doldurgumuz yer */
	nokta *dortgenNokta = (nokta*)malloc(n*sizeof(nokta));
	j = 0;
	
	for(i = 0; i<n; i++){
		if(abs(noktaY[i].x-ortaNokta.x)<d){
			dortgenNokta[j]= noktaY[i];
			j++;
		}
	}
	
	return dortgenEnYakin(dortgenNokta, j, d);
}

/* en kısa mesafeyi bulan ana foksiyon. Dallanma başka bir fonksiyon üzerinde yapılıyor */
float enYakin (nokta *noktalar, int n){
	int i, j;
	
	// X ve Y tabanli 2 nokta arrayi
	nokta *noktaX = (nokta*)malloc(n*sizeof(nokta));
	nokta *noktaY = (nokta*)malloc(n*sizeof(nokta));
	
	for(i=0;i<n;i++){
		noktaX[i] = noktalar[i];
		noktaY[i] = noktalar[i];
	}
	
	/* x ve y'ye göre dizilerin siralanmasi */
	quickSortX(noktaX, 0, n-1);
	quickSortY(noktaY, 0, n-1);
	
	return enYakinDallanma(noktaX, noktaY, n);
}

/* x referanslı partition */
int partitionX(nokta* noktalar, int low, int high){
	int i, j;
	int pivot = noktalar[high].x;
	i = low -1;
	
	for(j=low;j<=high-1;j++){
		if(noktalar[j].x<pivot){
			i++;
			swap(&noktalar[i],&noktalar[j]);
		}
	}
	swap(&noktalar[i+1],&noktalar[high]);
	return (i+1);
}

/* y referanslı partition */
int partitionY(nokta* noktalar, int low, int high){
	int i, j;
	int pivot = noktalar[high].y;
	i = low -1;
	
	for(j=low;j<=high-1;j++){
		if(noktalar[j].y<pivot){
			i++;
			swap(&noktalar[i],&noktalar[j]);
		}
	}
	swap(&noktalar[i+1],&noktalar[high]);
	return (i+1);
}

/* x referanslı quicksort */
void quickSortX(nokta* noktalar, int low, int high){
	
	if(low<high){
		int pi = partitionX(noktalar, low, high);
		
		quickSortX(noktalar, low, pi-1);
		quickSortX(noktalar, pi+1, high);
	}
	return;
}

/* y referanslı quicksort */
void quickSortY(nokta* noktalar, int low, int high){
	
	if(low<high){
		int pi = partitionY(noktalar, low, high);
		quickSortX(noktalar, low, pi-1);
		quickSortX(noktalar, pi+1, high);
	}
	return;
}

/* satir sayisini bulan fonksiyon */
int line_count(char *dosya){
	FILE *fp;
	int count = 0;
	char tmp;
	
	fp = fopen(dosya,"r");
	
	if (fp == NULL) 
    { 
        printf("HATA!! Dosya acilamadi.\n %s", dosya);
		return 0; 
    }
	else{
		for(tmp= getc(fp); tmp!= EOF; tmp= getc(fp)){
			if(tmp=='\n'){
				count++;
			}
		}
	}
	fclose(fp); 
	return count;
}

/* swap fonksiyonu */
void swap (nokta *a, nokta *b){
	nokta t = *a;
	*a = *b;
	*b = t;
	return;
}

