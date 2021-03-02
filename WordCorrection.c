#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define M 997
#define MM 996
#define MAX 10000


		struct node{
			char word[50];		// hash_table ' a eklenecek kelime
			int key;			// 	kelimelerin horner fonksiyonundan c�kan degerlerini yerlestiriyorum
	};

		struct node hash_table[M];		// hash_table '�m� global olusturdum


	typedef struct {					// node kullandigimdan bun adini second koydum ve wrong_words table'� islemlerinde kullaniliyor
			char given_word[50];		// kullanici tarafindan yanlis girilmis kelime
			char advice_word[50];		// kullanicinin bu kelime yerine aslinda ne girmek istedigi
			int key;					// hash degeri
	} second;
	
	second wrong_words[M];				// yanlis girilen kelimelerin tutuldugu table
	
	
		////////////////////verilen kelimeyi kucuk harflere ceviren fonksiyon///////////////////////

		void stringToLower(char word[50]){		// documentten al�nan veya arat�lan kelimeyi kucuk harflere ceviren fonksiyonum
			int i;
				
			 for(i=0;i<=strlen(word);i++){			// eger harf buyuk harse 32 ekleyerek kucuk harfe donusturuyor
			      if(word[i]>=65&&word[i]<=90)
			         word[i]=word[i]+32;
			   }
		}

	
		////////////// HORNER METHOD //////////////
	
	int hornerMethod (char* word){
		int length = strlen(word);	//kelimenin boyutunu hesapl�yorum
		int hash=0;
		int R=31;					// kucuk asal sayi secimi
		int i;
		
		for(i=0;i<length;i++){
			if(word[i]>=65 && word[i]<97){
				hash=(R * hash + (word[i]+32))% M;		// eger buyuk harfse kucultuyorum
			}
			else{
				hash=(R * hash + word[i]) % M;	// kucuk harf ise ayn� kal�yor
			}		
		}
		return hash;	// c�kan index degerini donduruyorum
	}


		//////////////// DOUBLE HASH �LE YERLEST�RME ////////////////////////////
	
	void insertDoubleHash(struct node hash_table[],int hash_adres, char word[]){
			int adres = hash_adres;		// Horner methoddan gelen index degerini yerlestiriyorum
			int i=0,j;			// eger index dolu olursa hash2 donksiyonunu bu degiskenle carparak devam ediyorum yer aramaya
			int isEqual=1;		// hash_table da kontrol ettigim kelimeyle , yerlestirecegim kelime ayn� m� isleminin sonucu buraya yerlestirilir
			int quitControl=1;		// kelimenin hash_table da olup olmad�g�na gore hareket etmem icin gerekli kontrol
		
						
			
			while(i<M && hash_table[adres].key!=-1 && quitControl){
				
				isEqual = strcmp(hash_table[adres].word,word);		// eger isEqual 0 ise kelime bulundu
				
				if(isEqual==0){										// kelime onceden hash_table'a eklenmis
					quitControl=0;									// kelime bulundu daha aramayacag�m
				}
				if(quitControl){		// kelime bulunmad�ysa aramaya devam
					i++;				// hash2 ile carpacag�m degeri her seferinde 1 artt�rmam laz�m
					adres=( hash_adres + (i * (adres % MM) ) ) % M;			// hash2 den c�kan sonucu i ile carpip adrese ekliyorum. Double Hashing mantigi
				}			
			}
			
			if(quitControl==1){		// eger kelime bulunmad�ysa gir 
				if(i!=M){											// Eger Tablo Dolu degilse yeni bir yere yerlesirecek wordu			
					hash_table[adres].key=hash_adres;				// yeni kelimenin ozellikleri yerlestirilir
					strcpy(hash_table[adres].word,word);			
				}
				else{			// Eger Tablo Dolu ise sadece kelimeyi yazdir
					printf("DIKKAT  || Tablo Dolu Art�k Kelime Eklenemez! || DIKKAT \n");
					printf("\nEklenemeyen Kelime -> %s\n ",word);
				}
			}
		} 	
		
		
		//////////////// DOUBLE HASH �LE WRONG WORDS ////////////////////////////
	
	void insertWrongWords(second wrong_words[],int hash_adres, char word[],char advice_word[]){
			int adres = hash_adres;		// Horner methoddan gelen index degerini yerlestiriyorum
			int i=0,j;			// eger index dolu olursa hash2 donksiyonunu bu degiskenle carparak devam ediyorum yer aramaya
			int isEqual=1;		// hash_table da kontrol ettigim kelimeyle , yerlestirecegim kelime ayn� m� isleminin sonucu buraya yerlestirilir
			int quitControl=1;		// kelimenin hash_table da olup olmad�g�na gore hareket etmem icin gerekli kontrol
		
						
			
			while(i<M && wrong_words[adres].key!=-1 && quitControl){
				
				isEqual = strcmp(hash_table[adres].word,word);		// eger isEqual 0 ise kelime bulundu
				
				if(isEqual==0){										// kelime onceden hash_table'a eklenmis
					quitControl=0;									// kelime bulundu daha aramayacag�m
				}
				if(quitControl){		// kelime bulunmad�ysa aramaya devam
					i++;				// hash2 ile carpacag�m degeri her seferinde 1 artt�rmam laz�m
					adres=( hash_adres + (i * (adres % MM) ) ) % M;			// hash2 den c�kan sonucu i ile carpip adrese ekliyorum. Double Hashing mantigi
				}			
			}
			
			if(quitControl==1){		// eger kelime bulunmad�ysa gir 
				if(i!=M){											// Eger Tablo Dolu degilse yeni bir yere yerlesirecek wordu			
					wrong_words[adres].key=hash_adres;				// yeni kelimenin ozellikleri yerlestirilir
					strcpy(wrong_words[adres].given_word,word);
					strcpy(wrong_words[adres].advice_word,advice_word);		
				}
				else{			// Eger Tablo Dolu ise sadece kelimeyi yazdir
					printf("DIKKAT  || Tablo Dolu Art�k Kelime Eklenemez! || DIKKAT \n");
					printf("\nEklenemeyen Kelime -> %s\n ",word);
				}
			}
		} 	
	

			//////////////////// DOKUMANLARI DOSYADAN OKUMA /////////////////
		
	void readFile(){		
		int i=0,j;
		char *a;										// tum sat�rlar� strtok ile gezerken ' ' ile ay�rd�g�m elemanlar� tuttugum iki char array
		char *b;
		char buf[MAX];	
		char word[100];	
		int hash_adres;							// input al�rken kullanacag�m buffer		
		
		FILE * ptr;										// ulasacag�m�z dosya icin bir file pointer yarat�yoruz
		
		char document_name[50]="smallDictionary.txt";
		
		ptr=fopen(document_name,"r");					// dosyay� read modunda aciyoruz
		
		while(fgets(buf,MAX,ptr)){						// Dosyan�n sonuna kadar devam ediyor ve sat�r sat�r al�yor
				
				a=strtok(buf,"\n");						// Herhangi bir sorun c�kmamas� ac�s�ndan once sat�ra gore ay�r�yorum
				b=strtok(a," ");						// Ard�ndan elemanlar�m� ' ' isaretine gore ay�r�yorum
				while(b!=NULL){							// O sat�r�n sonuna kadar ' ' isaretiyle ay�rmaya devam et
					
					strcpy(word,b);	// okudugum dosyan�n elemanlar�n� tek tek once kucuk harfe gecirip sonra horner methodundan gecirip keyini buluyorum.
					stringToLower(word);				
					hash_adres= hornerMethod(word);	
					insertDoubleHash(hash_table,hash_adres,word);		// Daha sonra wordleri yerlestiriyorum		// ald�g�n eleman� degiskenler listesine koy
					b=strtok(NULL," ");					// bir sonraki elemana gec											
			}	
	}						
}


	void searchGivenWord(struct node hash_table[],int hash_adres,char searchWord[],second wrong_words[],char allSentence[]){
		
		int i=0,j;
		int adres = hash_adres;		// Horner methoddan gelen index degerini yerlestiriyorum
		int isEqual=1;				// hash_table da kontrol ettigim kelimeyle , arad�g�m kelime ayn� m� isleminin sonucu buraya yerlestirilir
		int quitControl=1;			// kelimenin bulunup bulunmad�g�n� belirtmek amac�yla kulland�g�m degisken
		char space[10];			// bosluk b�rakmak �c�n kulland�g�m degisken
		strcpy(space, " " );	// bosluk atamas�n� yap�yorum
		
		while(i<M && hash_table[adres].key!=-1 && quitControl){			
				isEqual = strcmp(hash_table[adres].word,searchWord);		// eger isEqual 0 ise kelime bulundu
				if(isEqual==0){			// kelime onceden hash_table'a eklenmis yani sadece document ekleyecegim
					quitControl=0;		// kelimenin bulundugunu belirtmek icin 0 yap�yorum
				}
				if(quitControl){		// double hashing mant�g�yla aramaya devam ediyorum
					i++;				// hash2 ile carpacag�m degeri her seferinde 1 artt�rmam laz�m
					adres=( hash_adres + (i * (adres % MM) ) ) % M;		// hash2 den c�kan sonucu i ile carpip adrese ekliyorum. Double Hashing mantigi
				}			
			}
			if(quitControl==1){			// quitControl 1 olarak kald�ysa kelime dosyada yok
				
				searchWrongWords(wrong_words,hash_adres,searchWord,allSentence);	
				
			}
			else{
				strncat(allSentence,searchWord,50);		// yeni gelen dokuman� row char arrayimin sonuna ekliyor
				strncat(allSentence,space, 5);							// kelimeler arasi bosluk birakiyor
			}						
	}

	/////////////////////DISTANCE FUNCTION ILE YAZDIRMA ///////////////////////////////////////
	int distanceFunc (char * word1,char * word2,int control){
	
	int i,j;					// Dongu degiskenleri
	int len1=strlen(word1);		// Ilk kelimenin boyutu (searchWord)
	int len2=strlen(word2);		// Ikinci kelimenin boyutu
	char c1;					// harf kars�last�rmas� icin
	char c2;					// harf kars�last�rmas� icin
	
    int matrix[len1 + 1][len2 + 1];		// matrisimi olustruyorum
    
    for (i = 0; i <= len1; i++) {		// Levenshtein Edit Distance yonteminde ilk sat�r ve sutun s�rayla artar
        matrix[i][0] = i;
    }
    for (i = 0; i <= len2; i++) {		// Levenshtein Edit Distance yonteminde ilk sat�r ve sutun s�rayla artar
        matrix[0][i] = i;
    }
    for (i = 1; i <= len1; i++) {		// 1. kelimenin harflerini gezecegim

        c1 = word1[i-1];				// 1. kelimenin harfini karsilastirmak icin aliyorum
       
	    for (j = 1; j <= len2; j++) {	// 2.kelimenin harflerini ilk kelimenin harfleriyle karsilastirmak icin geziyorum

            c2 = word2[j-1];			// 2. kelimenin siradaki harfini aliyorum	
            
			if (c1 == c2) {
                matrix[i][j] = matrix[i-1][j-1];	// eger iki harf esitse distance artmaz ve matriste o bolgeye sol ustundeki deger yazilir
            }
            else {
                int delete1;		// delete islemi hesabi icin
                int insert;			// insert islemi hesabi icin
                int different;		// farkl�l�k islemi hesabi icin
                int minimum;		// hangisinin minimum degere ulastigini anlamak icin

                delete1 = matrix[i-1][j] + 1;	// Eger delete islemi gerceklesmisse soldaki kurudaki degeri aliriz ve harfler farkli oldugundan 1 artim yapariz
                insert = matrix[i][j-1] + 1;	// Eger insert islemi gerceklesmisse ustteki kutudaki degeri aliriz ve harfler farkli oldugundan 1 artim yapariz
                different = matrix[i-1][j-1] + 1;	// Eger iki harfte bos degilse fakat harfler farkliysa sol ustteki deger alinir ve harfler farkli oldugundan 1 artim yapariz.
                minimum = delete1;				// delete islemini minimum olarak initialize ettim ama fark etmezdi digerlerini de yapabilirdim nasil olsa kontrol ediyorum
                
				if (insert < minimum) {			// eger insert islemi minimum islemden daha az bir distance yaratabiliyorsa minimum art�k insert'un sonucudur
                    minimum = insert;
                }
                if (different < minimum) {		// eger different islemi minimum islemden daha az bir distance yaratabiliyorsa minimum art�k different'�n sonucudur
                    minimum = different;
                }
                matrix[i][j] = minimum;			// finalde en sag alttaki kutu icindeki deger benim minimum degerimdir
            }
        }
    }
    if(matrix[len1][len2]==1 && control==1){
    	printf("(D1): %s\n",word2);
	}
	else if(matrix[len1][len2]==2 && control==2){
		printf("(D2): %s\n",word2);
	}
	return matrix[len1][len2];
}
	
	//////////////////////KELIMEYI WRONG WORDS TABLE'INDA ARAMA YOKSA OLUSTURMA FONKSIYONU /////////////////////////
	
	void searchWrongWords(second wrong_words[],int hash_adres,char searchWord[],char allSentence[]){
			
			int i=0,j,k;
			int adres = hash_adres;		// Horner methoddan gelen index degerini yerlestiriyorum
			int isEqual=1;				// hash_table da kontrol ettigim kelimeyle , arad�g�m kelime ayn� m� isleminin sonucu buraya yerlestirilir
			int quitControl=1;			// kelimenin bulunup bulunmad�g�n� belirtmek amac�yla kulland�g�m degisken
			int onControl;				// distanceFunc tan gelen distance'� ilk buna at�yorum
			int controlDistance=1;		// distance controlu yap�yorum
			int minControl=2;			// eger 1 distance yoksa otomatik 2 olarak devam edecek
			char newWord[50]; 			// kulanicinin yanlis girdigi kelime yerine sonradan girdigi kelime
			char space[10];				// bosluk b�rakmak �c�n kulland�g�m degiskens
			strcpy(space, " " );		// bosluk atamas�n� yap�yorum
			char bos[5];				// gets isleminde sorun cikmamasi icin
			
		
			while(i<M && wrong_words[adres].key!=-1 && quitControl){			//wrong_adres table'�nda daha onceden bu kelime eklenmis mi kontrolu			
				isEqual = strcmp(wrong_words[adres].given_word,searchWord);		// eger isEqual 0 ise kelime bulundu
				if(isEqual==0){			// kelime onceden hash_table'a eklenmis 
					printf("\n%s yerine bunu mu demek istediniz: %s\nGirmek istediginiz kelime: ",searchWord,wrong_words[adres].advice_word);
					scanf("%s",&newWord);		// kullanicidan oneri esliginde yeni kelime girisi
					quitControl=0;				// kelimenin bulundugunu belirtmek icin 0 yap�yorum
					strncat(allSentence,wrong_words[adres].advice_word,50);		// yeni gelen dokuman� allSentence char arrayimin sonuna ekliyor
					strncat(allSentence,space, 5);								// kelimeler arasi bosluk birakiyor
				}
				if(quitControl){		// double hashing mant�g�yla aramaya devam ediyorum
					i++;				// hash2 ile carpacag�m degeri her seferinde 1 artt�rmam laz�m
					adres=( hash_adres + (i * (adres % MM) ) ) % M;		// hash2 den c�kan sonucu i ile carpip adrese ekliyorum. Double Hashing mantigi
				}								
			}
			if(quitControl==1){			// quitControl 1 olarak kald�ysa kelime wrong_words de yok olusturmamiz lazim
				adres=hash_adres;
				i=0;
				printf("\n%s yerine bunlardan birini mi demek istediniz: \n",searchWord);
				
				while(i<M){		// distance'� 1 olan var m� dongusu	
				if(hash_table[i].key!=-1)	{								
						onControl=distanceFunc(searchWord,hash_table[i].word,controlDistance);			// distanceFunc'a gonderip gelen mesafenin degerini donduruyor
																		
					if(onControl==1){		// Eger kelimeye olan uzaklik 1 ise girer
						minControl=1;		// Finalde hangi uzakl�ktan alindigini anlamak icin
					}				
				}											
					i++;				// tum diziyi gezmem gerekiyor cunku 1 uzakl�kta bir kelime bulunca c�km�yorum tek tek gezebilirim								
			}
		
			
			if(minControl==2){		// eger 1 degerinde distance yoksa zorunlu 2 degerinde distancelar� arayacag�m
				adres=hash_adres;
				i=0;
				controlDistance=2;		// fonksiyonun icine 2 degerini gonderiyorum ki 2' li olanlari ekrana bastirsin
				
				while(i<M){		// distance'� 2 olan var m� dongusu		
				if(hash_table[i].key!=-1)	{					
							onControl=distanceFunc(searchWord,hash_table[i].word,controlDistance);		// distanceFunc'a gonderip gelen mesafenin degerini donduruyor.
					
				
					if(onControl==2){		// Eger kelimeye olan uzaklik 2 ise girer
						minControl=2;		// Finalde hangi uzakl�ktan alindigini anlamak icin						
					}					
				}				
					i++;		// tum diziyi gezmem gerekiyor cunku 2 uzakl�kta bir kelime bulunca c�km�yorum tek tek gezebilirim													
			}		
		}
			
				printf("\n%s yerine girmek istediginiz kelimeyi yaziniz: ",searchWord);
				scanf("%s",&newWord);					// yanlis girilen kelime yerine girilen kelime
				gets(bos);								// maindeki getsi etkilememek icin
				strncat(allSentence,newWord,50);						// yeni gelen dokuman� allSentence char arrayimin sonuna ekliyor
				strncat(allSentence,space, 5);							// kelimeler arasi bosluk birakiyor
							
				insertWrongWords(wrong_words,hash_adres,searchWord,newWord);		// yanlis girilen kelimeleri wrong_words table'�na yerlestiriyorum					
		}		
	}


int main(){
	char allSentence[2000];			// cumlenin degisimlerden sonra son halini icine koyacag�m char array
	char sentence[200];				// kullan�c�n�n girecegi cumle
	char *b;						// kelimeleri strtok ile ay�r�rken icinde tutacag�m char array
	int i =0;
	char searchWord[50];			// girdigim cumledeki kelimeleri tek tek ararken kullanacag�m char array
	int hash_adres;					// kelimenin horner methodu sornas� c�kan degeri
	int secim;						// kullan�c�n�n hangi islemi yapmak istedigini tutan degisken
	char bos[5];					// getsi kullan�rken sorun c�kmas�n diye
	
	for(i=0;i<M;i++){
		hash_table[i].key=-1 ;				// keyleri -1 den baslat�yorum NULL kontrolu yapmak gibi kullancag�m
		wrong_words[i].key=-1;				// keyleri -1 den baslat�yorum NULL kontrolu yapmak gibi kullancag�m
	}
	
	readFile();		// dosyadan okuyup yerlestiriyorum
	
		printf("Cumle girmek icin 1, Cikmak icin -1 girin: ");
		scanf("%d",&secim);
		gets(bos);			// Asag�da gets isleminde sorun yasand�g� icin bu ara islemi yapmam gerekiyor
	
	while(secim!=-1){		// kullan�c� c�kmad�g� surece devam et
		
		strcpy(allSentence,"");
		printf("Cumlenizi giriniz: ");
		gets(sentence);
		
			
		b=strtok(sentence," ");
		
		while(b!=NULL){							// O sat�r�n sonuna kadar ' ' isaretiyle ay�rmaya devam et
						strcpy(searchWord,b);
						stringToLower(searchWord);								// aramak istedigim kelimeyi once kucuk harflere donusturuyorum
						hash_adres= hornerMethod(searchWord);					// ard�ndan horner moethoduna gonderip hash degerini al�yorum
						searchGivenWord(hash_table,hash_adres,searchWord,wrong_words,allSentence);		// hash degerini ald�ktan sonra search fonksiyonuma gonderiyorum
						
						b=strtok(NULL," ");					// bir sonraki kelimeye gec											
				}	
		
		printf("\nCumlenin son hali:\t%s\n\n",allSentence);
		
		printf("Cumle girmek icin 1, Cikmak icin -1 girin: ");
		scanf("%d",&secim);
		gets(bos);
	}
	
	return 0;
}
