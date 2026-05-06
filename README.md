# Operačné systémy 2018 - Copymaster

Tento projekt obsahuje základnú štruktúru zadania Copymaster.

Zdrojové súbory z tohoto projektu môžete použiť vo svojich zadaniach.

Aktuálne sa projekt skladá z dvoch modulov:
 - **copymaster** (`copymaster.c`) - Hlavný modul, ktorý obsahuje funkciu `main()`
 - **options** (`options.c`, `options.h`) - Spracovanie parametrov príkazového riadku (prepínačov)
 
## Spracovanie parametrov
 
Funkcia `ParseCopymaterOptions()` je zodpovedná za spracovanie parameterov príkazového riadku.
Typ návratovej hodnoty tejto funkcie je `struct CopymasterOptions`.
Táto štruktúra obsahuje členské premenné, ktoré reprezentujú jednotlivé prepínače programu v predspracovanej podobe.
Spôsob práce s týmito členskými premennými je demonštrovaný vo funkcii `main()` v súbore `copymaster.c`.

Pre kontrolu, či v parametroch príkazového riadku je nastavený prepínač, použite členskú premennú, ktorá má názov identický s názvom prepínača v zadaní úlohy. 
Výnimkou je prepínač `--delete` (v krátkej forme `-d`), ktorý je asociovaný s členskou premennou `delete_opt`.

Prepínače, ktoré majú aj nejakú vstupnú hodnotu (`--create`, `--lseek`, `--chmod`, `--umask`, `--truncate`), majú v štruktúre `CopymasterOptions` asociovanú dodatočnú členskú premennú. 
Použitie týchto premenných by malo byť ľahko pochopiteľné. 

Mále nejasnoti môžu byť pri členskej premennej `umask_options`. 
Premenná je dvojrozmerné pole 10x4 znakov. 
Toto pole je možné porozumieť ako zoznam nastavení z príkazového riadku. 
Každá položka prvej dimenzie poľa je platný reťazec v jazyku C, teda štvrtý bajt reťazca je `\0`.
Očakáva sa, že prepínač `--umask` môže mať maximálne 9 rôznych nastavení (3 typy používateľov - `u`,`g`,`o`, 3 typy práv - `r`, `w`, `x`). 
Ak je prvý bajt nastavenia `0`, znamená to koniec zoznamu nastavení a už nie je ďalej potrebné prechádzať položkami poľa `umask_options`.

## Changelog

Pre pravidelnú kontrolu aktivity je povinnosťou písať changelog. Changelog sa nachádza v súbore `CHANGELOG.md`. V prípade otázok je možné položiť otázky okrem [Mattermost](https://mattermost.kpi.fei.tuke.sk/os-2020/) aj v príslušnom týždnii v sekcii `Otazky`. V prípade žiadnej aktivity je potrebné do sekcie report napísať, že tento týždeň nebola vykonaná žiadna aktivita. 

Vzorový changelog: 

# Changelog

## 2. týždeň
### Report
- Implementácia prepínača pre pomalé kopírovanie
- Oprava chýb v implementácii prepínača pre rýchle kopírovanie
### Otazky
- 

## 1. týždeň
### Report
- Zoznámenie sa so zadaním, implementácia prepínača pre rýchle kopírovanie 
### Otázky
- Aký význam má implementácia prepínača pre pomalé kopírovanie?






Operating Systems 2018 – Copymaster

This project was developed as part of the Operating Systems course (2018).

It is based on the Copymaster assignment, which provides a basic structure for implementing a file copying utility with extended command-line options.

Project Structure

The project consists of two main modules:

copymaster (copymaster.c) – main program module containing the main() function
options (options.c, options.h) – command-line argument parsing and option handling
Functionality

The program processes command-line arguments using the function ParseCopymasterOptions(), which returns a structured representation of all parsed options.

These options are stored in the CopymasterOptions structure and are used in main() to control program behavior.

Each command-line switch (option) corresponds to a specific field in the structure. Some options also include additional parameters (e.g. --create, --lseek, --chmod, --umask, --truncate).

Special handling is used for the --umask option, which supports multiple permission settings stored in a 2D array structure.

Additional Requirements

A required part of the assignment was maintaining a CHANGELOG.md file, which documents weekly progress, implemented features, fixes, and reports of activity.

The changelog was used to track development progress and ensure continuous work on the project.

Purpose

The goal of the project was to practice:

working with command-line arguments in C
modular program design
file manipulation and system-level operations
structured data handling in C
disciplined development with changelog documentation
