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

## Vizualizace křižovatky

Na celou křižovatku byste se měli dívat tady tímto způsobem. V **Malování** jsem si už ze začátku práce na projektu udělal takový návrh, podle kterého jsem se **orientoval při vytváření kódu**. Dále v dokumentaci tu mám i trochu **rozšířenou verzi**, ale tady zatím stačí pouze její původní verze.

![Návrh projektu](Extern%C3%AD%20soubory%20ohledn%C4%9B%20projektu/N%C3%A1vrh%20projektu.png)

V obrázku není zahrnut přechod pro chodce z pohledu periferií, protože mi při návrhu šlo hlavně o semafory řídicí provoz na silnici. Lépe řešeno mně zajímali jednotlivé směry a jízdní pruhy, které jsem si musel očíslovat/popsat tak, aby se mi s nimi dobře pracovalo.

## Schéma zapojení

Schéma zapojení je vytvořeno v programu <a href="https://www.kicad.org/">**KiCad**</a>.

![Elektrické schéma - HARDWARE](Obrázky%20a%20videa/Elektrické%20sch%C3%A9ma%20-%20HARDWARE.png)

Pro detailnější zobrazení si otevřte PDF soubor [Model Křižovatky - Elektrické Schéma](Dokumentace/Model%20K%C5%99i%C5%BEovatky%20-%20Elektrick%C3%A9%20Sch%C3%A9ma%20-%20Projekt%20MIT.pdf).

## Vývojový diagram

## Popis činnosti programu

**Vytvoření algoritmu prošlo mnoha fázemi**. Kolikrát jsem měl již něco vytvořené, ale narazil jsem vždy na něco, kvůli čemu by to bylo nešikovné, ale nakonec jsem se dostal k verzi, která **funguje a řešení jednotlivých problémů mi přišlo dostatečně vhodné**.

Než začnu vysvětlovat činnost programu, tak bych Vám rád ukázal, jak jsem si rozdělil jednotlivé **pracovní cykly křižovatky**. **Barva** zde představuje **číslo režimu** (viz **legenda na levé straně**) a **směr**, který bude zrovna aktivní v tomto režimu. Všechny jiné semafory nepoužitých směrů budou nastavené do červené barvy.

![Návrh projektu - režimy jízdy](Externí%20soubory%20ohledně%20projektu/Návrh%20projektu%20-%20režimy%20jízdy.png)


Zde se taky můžete podívat, jak vypadá **finální výsledek na nepájivém poli**.

![Obrázek projektu](Obrázky%20a%20videa/Obrázek%20projektu.jpg)



## Zdrojový kód

## Nápady vylepšení, dotazy, připomínky
