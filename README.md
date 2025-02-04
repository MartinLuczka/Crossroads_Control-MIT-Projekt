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

## Blokové schéma

Blokové schéma je vytvořeno v programu <a href="https://cs.wikipedia.org/wiki/Microsoft_Word">**Microsoft Word**</a>.

![Blokové schéma](Dokumentace/Blokové%20schéma.png)

## Schéma zapojení

Schéma zapojení je vytvořeno v programu <a href="https://www.kicad.org/">**KiCad**</a>.

![Elektrické schéma - HARDWARE](Obrázky%20a%20videa/Elektrické%20sch%C3%A9ma%20-%20HARDWARE.png)

Pro detailnější zobrazení si otevřte PDF soubor [Model Křižovatky - Elektrické Schéma](Dokumentace/Model%20K%C5%99i%C5%BEovatky%20-%20Elektrick%C3%A9%20Sch%C3%A9ma%20-%20Projekt%20MIT.pdf).

## Vývojový diagram

Vývojový diagram jsem vytvořil v programu **Diagram Designer**.

![Vývojový diagram](Dokumentace/Vývojový%20diagram.png)

Pokud by Vás zajímalo více, tak se můžete podívat do složky [**Dokumentace**](Dokumentace/)

## Popis činnosti programu

**Vytvoření algoritmu prošlo mnoha fázemi**. Kolikrát jsem měl již něco vytvořené, ale narazil jsem vždy na něco, kvůli čemu by to bylo nešikovné, ale nakonec jsem se dostal k verzi, která **funguje a řešení jednotlivých problémů mi přišlo dostatečně vhodné**.

Než začnu vysvětlovat činnost programu, tak bych Vám rád ukázal, jak jsem si rozdělil jednotlivé **pracovní cykly křižovatky**. **Barva** zde představuje **číslo režimu** (viz **legenda na levé straně**) a **směr**, který bude zrovna aktivní v tomto režimu. Všechny jiné semafory nepoužitých směrů budou nastavené do červené barvy.

![Návrh projektu - režimy jízdy](Externí%20soubory%20ohledně%20projektu/Návrh%20projektu%20-%20režimy%20jízdy.png)


Zde se taky můžete podívat, jak vypadá **finální výsledek na nepájivém poli**.

![Obrázek projektu](Obrázky%20a%20videa/Obrázek%20projektu.jpg)

Program funguje tak, že si nejdříve **nadefinujeme makra**, díky kterým se nám bude mnohe lépe **přistupovat k našim periferiím**. Tyto makra poté následně hned využíváme při **inicializaci jednotlivých pinů a portů**. Těmto pinům také **nastavujeme počáteční hodnotu** a jak je má **mikroprocesor** chápat / **jak s nimi má pracovat**.

Dále zde máme **tvorbu polí**, do kterých si dáváme **piny a porty LED diod**, se kterými budeme pracovat v **určitých režimech**. Nebo také právě piny a jejich porty **určité skupiny**, se kterou potřebujeme pracovat. **Například včechny červené LED** --> Chceme **všechny semafory najednou nastavit do červené**.

**Odlišné pole** od těch ostatních je pole ***numbers[]***, do kterého si ukládáme **binární slova**, tak aby každé slovo bylo schopné **zobrazit určitou číslici na 7segmentu**.

Funkce ***show_numbers()*** nám právě na sedmisegment zobrazí takové číslo, které do této funkce **pošleme ve formě bajtového slova**.

Funkci ***crossing_activated()*** voláme, pokud chceme obsloužit požadavek na vykonání aktivního přechodu. V této funkci **pracujeme s reproduktorem** (**vyšší frekvence kmitání** --> **zvuková signalizace** aktivního přechodu pro chodce.) Dále také především pracujeme se **zobrazováním zbývajícího času pro přechod na 7segmentu**. Nesmíme také zapomenout na to, že **proměnná, která zaznamenává požadavek na přechod pro chodce je globální** a můžeme ji právě přepisovat z každého místa programu.

Požadavek na přechod pro chodce může být vykonán **kdykoliv** (až samozřejmě na stav aktivního přechodu) **skrze běh programu**. Požadavek uživatel vznese **zmáčnutím jednoho z tlačítek** (představuje 2 tlačítka --> na obou stranách přechodu).

Funkce ***wait_for_any_miliseconds()*** nám **umožňuje počkat v určitém stavu křižovatky na námi zvolenou dobu v milisekundách**. Pro tyto účely **nemůžeme použít funkci delay()**, protože ta by v průběhu nic nedělala, my ovšem **potřebujeme pracovat s reprodokturem a také detekovat stisk tlačítka**. To vše nám zajišťuje tato funkce.

Další 3 funkce jsou velice **jednoduché a plní docela podobný požadavek**. Jsou to funkce:
***turn_on()***, ***turn_off()*** a ***reverse()***. Do těchto funkcích posíláme v podstatě 3 porty a 3 piny našich 3 LEDek, se kterými chceme zrovna pracovat. **První funkce nám všechny LEDky rozsvítí, další funkce je zhasne a třetí funkce převrátí jejich stav.**

Další na řadě je funkce ***main()***, která se v podstatě **spustí hned po spuštění programu**. V této funkci hned na začátku **voláme funkci** ***init()***, ve které je dána **veškerá inicializace**. Dále zde **deklarujeme**, pro nás velice zásadní, **proměnné**. Poté už je zde jen **nekonečná smyčka**, ve které se **program v běhu neustále točí**.

V tomto nekonečném cyklu opět řídíme reproduktor (mimo aktivní přechod pro chodce má mnohem nižší frekvenci - signál STOP) a detekujeme stisknutí tlačítka. Dále zde jen v podsatě střídáme 3 režimy celé křižovatky, které si liší tím, kdo má zelenou - signál GO.

**Který režim se má zrovna vykonat / nebo se vykonává** snímáme v proměnné ***crossroad_mode***, kterou po dokončení daného režimu **vždy přepisujeme na následující** režim. Na konci toho posledního režimu ji opět přepíšeme na režim první. Na začátku těchto režimů **vždy kontrolujeme, jestli nebyl vznešen požadavek na přechodu pro chodce**. Pokud byl, tak ho **provedeme a požadavek přepíšeme do 0** (vynonali jsme ho). Jak již tedy jistě tušíte, tak **pokud někdo stiskne tlačítko**, že chce přejít, **tak se vykoná již rozdělaný režim a požadavek se vykoná před začátkem následujícího**.

V jednotlivých režimech **zhasínáme a rožínáme LEDky podle námi vytvořeného systému**. Nejdříve **rožneme oranžovou**, kterou **následně vystřídá zelená**. Před koncem zelené začne **blikat (signalizace, že už bude končit)**, následně se **rozsvítí opět na sekundu oranžová**, která se **následně přepne na červenou**. Na červenou se přepnout všechny semafory, takto končí každý režim. I režim při vykonání přechodu pro chodce. **Na konci vždy červená zůstává 2 sekundy, poté už se vykonává následující režim**.

## Závěr

V tomto projektu se mi podařilo realizovat všechny cíle, které jsem si před jeho zahájením stanovil. Periferie, zapojené k mikrokontroléru podle schématu, fungovaly správně a bylo možné je úspěšně řídit prostřednictvím vytvořeného programu. Při řešení jsem narazil na řadu technických problémů, avšak u všech se mi podařilo najít optimální řešení. Rád bych vyzdvihl zejména vytvořený program, který implementuje systém řízení křižovatky a dokáže efektivně reagovat na vstupy od uživatele.

Práce na tomto programu mi přinesla mnoho nových poznatků. Nejvíce jsem se naučil při práci s makry a při vytváření funkcí, které výrazně usnadnily a zpřehlednily další programování. Díky tomu, že jsem většinu příkazů zapouzdřil do funkcí, zůstala funkce main() přehledná a dobře organizovaná, což významně přispělo k systematickému přístupu k práci.

Do budoucna vidím prostor pro několik vylepšení. Jedním z nich by bylo vytvoření fyzického modelu reálné křižovatky, který by umožnil praktickou implementaci mých komponent společně s mikrokontrolérem. Dále by bylo možné celý projekt posunout na vyšší úroveň přidáním senzorů pro detekci přítomnosti vozidel. Tyto senzory by mohly poskytovat data o zaplněnosti jednotlivých příjezdových pruhů a na jejich základě by systém mohl optimalizovat řízení dopravy na křižovatce.

## Kontakt

Pokud by Vás něco zajímalo, chtěli byste se podílet na tvoření tohoto projektu, tak se mě nebojte kontaktovat na emailové adrese: ***luczka.martin@gmail.com***

**Martin Luczka**

## Zdrojový kód

Zde je k dispozici zdrojový kód ze **zdrojového souboru** [**main.c**](src/main.c).

```c
#include <stdbool.h>
#include <stm8s.h>
#include "main.h"
#include "milis.h"

// Makra našich portů a pinů SMT8, děláme to proto, abychom věděli, 
// k čemu zrovna přistupujeme, př. GPIOC, GPIO_PIN_6 nám asi nic moc neřekne
// Když ovšem máme speaker_PORT a speaker_PIN, tak hned vím, k jaké pariferii se daný PORT s PINEM vážou

// segment A sedmisegmentu
#define segment_a_PORT GPIOE
#define segment_a_PIN  GPIO_PIN_0
// segment b sedmisegmentu
#define segment_b_PORT GPIOC
#define segment_b_PIN  GPIO_PIN_1
// segment C sedmisegmentu
#define segment_c_PORT GPIOC
#define segment_c_PIN  GPIO_PIN_3
// segment D sedmisegmentu
#define segment_d_PORT GPIOC
#define segment_d_PIN  GPIO_PIN_2
// segment E sedmisegmentu
#define segment_e_PORT GPIOG
#define segment_e_PIN  GPIO_PIN_0
// segment F sedmisegmentu
#define segment_f_PORT GPIOD
#define segment_f_PIN  GPIO_PIN_5
// segment G sedmisegmentu
#define segment_g_PORT GPIOD
#define segment_g_PIN  GPIO_PIN_6
// tlačítko, hardwarově v podstatě dvě
#define button_PORT GPIOA
#define button_PIN  GPIO_PIN_6
// reproduktor (zvuková signalizace přechodu)
#define speaker_PORT GPIOG
#define speaker_PIN GPIO_PIN_6
// zelená přechodu pro chodce
#define crossing_lights_green_PORT GPIOB
#define crossing_lights_green_PIN  GPIO_PIN_7
// červená přechodu pro chodce
#define crossing_lights_red_PORT GPIOB
#define crossing_lights_red_PIN  GPIO_PIN_6
// zelená směru A - rovně
#define A_straight_green_PORT GPIOB
#define A_straight_green_PIN  GPIO_PIN_0
// oranžová směru A - rovně
#define A_straight_orange_PORT GPIOB
#define A_straight_orange_PIN  GPIO_PIN_1
// červená směru A - rovně
#define A_straight_red_PORT GPIOB
#define A_straight_red_PIN  GPIO_PIN_2
// zelená směru A - doprava
#define A_right_green_PORT GPIOB
#define A_right_green_PIN  GPIO_PIN_3
// oranžová směru A - doprava
#define A_right_orange_PORT GPIOB
#define A_right_orange_PIN  GPIO_PIN_4
// červená směru A - doprava
#define A_right_red_PORT GPIOB
#define A_right_red_PIN  GPIO_PIN_5
// zelená směru B - doleva
#define B_left_green_PORT GPIOE
#define B_left_green_PIN  GPIO_PIN_2
// oranžová směru B - doleva
#define B_left_orange_PORT GPIOE
#define B_left_orange_PIN  GPIO_PIN_1
// červená směru B - doleva
#define B_left_red_PORT GPIOD
#define B_left_red_PIN  GPIO_PIN_7
// zelená směru B - doprava
#define B_right_green_PORT GPIOE
#define B_right_green_PIN  GPIO_PIN_5
// oranžová směru B - doprava
#define B_right_orange_PORT GPIOC
#define B_right_orange_PIN  GPIO_PIN_6
// červená směru B - doprava
#define B_right_red_PORT GPIOC
#define B_right_red_PIN  GPIO_PIN_7
// zelená směru C - doleva
#define C_left_green_PORT GPIOF
#define C_left_green_PIN  GPIO_PIN_5
// oranžová směru C - doleva
#define C_left_orange_PORT GPIOF
#define C_left_orange_PIN  GPIO_PIN_6
// červená směru C - doleva
#define C_left_red_PORT GPIOF
#define C_left_red_PIN  GPIO_PIN_7
// zelená směru C - rovně
#define C_straight_green_PORT GPIOF
#define C_straight_green_PIN  GPIO_PIN_0
// oranžová směru C - rovně
#define C_straight_orange_PORT GPIOF
#define C_straight_orange_PIN  GPIO_PIN_3
// červená směru C - rovně
#define C_straight_red_PORT GPIOF
#define C_straight_red_PIN  GPIO_PIN_4

// definování globální proměnné, pokud je tato proměnná 1, tak chce někdo přejít na přechodu
bool crossing_request = 0;

// Funkce, ve které inicializujeme jednotlivé vstupy a výstupy mikrokontroléru, voláme ji poté ve funkci main()
void init(void)
{
    // Taktování MCU na 16MHz
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    // 7 segment
    GPIO_Init(segment_a_PORT, segment_a_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_b_PORT, segment_b_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_c_PORT, segment_c_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_d_PORT, segment_d_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_e_PORT, segment_e_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_f_PORT, segment_f_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_g_PORT, segment_g_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Reproduktor
    GPIO_Init(speaker_PORT, speaker_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    // tlačítko
    GPIO_Init(button_PORT, button_PIN, GPIO_MODE_IN_PU_NO_IT);

    // LED semaforu přechodu
    GPIO_Init(crossing_lights_green_PORT, crossing_lights_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(crossing_lights_red_PORT, crossing_lights_red_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    // Směr křižovatky A
    GPIO_Init(A_straight_red_PORT, A_straight_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_straight_orange_PORT, A_straight_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_straight_green_PORT, A_straight_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_red_PORT, A_right_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_orange_PORT, A_right_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_green_PORT, A_right_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Směr křižovatky B
    GPIO_Init(B_left_red_PORT, B_left_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_left_orange_PORT, B_left_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_left_green_PORT, B_left_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_red_PORT, B_right_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_orange_PORT, B_right_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_green_PORT, B_right_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Směr křižovatky C
    GPIO_Init(C_left_red_PORT, C_left_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_left_orange_PORT, C_left_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_left_green_PORT, C_left_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_red_PORT, C_straight_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_orange_PORT, C_straight_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_green_PORT, C_straight_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Povolení milis()
    init_milis();
}

// Pole pro všechny porty - v kódu nevyužíváme, ale mohlo by se eventuelně hodit
const GPIO_TypeDef* all_ports[] = {
    A_straight_red_PORT,
    A_straight_orange_PORT,
    A_straight_green_PORT,
    A_right_red_PORT,
    A_right_orange_PORT,
    A_right_green_PORT,
    B_left_red_PORT,
    B_left_orange_PORT,
    B_left_green_PORT,
    B_right_red_PORT,
    B_right_orange_PORT,
    B_right_green_PORT,
    C_left_red_PORT,
    C_left_orange_PORT,
    C_left_green_PORT,
    C_straight_red_PORT,
    C_straight_orange_PORT,
    C_straight_green_PORT
};

// Pole pro všechny piny - v kódu nevyužíváme, ale mohlo by se eventuelně hodit
const uint16_t all_pins[] = {
    A_straight_red_PIN,
    A_straight_orange_PIN,
    A_straight_green_PIN,
    A_right_red_PIN,
    A_right_orange_PIN,
    A_right_green_PIN,
    B_left_red_PIN,
    B_left_orange_PIN,
    B_left_green_PIN,
    B_right_red_PIN,
    B_right_orange_PIN,
    B_right_green_PIN,
    C_left_red_PIN,
    C_left_orange_PIN,
    C_left_green_PIN,
    C_straight_red_PIN,
    C_straight_orange_PIN,
    C_straight_green_PIN
};

// Pole pro porty všech červených - využíváme v kódu pro nastavení všech silničních semaforů na červenou
const GPIO_TypeDef* all_red_ports[] = {
    A_straight_red_PORT,
    A_right_red_PORT,
    B_left_red_PORT,
    B_right_red_PORT,
    C_left_red_PORT,
    C_straight_red_PORT,
};
// Pole pro piny všech červených - využíváme v kódu pro nastavení všech silničních semaforů na červenou
const uint16_t all_red_pins[] = {
    A_straight_red_PIN,
    A_right_red_PIN,
    B_left_red_PIN,
    B_right_red_PIN,
    C_left_red_PIN,
    C_straight_red_PIN,
};

// Pole pro porty, na kterým máme LED diody, které využíváme v režimu křižovatky 1
const GPIO_TypeDef* mode_1_PORT[] = {
    B_right_red_PORT,
    B_left_red_PORT,
    A_right_red_PORT,
    B_right_orange_PORT,
    B_left_orange_PORT,
    A_right_orange_PORT,
    B_right_green_PORT,
    B_left_green_PORT,
    A_right_green_PORT,
};
// Pole pro piny, na kterým máme LED diody, které využíváme v režimu křižovatky 1
const uint8_t mode_1_PIN[] = {
    B_right_red_PIN,
    B_left_red_PIN,
    A_right_red_PIN,
    B_right_orange_PIN,
    B_left_orange_PIN,
    A_right_orange_PIN,
    B_right_green_PIN,
    B_left_green_PIN,
    A_right_green_PIN,
};

// Pole pro porty, na kterým máme LED diody, které využíváme v režimu křižovatky 2
const GPIO_TypeDef* mode_2_PORT[] = {
    A_straight_red_PORT,
    A_right_red_PORT,
    C_straight_red_PORT,
    A_straight_orange_PORT,
    A_right_orange_PORT,
    C_straight_orange_PORT,
    A_straight_green_PORT,
    A_right_green_PORT,
    C_straight_green_PORT,
};
// Pole pro piny, na kterým máme LED diody, které využíváme v režimu křižovatky 2
const uint8_t mode_2_PIN[] = {
    A_straight_red_PIN,
    A_right_red_PIN,
    C_straight_red_PIN,
    A_straight_orange_PIN,
    A_right_orange_PIN,
    C_straight_orange_PIN,
    A_straight_green_PIN,
    A_right_green_PIN,
    C_straight_green_PIN,
};

// Pole pro porty, na kterým máme LED diody, které využíváme v režimu křižovatky 3
const GPIO_TypeDef* mode_3_PORT[] = {
    C_straight_red_PORT,
    C_left_red_PORT,
    B_right_red_PORT,
    C_straight_orange_PORT,
    C_left_orange_PORT,
    B_right_orange_PORT,
    C_straight_green_PORT,
    C_left_green_PORT,
    B_right_green_PORT,
};
// Pole pro piny, na kterým máme LED diody, které využíváme v režimu křižovatky 3
const GPIO_TypeDef* mode_3_PIN[] = {
    C_straight_red_PIN,
    C_left_red_PIN,
    B_right_red_PIN,
    C_straight_orange_PIN,
    C_left_orange_PIN,
    B_right_orange_PIN,
    C_straight_green_PIN,
    C_left_green_PIN,
    B_right_green_PIN,
};

// Pole pro porty, na kterým máme LED diody, které využíváme v režimu, když je aktivní přechod pro chodce
const GPIO_TypeDef* crossing_mode_PORT[] = {
    A_straight_orange_PORT,
    C_straight_orange_PORT,
    A_straight_green_PORT,
    C_straight_green_PORT,
    A_straight_red_PORT,
    C_straight_red_PORT,
};
// Pole pro piny, na kterým máme LED diody, které využíváme v režimu, když je aktivní přechod pro chodce
const GPIO_TypeDef* crossing_mode_PIN[] = {
    A_straight_orange_PIN,
    C_straight_orange_PIN,
    A_straight_green_PIN,
    C_straight_green_PIN,
    A_straight_red_PIN,
    C_straight_red_PIN,
};

// pole, ve kterém máme bitová slova, podle toho, jaké chceme rozsvítit segmenty, aby se zobrazilo konkrétní číslo
const uint8_t numbers[] =
{
    0b11000000, // číslo 0
    0b11111001, // číslo 1
    0b10100100, // číslo 2
    0b10110000, // číslo 3
    0b10011001, // číslo 4
    0b10010010, // číslo 5
    0b10000010, // číslo 6
    0b11111000, // číslo 7
    0b10000000, // číslo 8
    0b10010000, // číslo 9
    0b11111111, // 7segmentová obrazovka je zhasnuta
};

// Funkce, do které si pošleme bitové slovo z pole numbers. V tomto slově si procházíme každý bit 
// a podle jeho hodnoty buď, rozvítíme, nebo zhasneme segment
void show_number(uint8_t number) {
    if (number & 1<<0) {
        HIGH(segment_a_PORT, segment_a_PIN);
    }
    else {
        LOW(segment_a_PORT, segment_a_PIN);
    }
    if (number & 1<<1) {
        HIGH(segment_b_PORT, segment_b_PIN);
    }
    else {
        LOW(segment_b_PORT, segment_b_PIN);
    }
    if (number & 1<<2) {
        HIGH(segment_c_PORT, segment_c_PIN);
    }
    else {
        LOW(segment_c_PORT, segment_c_PIN);
    }
    if (number & 1<<3) {
        HIGH(segment_d_PORT, segment_d_PIN);
    }
    else {
        LOW(segment_d_PORT, segment_d_PIN);
    }
    if (number & 1<<4) {
        HIGH(segment_e_PORT, segment_e_PIN);
    }
    else {
        LOW(segment_e_PORT, segment_e_PIN);
    }
    if (number & 1<<5) {
        HIGH(segment_f_PORT, segment_f_PIN);
    }
    else {
        LOW(segment_f_PORT, segment_f_PIN);
    }
    if (number & 1<<6) {
        HIGH(segment_g_PORT, segment_g_PIN);
    }
    else {
        LOW(segment_g_PORT, segment_g_PIN);
    }
}

// Funkce která se spustí
void crossing_activated(void) {
    // Deklarace potřebných proměnných
    uint8_t crossing_time_number = 8; // Může se zde upravit
    uint32_t time = 0;
    uint32_t crossing_sound_time = 0;
    uint32_t crossing_sound_on = 0;
    uint32_t sound_time = 0;
    // Vypnutí červené na aktivních semaforech
    HIGH(crossing_mode_PORT[4], crossing_mode_PIN[4]);
    HIGH(crossing_mode_PORT[5], crossing_mode_PIN[5]);
    // Vypnutí a zapnutí oranžové
    LOW(crossing_mode_PORT[0], crossing_mode_PIN[0]);
    LOW(crossing_mode_PORT[1], crossing_mode_PIN[1]);
    wait_for_any_miliseconds(1000);
    HIGH(crossing_mode_PORT[0], crossing_mode_PIN[0]);
    HIGH(crossing_mode_PORT[1], crossing_mode_PIN[1]);
    // Zapnutí zelené na silničních semaforech
    LOW(crossing_mode_PORT[2], crossing_mode_PIN[2]);
    LOW(crossing_mode_PORT[3], crossing_mode_PIN[3]);
    // Přepnutí barev semaforu pro chodce, rozvicujeme zelenou - VOLNO
    HIGH(crossing_lights_red_PORT, crossing_lights_red_PIN);
    LOW(crossing_lights_green_PORT, crossing_lights_green_PIN);

    // Dokud je zobrazované číslo menší jak 0
    while (crossing_time_number > 0) {
        // Provádíme každých 1000 ms
        if(milis() - time > 1000) {
            time = milis();
            // Zobrazení čísla
            show_number(numbers[crossing_time_number]);
            // Tak, jak je nastavena podmínka, tak by se nezobrazila 1
            if (crossing_time_number == 1) {
                // Počkej na ní 1000 ms
                wait_for_any_miliseconds(1000);
            }
            // Odečteme zobrazované číslo
            crossing_time_number -= 1;
        }
        // Každých 125 ms zapni nebo vypni zvuk, odpovídá frekvenci 8 Hz (reálná hodnota semaforů v současnosti)
        if(milis() - crossing_sound_time > 125) {
            // Pokud je zvuk vypnutý
            if(crossing_sound_on == 0) {
                // Tak ho zapni
                crossing_sound_on = 1;
            }
            // Popřípadě ho vypni, pokud je zapnutý
            else{
                crossing_sound_on = 0;
            }
            crossing_sound_time = milis();
        }
        // Generujeme zvuk o frekvenci 1000 Hz, podmínka se splní v případě, že je crossing_sound_on nastaveno do 1, 
        // logický součin AND
        if((milis() - sound_time > 1) && crossing_sound_on) {
                sound_time = milis();
                // Převracení stavu PINU, hardwarově spínáme tranzistor
                REVERSE(speaker_PORT, speaker_PIN);
            }
    }
    // Zhasnutí 7segmentovky
    show_number(numbers[10]);
    // Vypnutí zelené
    HIGH(crossing_mode_PORT[2], crossing_mode_PIN[2]);
    HIGH(crossing_mode_PORT[3], crossing_mode_PIN[3]);
    // Zapnutí a vypnutí oranžové
    LOW(crossing_mode_PORT[0], crossing_mode_PIN[0]);
    LOW(crossing_mode_PORT[1], crossing_mode_PIN[1]);
    // Oranžovou chceme zobrazovat sekundu
    wait_for_any_miliseconds(1000);
    HIGH(crossing_mode_PORT[0], crossing_mode_PIN[0]);
    HIGH(crossing_mode_PORT[1], crossing_mode_PIN[1]);
    // Nastavení všeho zpět na červenou, for cyklem projíždíme pole se všemi červenými porty/piny
    for(uint16_t i = 0; i < sizeof(all_red_ports)/sizeof(all_red_ports[0]); i++) {
        LOW(all_red_ports[i], all_red_pins[i]);
    }
    // Zhasnutí zelených světel semaforu pro chodce, nastavení červených - signál STOP
    LOW(crossing_lights_red_PORT, crossing_lights_red_PIN);
    HIGH(crossing_lights_green_PORT, crossing_lights_green_PIN);
    // V červené chceme zůstat vždy 2 sekundy, ať fungování celé křižovatky není zbytečně rychlé
    wait_for_any_miliseconds(2000);
}

// Funkce, do které si pošleme počet milisekund, které chceme počkat, 
// zajišťuje správné fungování reproduktoru a detekci tlačítka
void wait_for_any_miliseconds(int16_t waiting_miliseconds) {
    // Zjistíme si současný čas a uložíme do proměnné + ostatní potřebné proměnné
    int32_t time_now = milis();
    int32_t switching_sound_time = 0;
    bool sound_on = 0;
    int32_t sound_time = 0;
    bool stisk = 0;
    int32_t time = 0;
    // Dokud je současný čas menší než, čas při kterém jsme do funkce vstupovali + čas, 
    // který chce uživatel ve funkci strávit
    while(milis() < time_now + waiting_miliseconds) {
        // Vypináme a zapínáme každých 667 ms, odpovídá frekvenci 1,5 Hz, 
        // což je v podstatě reálná hodnota používaná v praxi
        if(milis() - switching_sound_time > 667) {
            if(sound_on == 0) {
                sound_on = 1;
            }
            else{
                sound_on = 0;
            }
            switching_sound_time = milis();
        }
        // Generujeme zvuk o frekvenci 1000 Hz
        if((milis() - sound_time > 1) && sound_on) {
                sound_time = milis();
                REVERSE(speaker_PORT, speaker_PIN);
            }
        // Každých 100 ms kontrolujeme, jestli nebylo stisknuto tlačítko    
        if (milis() - time > 100) {
            time = milis();
            if (PUSH(button_PORT, button_PIN)) {
                stisk = 1;
            } else {
                if (stisk == 1) {
                    // Nastavení globální proměnné do 1, víme, že budeme chtít spustit režim přechodu pro chodce
                    crossing_request = 1;
                    stisk = 0;
                }
            }
        }
    }
}
// Funkce pro zapnutí 3 LED diod zaráz
void turn_on(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3) {
    LOW(port1, pin1);
    LOW(port2, pin2);
    LOW(port3, pin3);
}

// Funkce pro vypnutí 3 LED diod zaráz
void turn_off(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3) {
    HIGH(port1, pin1);
    HIGH(port2, pin2);
    HIGH(port3, pin3);
}

// Funkce pro převrácení stavu 3 LED diod zaráz
void reverse(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3) {
    REVERSE(port1, pin1);
    REVERSE(port2, pin2);
    REVERSE(port3, pin3);
}

// Hlavní funkce main
int main(void) {
    // Zavolání funkce, která nám inicializuje jednotlivé porty a piny + jejich počáteční hodnoty
    init();

    // Deklarace potřebných proměnných, pokud nám proměnná nabývá pouze 2 hodnot - pravda/nepravda, 
    // tak ji nastavujeme datový typ bool
    uint32_t time = 0;
    bool stisk = 0;
    // Režim křižovatky počátečně nastavujeme do 1, respektive první režim
    uint8_t crossroad_mode = 1;
    uint32_t crossroad_mode_time = 0;
    uint32_t sound_time = 0;
    bool sound_on = 0;
    uint32_t switching_sound_time = 0;
    
    // Ze začátku nastavíme veškeré semafory do stavu, kdy zobrazují červenou
    for(uint16_t i = 0; i < sizeof(all_red_ports)/sizeof(all_red_ports[0]); i++) {
        LOW(all_red_ports[i], all_red_pins[i]);
    }
    // Na tomto stavu chceme setrvat po dobu 2000 ms
    wait_for_any_miliseconds(2000);
    
    // Nekonečný cyklus, ve kterém nám náš program neustále běží po spuštění
    while (1) {
        // Pokud nejsme ve funkci wait_for_any_miliseconds, 
        // tak musíme reproduktor ovládat a tlačítko zaznamenávat i ve funkci main
        // Při stavu neaktivního přechodu pro chodce signál spínáme frekvencí 1,5 Hz
        if(milis() - switching_sound_time > 667) {
            if(sound_on == 0) {
                sound_on = 1;
            }
            else{
                sound_on = 0;
            }
            switching_sound_time = milis();
        }
        // Generujeme signál o frekvenci 1000 Hz
        if((milis() - sound_time > 1) && sound_on) {
                sound_time = milis();
                REVERSE(speaker_PORT, speaker_PIN);
            }
        // Kontrolujeme stisknutí tlačítka
        if (milis() - time > 100) {
            time = milis();
            if (PUSH(button_PORT, button_PIN)) {
                stisk = 1;
            } else {
                if (stisk == 1) {
                    crossing_request = 1;
                    stisk = 0;
                }
            }
            // Pokud je režim křizovatky nastaven do jedničky...
            if (crossroad_mode == 1) {
                // Pokud byl vznešen požadavek na přechod (stisknutí tlačítka)
                if(crossing_request == 1) {
                    // Provedeme režim přechodu
                    crossing_activated();
                    // Požadavek byl splněn, nastavíme tedy zpátky do 0
                    crossing_request = 0;
                }
                // Zhasnutí červené na aktivních semaforech při tomto režimu
                turn_off(mode_1_PORT[0], mode_1_PIN[0], mode_1_PORT[1], mode_1_PIN[1], mode_1_PORT[2], mode_1_PIN[2]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_1_PORT[3], mode_1_PIN[3], mode_1_PORT[4], mode_1_PIN[4], mode_1_PORT[5], mode_1_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_1_PORT[3], mode_1_PIN[3], mode_1_PORT[4], mode_1_PIN[4], mode_1_PORT[5], mode_1_PIN[5]);
                // Rožnutí zelených LED - VOLNO v daných směrech
                turn_on(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(6500);
                // Blikání LED před vypnutím zelené na semaforu, přeblikáváme každých 250 ms
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                wait_for_any_miliseconds(250);
                turn_off(mode_1_PORT[6], mode_1_PIN[6], mode_1_PORT[7], mode_1_PIN[7], mode_1_PORT[8], mode_1_PIN[8]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_1_PORT[3], mode_1_PIN[3], mode_1_PORT[4], mode_1_PIN[4], mode_1_PORT[5], mode_1_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_1_PORT[3], mode_1_PIN[3], mode_1_PORT[4], mode_1_PIN[4], mode_1_PORT[5], mode_1_PIN[5]);
                // Zpět na červenou pro všechny semafory
                for(uint16_t i = 0; i < sizeof(all_red_ports)/sizeof(all_red_ports[0]); i++) {
                    LOW(all_red_ports[i], all_red_pins[i]);
                }
                // Červenou chceme mít nastavenou po dobu 2000 ms = 2 s
                wait_for_any_miliseconds(2000);
                // Nastavíme režim křižovatky do 2, druhý režim
                crossroad_mode = 2;
            }
            // Když je program nastaven do druhého režimu křižovatky
            if(crossroad_mode == 2) {
                // Kontrola pro obsloužení přechodu pro chodce
                if(crossing_request == 1) {
                    crossing_activated();
                    crossing_request = 0;
                }
                // Zhasnutí červené na aktivních semaforech při tomto režimu
                turn_off(mode_2_PORT[0], mode_2_PIN[0], mode_2_PORT[1], mode_2_PIN[1], mode_2_PORT[2], mode_2_PIN[2]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_2_PORT[3], mode_2_PIN[3], mode_2_PORT[4], mode_2_PIN[4], mode_2_PORT[5], mode_2_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_2_PORT[3], mode_2_PIN[3], mode_2_PORT[4], mode_2_PIN[4], mode_2_PORT[5], mode_2_PIN[5]);
                // Rožnutí zelených LED - VOLNO v daných směrech
                turn_on(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(6500);
                // Blikání LED před vypnutím zelené
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                wait_for_any_miliseconds(250);
                turn_off(mode_2_PORT[6], mode_2_PIN[6], mode_2_PORT[7], mode_2_PIN[7], mode_2_PORT[8], mode_2_PIN[8]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_2_PORT[3], mode_2_PIN[3], mode_2_PORT[4], mode_2_PIN[4], mode_2_PORT[5], mode_2_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_2_PORT[3], mode_2_PIN[3], mode_2_PORT[4], mode_2_PIN[4], mode_2_PORT[5], mode_2_PIN[5]);
                // Nastavení červeného světla na všech semaforech
                for(uint16_t i = 0; i < sizeof(all_red_ports)/sizeof(all_red_ports[0]); i++) {
                    LOW(all_red_ports[i], all_red_pins[i]);
                }
                wait_for_any_miliseconds(2000);
                // Nastavení režimu do 3, třetí režim
                crossroad_mode = 3;
            }
            // Pokud je nastaven třetí režim křižovatky
            if(crossroad_mode == 3) {
                // Kontrola, jestli někdo nechce přejít přechod pro chodce
                if(crossing_request == 1) {
                    crossing_activated();
                    crossing_request = 0;
                }
                // Zhasnutí červené na aktivních semaforech při tomto režimu
                turn_off(mode_3_PORT[0], mode_3_PIN[0], mode_3_PORT[1], mode_3_PIN[1], mode_3_PORT[2], mode_3_PIN[2]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_3_PORT[3], mode_3_PIN[3], mode_3_PORT[4], mode_3_PIN[4], mode_3_PORT[5], mode_3_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_3_PORT[3], mode_3_PIN[3], mode_3_PORT[4], mode_3_PIN[4], mode_3_PORT[5], mode_3_PIN[5]);
                // Rožnutí zelených LED - VOLNO v daných směrech
                turn_on(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(6500);
                // Blikání LED před vypnutím zeleného světla na semaforech
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                reverse(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                wait_for_any_miliseconds(250);
                turn_off(mode_3_PORT[6], mode_3_PIN[6], mode_3_PORT[7], mode_3_PIN[7], mode_3_PORT[8], mode_3_PIN[8]);
                // Rožnutí a zhasnutí oranžové
                turn_on(mode_3_PORT[3], mode_3_PIN[3], mode_3_PORT[4], mode_3_PIN[4], mode_3_PORT[5], mode_3_PIN[5]);
                wait_for_any_miliseconds(1000);
                turn_off(mode_3_PORT[3], mode_3_PIN[3], mode_3_PORT[4], mode_3_PIN[4], mode_3_PORT[5], mode_3_PIN[5]);
                // Zpět na červenou pro všechny semafory
                for(uint16_t i = 0; i < sizeof(all_red_ports)/sizeof(all_red_ports[0]); i++) {
                    LOW(all_red_ports[i], all_red_pins[i]);
                }
                // V červené chceme setrvat 2 s
                wait_for_any_miliseconds(2000);
                // Celý proces se opakuje a režim semaforu se nastavuje do 1 (počáteční stav), vrací se na začátek
                crossroad_mode = 1;
            }
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
```

Tady se ještě můžete kouknout na používaná makra a funkce v **hlavičkovém souboru** 
[**main.h**](inc/main.h).

```c
#ifndef _MAIN_H_
#define _MAIN_H_ 1

#define LOW(PORT, PIN) GPIO_WriteLow(PORT, PIN)
#define HIGH(PORT, PIN) GPIO_WriteHigh(PORT, PIN)
#define REVERSE(PORT, PIN) GPIO_WriteReverse(PORT, PIN)

#define READ(PORT, PIN) (GPIO_ReadInputPin(PORT, PIN)!=RESET) 
#define PUSH(PORT, PIN) (GPIO_ReadInputPin(PORT, PIN)==RESET) 

// Discovery Board
#ifdef STM8S003
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#define BTN_PORT GPIOB
#define BTN_PIN  GPIO_PIN_7
#endif

// Dero Board
#ifdef STM8S103
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_4
#endif

// Discovery Board
#ifdef STM8S105
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#endif

// Nucleo Kit
#ifdef STM8S208
#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#endif

// functions
void init(void);
void show_number(uint8_t number);
void crossing_activated(void);
void wait_for_any_miliseconds(int16_t waiting_miliseconds);
void turn_on(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3);
void turn_off(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3);
void reverse(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3);

#endif // !_MAIN_H_
```
