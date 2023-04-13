Dieses Repo speichert ein oder mehrere embedded Programme fuer das ITSboard oder dessen Variante ITSboardAtHome. 
So kann zum Beispiel ein Studierender ein fork von diesem Repo erstellen und alle Aufgaben des GS Praktikums speichern. 

## Dokumentation
- s. [Wiki ITS_BRD](https://git.haw-hamburg.de/tiboard_dev/its_brd/-/wikis/**Dokumentation-der-Software-des-ITS-Board**)

Am Anfang werden die Variablen (AFFE, BEEF, 1234) werden im Memory an der Adresse 0x2000000C gespeichert. 

Anw01 ; speichert in Register0 (R0) die Adresse von Varaible A

Anw02 ; das niedrigste (Niederwertigste) Byte aus an der Stelle der Adresse , welche in R0 in ANw01 gespeichert wurde, wird in R2 gespeichert. (R2 = 0x000000EF ; memory: EF BE 34 12 FE AF )

Anw03 ; das zweitniedrigste (Niederwertigste) Byte aus an der Stelle der Adresse , welche in R0 in ANw01 gespeichert wurde, wird in R3 gespeichert. (R3 = 0x0000000BE ; memory: EF BE 34 12 FE AF)

Anw04 ; lsl verschiebt was im Register gespeichert um 8 bit (1byte) wurde das EF nach links verschoben (R2 = 0x0000EF00, memory: EF BE 34 12 FE AF )

Anw05 ; orr vergleicht die beiden Register R2 und R3 und wählt für jedes bit wo eine 1 in einem der Register steht eine 1 ansonsten eine 0 (R2 = 0x0000EFB, memory: EF BE 34 12 FE AFF)

Anw06 ; strh speichert 2byte (16 bit) an der Adresse welche in R0 gespeichert ist als little endian (niedrigster byte zu erst) = (Memory = BE EF 34 12 FE AF))
# GS
