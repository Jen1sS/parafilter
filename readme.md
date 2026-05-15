Il comando deve rispettare rigorosamente la seguente architettura:
1. Parallelismo: Il processo padre deve creare un processo figlio
(fork) per ogni file passato come argomento
2. Ricerca: Ogni figlio analizza il proprio file e cerca la keyword riga
per riga
3. IPC (Pipe): I figli non possono scrivere direttamente sul
terminale. Ogni figlio deve inviare i risultati (le righe trovate) al
padre tramite una pipe
4. Raccolta: Il padre legge da tutte le pipe e stampa su stdout i
risultati ricevuti dai figli

Meh
