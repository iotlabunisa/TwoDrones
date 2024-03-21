# Drone Prototype
Progetto sviluppato per l'esame di Lab of IoT presso l'Università degli Studi di Salerno A.A. 2023/2024

# Hardware
### Componenti
- Arduino: Viene utilizzato per gestire le funzioni di controllo di base del drone, come la stabilizzazione, il controllo dei motori e la gestione dei sensori di volo (giroscopi, accelerometri)
- ESP32-CAM: Questo modulo offre la capacità di trasmettere video in tempo reale tramite Wi-Fi. Viene utilizzato per integrare una telecamera nel drone, consentendo di ottenere feedback visivo in tempo reale
- I motori DC vengono utilizzati per controllare i movimenti del drone, ad esempio per la propulsione e il controllo dell’equilibrio 
- Il DC Motor Controller L298N agisce come un ponte H, permettendo di controllare la velocità e la direzione dei motori in base ai segnali inviati da Arduino

# Software
### Arduino IDE
In Arduino IDE è stato sviluppato l’intero sistema di controllo.
Nel loop principale del programma, vengono eseguite le seguenti operazioni:
- Vengono attivati i motori del drone per permettere il decollo
- Viene effettuata la lettura dei dati dai sensori inerziali (accelerometro e giroscopio)
- Vengono calcolati i segnali di controllo per il pitch e il roll del drone utilizzando i controller PID
- Viene inviato un pacchetto di dati contenente le letture dei sensori e i segnali di controllo al client connesso tramite Wi-Fi

La funzione `stabilise` è responsabile della stabilizzazione del drone regolando la velocità dei motori in base agli angoli di pitch e roll misurati dai sensori inerziali. 
La funzione calcola la velocità dei motori in base agli angoli di pitch e roll. Questi angoli indicano l'inclinazione del drone rispetto all'asse longitudinale (roll) e all'asse laterale (pitch). Dopodichè viene utilizzata la differenza tra gli angoli misurati e i punti di riferimento desiderati per determinare quanto deve essere regolata la velocità dei motori per correggere l'inclinazione del drone.
Una volta calcolate le nuove velocità dei motori, la funzione imposta direttamente i valori di PWM dei pin di controllo del motore (EN).

### Node-RED
Node-RED è uno tra i più noti tool di flow-based programming per l'Internet of Things. Nasce con l'obiettivo di dare a tutti la possibilità di collegare tra loro diversi dispositivi per poter realizzare sistemi altamente integrati e complessi in modo del tutto semplice ed intuitivo.
Nel nostro contesto è stato utilizzato per rappresentare graficamente (mediante una dashboard) tutti i valori in ricezione da Arduino. 
Più nello specifico i valori dell’Accelerometro, del Giroscopio, del sensore di temperatura e le potenze dei quattro motori.

### Python
Tramite Python viene implementato un Artificial Horizon, un componente grafico che offre una rappresentazione visiva dei dati di inclinazione (pitch) e rotazione laterale (roll).

# Autori & Contatti
| Name | Description |
| --- | --- |
| <p dir="auto"><strong>Alessandro Ricchetti</strong> |<br>Developer - <a href="https://github.com/Aless-FG">Aless-FG</a></p><p dir="auto">Email - <a href="mailto:a.ricchetti1@studenti.unisa.it">a.ricchetti1@studenti.unisa.it</a></p><p dir="auto">LinkedIn - <a href="https://www.linkedin.com/in/alessandro-ricchetti-832074275/">Alessandro Ricchetti</a></p><br>|
| <p dir="auto"><strong>Alessandro Macaro</strong> |<br>Developer   - <a href="https://github.com/mtolkien">mtolkien</a></p><p dir="auto">Email - <a href="mailto:a.macaro@studenti.unisa.it">a.macaro@studenti.unisa.it</a></p><p dir="auto">LinkedIn - <a href="https://www.linkedin.com/in/alessandro-macaro-391b7a214/">Alessandro Macaro</a></p><br>|
