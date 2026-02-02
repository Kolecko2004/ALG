// Kolika zpusoby mohu zaplatit 10 kc

#include <stdio.h>

int countWays(int coins[], int n, int amount) {
    int table[amount + 1];
    for(int i=0; i<=amount; i++) table[i] = 0;
    
    table[0] = 1; // Existuje 1 způsob, jak zaplatit 0 (nevzít nic)

    for (int i = 0; i < n; i++) {
        for (int j = coins[i]; j <= amount; j++) {
            table[j] += table[j - coins[i]];
        }
    }
    return table[amount];
}

int main() {
    int coins[] = {1, 2, 5};
    int amount = 10;
    printf("Pocet zpusobu: %d\n", countWays(coins, 3, amount));
    return 0;
}