#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Prepared by Eren Durucan

// DESCRIPTION: This program simulates various page replacement algorithms with a list of N page accesses.
// COMPILE: gcc -o pr pr.c
// RUN: ./pr 3

// Max sayfa Numarasi
#define MAX_PAGE_NUM 100
#define MAX_LINE_NUM 1000

// Global bellek boyutu
int memory_size;

// Sayfa yerine koyma algoritmalarini simule eden fonksiyon prototipleri
int FIFO(int pages[], int size);
int LRU(int pages[], int size);
int LFU(int pages[], int size);
double RandomReplacement(int pages[], int size);
double WeightedRandom(int pages[], int size);

/*
    Algortima aciklamalari:
        FIFO (First-In First-Out):      Ilk gelen ilk cikar mantigina gore calisir.
        LRU (Least Recently Used):      En uzun suredir kullanilmayan sayfayi degistirir.
        LFU (Least Frequently Used):    En az kullanilan sayfayi degistirir. Esitlik durumunda LRU mantigi uygulanir.
        Random:                         Rastgele bir sayfayi degistirir.
        Weighted Random:                Erisim sikligina gore agirliklandirilmis rastgele secim yapar.
*/

// Yardimci fonksiyonlar
int findLRU(int time[], int n);
int findLFU(int count[], int time[], int n);

int main(int argc, char *argv[])
{
    FILE *file;                        // Dosya isaretcisi
    char line[MAX_LINE_NUM];           // Satiri okumak icin yeterince buyuk bir dizi
    int pages[MAX_PAGE_NUM];           // Sayfa erisim listesi
    int page_faults;                   // Sayfa hatalari
    double weighted_random_faults = 0; // Weighted random algoritmasi icin sayfa hatalari
    double random_faults = 0;          // Random algoritmasi icin sayfa hatalari
    int size;                          // Erisim listesi boyutu

    // Bellek boyutunu argumandan oku
    if (argc != 2)
    {
        // printf("Kullanim: %s <bellek boyutu>\n", argv[0]);
        return 1;
    }
    memory_size = atoi(argv[1]);

    // Dosyayi ac
    file = fopen("input.txt", "r");
    if (file == NULL)
    {
        // perror("input.txt dosyasi acilamadi");
        return 1;
    }

    srand(time(NULL)); // Rastgele sayi uretmek icin seed ayarla

    // Dosyadan satir satir oku ve her satir icin algoritmalari calistir
    while (fgets(line, sizeof(line), file))
    {
        size = 0; // Her satir icin boyutu sifirla
        char *token = strtok(line, ", ");

        // Satirdan sayfa numaralarini oku
        while (token != NULL)
        {
            pages[size++] = atoi(token);
            token = strtok(NULL, ", ");
        }

        printf("Sayfa Erisim Listesi: ");
        for (int i = 0; i < size; i++)
        {
            printf("%d ", pages[i]);
        }
        printf("\n");

        // Her algoritmayi calistir ve page fault sayilarini bas
        page_faults = FIFO(pages, size);
        printf("FIFO: %d\n", page_faults);

        page_faults = LRU(pages, size);
        printf("LRU: %d\n", page_faults);

        page_faults = LFU(pages, size);
        printf("LFU: %d\n", page_faults);

        random_faults = 0;
        for (int iter = 0; iter < 1000; ++iter)
        {
            random_faults += RandomReplacement(pages, size);
        }
        printf("Random: %.6f\n", random_faults / 1000);

        weighted_random_faults = 0;
        for (int iter = 0; iter < 1000; ++iter)
        {
            weighted_random_faults += WeightedRandom(pages, size);
        }
        printf("Weighted random: %.6f\n", weighted_random_faults / 1000);

        // Satir sonunu temizle
        memset(line, 0, sizeof(line));
    }

    // Dosyayi kapat
    fclose(file);
    return 0;
}

int FIFO(int pages[], int size)
{
    int frame[memory_size], frame_index = 0;
    int page_faults = 0, i, j;
    int flag1, flag2;

    // Frame'leri sifirla
    for (i = 0; i < memory_size; i++)
    {
        frame[i] = -1;
    }

    // Sayfa erisim listesini gez
    for (i = 0; i < size; i++)
    {
        flag1 = flag2 = 0;

        // Sayfa zaten frame'de mi?
        for (j = 0; j < memory_size; j++)
        {
            if (frame[j] == pages[i])
            {
                flag1 = flag2 = 1;
                break;
            }
        }

        // Sayfa frame'de degilse
        if (flag1 == 0)
        {
            for (j = 0; j < memory_size; j++)
            {
                if (frame[j] == -1)
                {                        // Bos frame varsa
                    page_faults++;       // Sayfa hatasi sayisini arttir
                    frame[j] = pages[i]; // Sayfayi frame'e yerlestir
                    flag2 = 1;           // Sayfa kullanildi
                    break;               // Donguden cik
                }
            }
        }
        // Sayfa frame'de degilse ve bos frame yoksa
        if (flag2 == 0)
        {
            page_faults++;                                 // Sayfa hatasi sayisini arttir
            frame[frame_index] = pages[i];                 // Sayfayi frame'e yerlestir
            frame_index = (frame_index + 1) % memory_size; // Frame indexini guncelle
        }
    }

    return page_faults; // Sayfa hatalarini dondur
}

// LRU algoritmasinin simulasyonu
int LRU(int pages[], int size)
{
    int frame[memory_size], frame_time[memory_size];
    int page_faults = 0, frame_index = 0;
    int i, j, pos, flag1, flag2, least_time;

    // Frame'leri sifirla
    for (i = 0; i < memory_size; i++)
    {
        frame[i] = -1;
    }

    // Sayfa erisim listesini gez
    for (i = 0; i < size; i++)
    {
        flag1 = flag2 = 0;

        // Sayfa zaten frame'de mi?
        for (j = 0; j < memory_size; j++)
        {
            if (frame[j] == pages[i])
            {
                frame_time[j] = i; // Sayfanin ilk kullanilma zamanini guncelle
                flag1 = flag2 = 1; // Sayfa frame'de ve kullanildi
                break;
            }
        }

        // Sayfa frame'de degilse
        if (flag1 == 0)
        {
            for (j = 0; j < memory_size; j++)
            {
                if (frame[j] == -1)
                {                        // Bos frame varsa
                    page_faults++;       // Sayfa hatasi sayisini arttir
                    frame[j] = pages[i]; // Sayfayi frame'e yerlestir
                    frame_time[j] = i;   // Sayfanin ilk kullanilma zamanini guncelle
                    flag2 = 1;           // Sayfa kullanildi
                    break;
                }
            }
        }

        // Sayfa frame'de degilse ve bos frame yoksa
        if (flag2 == 0)
        {
            pos = findLRU(frame_time, memory_size); // En az kullanilan sayfanin indexini bul
            frame[pos] = pages[i];                  // Sayfayi frame'e yerlestir
            frame_time[pos] = i;                    // Sayfanin ilk kullanilma zamanini guncelle
            page_faults++;                          // Sayfa hatasi sayisini arttir
        }
    }

    return page_faults; // Sayfa hatalarini dondur
}

// LRU algoritmasinda en az kullanilan sayfayi bul
int findLRU(int time[], int n)
{
    int i, minimum = time[0], pos = 0;

    // Ilk kullanilma zamanlarini gez
    for (i = 1; i < n; i++)
    {
        if (time[i] < minimum)
        {                      // Ilk kullanilma zamani min'den kucukse
            minimum = time[i]; // Min'i guncelle
            pos = i;           // Min'in indexini guncelle
        }
    }

    return pos; // En az kullanilan sayfanin indexini dondur
}

int LFU(int pages[], int size)
{
    int frame[memory_size], page_count[memory_size], page_time[memory_size];
    int page_faults = 0;
    int i, j, pos;

    // Frame'leri ve sayaci sifirla
    for (i = 0; i < memory_size; i++)
    {
        frame[i] = -1;
        page_count[i] = 0;
        page_time[i] = -1; // Ilk kullanilma zamani henuz yok
    }

    // Sayfa erisim listesini gez
    for (i = 0; i < size; i++)
    {
        pos = -1;
        // Sayfa zaten frame'de mi?
        for (j = 0; j < memory_size; j++)
        {
            if (frame[j] == pages[i])
            {
                pos = j;         // Sayfanin indexini kaydet
                page_count[j]++; // Sayfanin frekansini arttir
                break;           // Donguden cik
            }
        }
        // Sayfa frame'de degilse
        if (pos == -1)
        {
            // En az kullanilan sayfanin indexini bul
            pos = findLFU(page_count, page_time, memory_size);
            frame[pos] = pages[i]; // Sayfayi frame'e yerlestir
            page_faults++;         // Sayfa hatasi sayisini arttir
            page_count[pos] = 1;   // Sayfanin frekansini 1 olarak ayarla
            page_time[pos] = i;    // Sayfanin ilk kullanilma zamanini guncelle
        }
    }

    return page_faults; // Sayfa hatalarini dondur
}

int findLFU(int count[], int time[], int n)
{
    int i, min = count[0], pos = 0;
    int first_used_time = time[0];

    // Sayfa frekanslarini gez
    for (i = 1; i < n; i++)
    {
        // Sayfa frekansi min'den kucukse veya esitse ve ilk kullanilma zamani min'den kucukse
        if (count[i] < min || (count[i] == min && time[i] < first_used_time))
        {
            min = count[i];            // Min'i guncelle
            pos = i;                   // Min'in indexini guncelle
            first_used_time = time[i]; // Ilk kullanilma zamani min'den kucukse guncelle
        }
    }

    return pos; // En az kullanilan sayfanin indexini dondur
}

// Random algoritmasinin simulasyonu
double RandomReplacement(int pages[], int size)
{
    int frame[memory_size];
    int page_faults = 0, i, j, found, random_frame, pageNum = 0;

    // Frame'leri sifirla
    for (i = 0; i < memory_size; ++i)
    {
        frame[i] = -1;
    }

    for (i = 0; i < size; ++i)
    {
        found = 0;
        // Sayfa zaten frame'de mi kontrol et
        for (j = 0; j < pageNum; ++j)
        {
            if (frame[j] == pages[i])
            {
                found = 1;
                break;
            }
        }

        if (!found)                                     // Sayfa frame'de degilse
        {
            page_faults++;

            if (pageNum < memory_size)                  // Frame'ler dolmamissa
            {
                frame[pageNum] = pages[i];              // Sayfayi frame'e yerlestir
                pageNum++;                              // Frame sayisini arttir

            }
            else // Frame'ler dolmussa
            {
                random_frame = rand() % memory_size;    // Random bir frame sec
                frame[random_frame] = pages[i];         // Sayfayi secilen frame'e yerlestir
            }
        }
    }

    return page_faults;
}

// Weighted Random algoritmasinin simulasyonu
double WeightedRandom(int pages[], int size) {
    int frame[memory_size];
    int page_frequency[MAX_PAGE_NUM] = {0};
    int page_faults = 0, i, j, found, weighted_index;
    double weights[memory_size], total_weight;

    // Frame'leri sifirla
    for (i = 0; i < memory_size; ++i) {
        frame[i] = -1;
    }

    // Sayfa frekanslarini hesapla
    for (i = 0; i < size; ++i) {
        page_frequency[pages[i]]++;
    }

    for (i = 0; i < size; ++i) {
        found = 0;
        // Sayfa zaten frame'de mi kontrol et
        for (j = 0; j < memory_size; ++j) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {                               // Sayfa frame'de degilse
            page_faults++;

            if (page_faults <= memory_size) {       // Frame'ler dolmamissa
                frame[page_faults - 1] = pages[i];  // Sayfayi frame'e yerlestir
            } else {                                // Frame'ler dolmussa
                // Toplam agirligi hesapla
                total_weight = 0;
                for (j = 0; j < memory_size; ++j) {
                    weights[j] = 1.0 / (double)page_frequency[frame[j]];
                    total_weight += weights[j];
                }

                // Agirliklandirilmis rastgele index sec
                double random_value = ((double)rand() / (double)RAND_MAX) * total_weight; // 0-1 arasi rastgele sayi uret
                for (j = 0, weighted_index = -1; j < memory_size && weighted_index == -1; ++j) {
                    if (random_value <= weights[j]) {       // Secilen frame'i bul
                        weighted_index = j;
                    } else {                                // Secilen frame'i gec
                        random_value -= weights[j];
                    }
                }

                // Sayfayi secilen frame'e yerlestir
                frame[weighted_index] = pages[i];
            }
        }
    }

    return page_faults;
}