# MODEL KŘIŽOVATKY

Tento projekt byl vytvořen jako závěrečná práce do předmětu Mikroprocesorová technika na 
<a href="https://www.spseol.cz/">**SPŠE v Olomouci**</a> ve třetím ročníku. Cílem projektu bylo vybrat si téma a zpracovat ho podle zadání. Já jsem se rozhodl vytvořit **model křižovatky** - jeho **sestavení a zapojení modulů na nepájivém poli** a také **vytvoření programu**, který jednotlivé moduly (respektive komponenty/součástky) **ovládat a řídit**.

Nápady řešení některých věcí a znalosti jsem potom skrze celé studium a také při tvorbě projektu jsem čerpal z těchto dvou stránek:

- <a href="https://chytrosti.marrek.cz/">**Marrkovy chytrosti**</a> - autor: **Ing. Marek Nožka**
- <a href="http://www.elektromys.eu/">**Elektromyš**</a> - autor: **Mgr. Michal Dudka**

## Zadání

- **Sestavte model křižovatky typu T.**

- **Každý směr jízdy řiďte světelnou signalizací (ROG semafory).**

- **Do této křižovatky také zařaďte přechod pro chodce.**

- **Přechod bude moct být spuštěn stiskem tlačítka z obou směrů.**

- **Na každé straně přechodu bude světelná signalizace (RG semafory).**

- **Dobu, která zbývá pro přejití přechodu, bude zobrazovat 7-segmentový displej na obou stranách přechodu.**

- **Během doby, kdy půjde přes přechod bezpečně přejít, bude znít také nějaké akustické znamení.**

- **Vytvořte algoritmus, který bude model křižovatky řídit tak, aby každý směr měl podobnou časovou dotaci volného průjezdu i čekání. Tento algoritmus bude muset také reagovat na vedlejší vlivy, jako je právě přechod pro chodce.**

- **Algoritmus zpracujte v rámci programu pro mikrokontrolér STM8. Program bude muset vhodně přijímat signály od tlačítek a vysílat signály pro jednotlivé periferie (LED, 7-segment, elektro-akustický měnič).**

- **Světelné řízení křižovatky popřípadě doplňte o vlastní nápady a inovace.**

## Schéma zapojení

Schéma zapojení je vytvořeno v programu <a href="https://www.kicad.org/">**KiCad**</a>.