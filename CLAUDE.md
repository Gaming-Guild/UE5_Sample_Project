# Klondike Solitaire en UE5 (C++ + UMG)

Guía paso a paso para implementar un Klondike Solitaire usando widgets UMG (UI 2D) para el layout visual y C++ para los datos y la lógica del juego. Los Blueprints heredan de clases C++ ("reparenting") y se usan solo para diseño visual.

## Estado actual (completado)

- ✅ Enum `E_CardSuit` (Hearts, Diamonds, Clubs, Spades)
- ✅ Struct `S_Card` (Suit, Rank, bIsFaceUp)
- ✅ 52 imágenes de cartas + reverso en `Content/UI/Cards`
- ✅ Struct `S_CardVisual` (Suit, Rank, CardTexture) y `DT_Cards` (DataTable, 52 filas)
- ✅ Widgets visuales creados: `WBP_Card`, `WBP_PileSlot`, `WBP_GameBoard`
- ✅ `WBP_Card` (versión Blueprint, a migrar): función `UpdateCardImage`, función `SetCardData`, event dispatchers `OnCardClicked`/`OnCardDragged`

## Orden de implementación (a partir de ahora)

### 1. Configurar módulo C++
- Borrar `Source/UE5_Sample_Project/PopulateDataTableAutomation.cpp` (pseudocódigo huérfano, no usado).
- Usar el asistente **"New C++ Class"** del editor (genera Build.cs, Target.cs y actualiza el .uproject).
- Crear primera clase: `UCardWidget` (hereda de `UUserWidget`).

### 2. Migrar WBP_Card a C++ (`UCardWidget`)
- Propiedades (`UPROPERTY`): `CardData` (S_Card), `CardImage` (puntero a `UImage`, `BindWidget`).
- Funciones (`UFUNCTION`): `UpdateCardImage(UTexture2D* NewTexture)`, `SetCardData(FS_Card NewCard)` (busca en `DT_Cards` y llama a `UpdateCardImage`).
- Event dispatchers: `OnCardClicked`, `OnCardDragged` (delegates `DECLARE_DYNAMIC_MULTICAST_DELEGATE`).
- Reparentar `WBP_Card` para que herede de `UCardWidget`.

### 3. WBP_PileSlot → `UPileSlotWidget`
- Array de `UCardWidget*`.
- Lógica de apilado: offset vertical para columnas tableau, sin offset para fundaciones/stock/waste.
- Reparentar `WBP_PileSlot`.

### 4. GameManager (C++, Actor o GameMode)
- Mazo: array de 52 `FS_Card` + función `ShuffleDeck` (Fisher-Yates).
- Reparto inicial: columnas 1-7 cartas, última boca arriba, resto al Stock.
- Reglas de movimiento:
  - Tableau → Tableau: secuencia descendente alternando colores, sobre carta rango+1 de color contrario.
  - Tableau/Waste → Foundation: As primero, luego ascendente mismo palo.
  - Stock → Waste: clic voltea 1 (o 3) cartas; si Stock vacío, reciclar Waste.
- Auto-flip: al vaciar última carta de columna, voltear la siguiente.

### 5. Drag & Drop
- `OnDragDetected` en `UCardWidget`: crear Drag Drop Operation arrastrando la sub-pila desde la carta clicada.
- `OnDrop` en `UPileSlotWidget`: validar jugada según reglas del paso 4; si inválida, devolver carta a origen.

### 6. Detección de victoria
- Tras cada movimiento a Foundation, comprobar si las 4 Foundations tienen 13 cartas → pantalla de victoria.

### 7. Extras (opcional, después de lo básico)
- Animaciones de volteo (Widget Animation, escala X 1→0→1 con cambio de textura a la mitad).
- Doble clic para auto-mover a Foundation.
- Botón Undo con snapshots del estado del tablero.

## Notas
- Enfoque: híbrido C++ (datos/lógica) + Blueprint/UMG (layout visual, vía reparenting).
- Motivo: mejor control de versiones con Git (texto plano vs binarios .uasset) y mayor comodidad viniendo de Java/C++.
- Visual: UI 2D con UMG/Widgets (no mundo 3D).
