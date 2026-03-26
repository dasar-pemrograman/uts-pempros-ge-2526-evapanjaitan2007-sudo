//12S25047-Eva Dahlia Panjaitan


#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_LOGS 500

// --- Struktur Data ---
typedef struct {
    char id[50];
    char name[100];
    int quantity;
    double price;

typedef struct {
    int log_id;
    char item_id[50];
    char type[20];
    int quantity;
} AuditLog;

// --- Variabel Global ---
Item inventory[MAX_ITEMS];
AuditLog logs[MAX_LOGS];
int item_count = 0;
int log_count = 0;
int global_log_id = 1;

// --- Fungsi Pendukung ---
int find_item_index(char *id) {                                                                                                                                            
    for (int i = 0; i < item_count; i++) {
        if (strcmp(inventory[i].id, id) == 0) return i;
    }
    return -1;
}

void add_log(char *item_id, char *type, int qty) {
    if (log_count < MAX_LOGS) {
        logs[log_count].log_id = global_log_id++;
        strcpy(logs[log_count].item_id, item_id);
        strcpy(logs[log_count].type, type);
        logs[log_count].quantity = qty;
        log_count++;
    }
}

// --- Program Utama ---
int main() {
    char input[256];

    // 1. Membaca input hingga menemukan "---"
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\r\n")] = 0; // Bersihkan karakter newline
        
        if (strcmp(input, "---") == 0) break;

        // TASK 01: Receive
        if (strncmp(input, "receive#", 8) == 0) {
            char id[50], name[100];
            int qty;
            double price;
            if (sscanf(input, "receive#%[^#]#%[^#]#%d#%lf", id, name, &qty, &price) == 4) {
                if (find_item_index(id) == -1) { // Validasi ID unik
                    strcpy(inventory[item_count].id, id);
                    strcpy(inventory[item_count].name, name);
                    inventory[item_count].quantity = qty;
                    inventory[item_count].price = price;
                    add_log(id, "receive", qty);
                    item_count++;
                }
            }
        } 
        // TASK 02: Ship
        else if (strncmp(input, "ship#", 5) == 0) {
            char id[50];
            int qty;
            if (sscanf(input, "ship#%[^#]#%d", id, &qty) == 2) {
                int idx = find_item_index(id);
                // Validasi: barang ada, qty positif, stok cukup
                if (idx != -1 && qty > 0 && inventory[idx].quantity >= qty) {
                    inventory[idx].quantity -= qty;
                    add_log(id, "ship", qty);
                }
            }
        }
        // TASK 02: Restock
        else if (strncmp(input, "restock#", 8) == 0) {
            char id[50];
            int qty;
            if (sscanf(input, "restock#%[^#]#%d", id, &qty) == 2) {
                int idx = find_item_index(id);
                if (idx != -1 && qty > 0) {
                    inventory[idx].quantity += qty;
                    add_log(id, "restock", qty);
                }
            }
        }
        // TASK 01: Report
        else if (strcmp(input, "report") == 0) {
            for (int i = 0; i < item_count; i++) {
                printf("%s|%s|%d|%.1f\n", inventory[i].id, inventory[i].name, 
                       inventory[i].quantity, inventory[i].price);
            }
        }
        // TASK 03: Audit
        else if (strncmp(input, "audit#", 6) == 0) {
            char id[50];
            sscanf(input + 6, "%s", id);
            int idx = find_item_index(id);
            if (idx != -1) {
                // Info barang (Baris 1)
                printf("%s|%s|%d|%.1f\n", inventory[idx].id, inventory[idx].name, 
                       inventory[idx].quantity, inventory[idx].price);
                // History log (Baris selanjutnya)
                for (int i = 0; i < log_count; i++) {
                    if (strcmp(logs[i].item_id, id) == 0) {
                        printf("%d|%s|%d\n", logs[i].log_id, logs[i].type, logs[i].quantity);
                    }
                }
            }
        }
    }
    return 0;
}