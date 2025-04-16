#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to store each country's data
typedef struct CountryData {
    char country[100];
    int confirmed;
    int deaths;
    int recovered;
    float death_rate;
    float recovery_rate;
    struct CountryData* next;
} CountryData;

// Structure for the Heap
typedef struct Heap {
    CountryData** data;
    int size;
    int capacity;
    char type[20];
} Heap;

CountryData* create_node(const char* country, int confirmed, int deaths, int recovered) {
    CountryData* new_node = (CountryData*)malloc(sizeof(CountryData));
    if (!new_node) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    strncpy(new_node->country, country, sizeof(new_node->country) - 1);
    new_node->confirmed = confirmed;
    new_node->deaths = deaths;
    new_node->recovered = recovered;
    
    new_node->death_rate = (confirmed > 0) ? (deaths * 100.0f) / confirmed : 0;
    new_node->recovery_rate = (confirmed > 0) ? (recovered * 100.0f) / confirmed : 0;
    
    new_node->next = NULL;
    return new_node;
}

void free_list(CountryData* head) {
    while (head) {
        CountryData* temp = head;
        head = head->next;
        free(temp);
    }
}

Heap* create_heap(int capacity, const char* type) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (!heap) {
        perror("Heap allocation error");
        exit(EXIT_FAILURE);
    }

    heap->data = (CountryData**)malloc(capacity * sizeof(CountryData*));
    if (!heap->data) {
        free(heap);
        perror("Heap array allocation error");
        exit(EXIT_FAILURE);
    }

    heap->size = 0;
    heap->capacity = capacity;
    strncpy(heap->type, type, sizeof(heap->type) - 1);
    return heap;
}

void heapify(Heap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size) {
        int left_value, largest_value;
        
        if (strcmp(heap->type, "confirmed") == 0) {
            left_value = heap->data[left]->confirmed;
            largest_value = heap->data[largest]->confirmed;
        } else if (strcmp(heap->type, "deaths") == 0) {
            left_value = heap->data[left]->deaths;
            largest_value = heap->data[largest]->deaths;
        } else {
            left_value = heap->data[left]->recovered;
            largest_value = heap->data[largest]->recovered;
        }

        if (left_value > largest_value) {
            largest = left;
        }
    }

    if (right < heap->size) {
        int right_value, largest_value;
        
        if (strcmp(heap->type, "confirmed") == 0) {
            right_value = heap->data[right]->confirmed;
            largest_value = heap->data[largest]->confirmed;
        } else if (strcmp(heap->type, "deaths") == 0) {
            right_value = heap->data[right]->deaths;
            largest_value = heap->data[largest]->deaths;
        } else {
            right_value = heap->data[right]->recovered;
            largest_value = heap->data[largest]->recovered;
        }

        if (right_value > largest_value) {
            largest = right;
        }
    }

    if (largest != index) {
        CountryData* temp = heap->data[index];
        heap->data[index] = heap->data[largest];
        heap->data[largest] = temp;
        heapify(heap, largest);
    }
}

void load_data(CountryData** head, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        if (!token) continue;

        char country[100];
        strncpy(country, token, sizeof(country) - 1);

        // Skip unused columns
        for (int i = 0; i < 2; i++) strtok(NULL, ",");

        int confirmed = atoi(strtok(NULL, ","));
        int deaths = atoi(strtok(NULL, ","));
        int recovered = atoi(strtok(NULL, ","));

        CountryData* new_node = create_node(country, confirmed, deaths, recovered);
        new_node->next = *head;
        *head = new_node;
    }

    fclose(file);
}

void show_menu() {
    printf("\n=== MENU OPTIONS ===\n");
    printf("1. Countries with most confirmed cases\n");
    printf("2. Countries with most deaths\n");
    printf("3. Countries with most recoveries\n");
    printf("4. Exit\n");
    printf("Select an option: ");
}

void display_results(Heap* heap, int count) {
    printf("\n=== RESULTS ===\n");
    printf("%-30s %-12s %-12s %-12s %-12s %-12s\n", 
           "Country", "Confirmed", "Deaths", "Recovered", 
           "DeathRate%%", "Recovery%%");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count && heap->size > 0; i++) {
        CountryData* country = heap->data[0];
        printf("%-30s %-12d %-12d %-12d %-12.2f %-12.2f\n", 
               country->country, country->confirmed, country->deaths, 
               country->recovered, country->death_rate, 
               country->recovery_rate);

        heap->data[0] = heap->data[--heap->size];
        heapify(heap, 0);
    }
}

int main() {
    CountryData* data = NULL;
    printf("Loading data from country_wise_latest.csv...\n");
    load_data(&data, "country_wise_latest.csv");

    if (!data) {
        fprintf(stderr, "Error: Failed to load data.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        show_menu();
        int option;
        scanf("%d", &option);

        if (option == 4) break;

        const char* type = NULL;
        switch (option) {
            case 1: type = "confirmed"; break;
            case 2: type = "deaths"; break;
            case 3: type = "recovered"; break;
            default:
                printf("Invalid option!\n");
                continue;
        }

        printf("How many countries do you want to display? ");
        int count;
        scanf("%d", &count);

        Heap* heap = create_heap(200, type);
        for (CountryData* current = data; current; current = current->next) {
            if (heap->size < heap->capacity) {
                heap->data[heap->size++] = current;
            }
        }

        for (int i = heap->size / 2 - 1; i >= 0; i--) {
            heapify(heap, i);
        }

        display_results(heap, count);
        
        free(heap->data);
        free(heap);
    }

    free_list(data);
    printf("Program ended.\n");
    return EXIT_SUCCESS;
}
