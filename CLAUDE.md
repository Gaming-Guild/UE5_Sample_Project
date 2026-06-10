# Klondike Solitaire en UE5 (Blueprints + UMG)

Guía paso a paso para implementar un Klondike Solitaire usando Blueprints y widgets UMG (UI 2D).

## Orden de implementación

### 1. Datos de las cartas
- Crear Enum `E_CardSuit`: Hearts, Diamonds, Clubs, Spades.
- Crear Struct `S_Card`: `Suit` (E_CardSuit), `Rank` (Integer 1-13), `bIsFaceUp` (Boolean).

### 2. Imágenes de cartas
- Crear carpeta `Content/UI/Cards`.
- Importar 52 imágenes (frente) + 1 imagen de reverso.
- Compression Settings: `UserInterface2D (RGBA)`.

### 3. Data Table de visuales
- Crear Struct `S_CardVisual`: `Suit` (E_CardSuit), `Rank` (Integer), `CardTexture` (Texture2D).
- Crear Data Table basada en `S_CardVisual` con las 52 filas (Suit+Rank → Textura).

### 4. Widgets base
- `WBP_Card`: Image (frente/dorso) + botón invisible para clic, variables `CardData` (S_Card), eventos `OnCardClicked`, `OnCardDragged`.
- `WBP_PileSlot`: contenedor con array de `WBP_Card`, lógica de apilado (con offset vertical para columnas, sin offset para fundaciones).
- `WBP_GameBoard`: layout completo - 1 Stock, 1 Waste, 4 Foundations, 7 columnas Tableau.

### 5. Lógica del juego (GameMode/GameInstance/Actor "GameManager")
- Mazo: array de 52 `S_Card` + función `ShuffleDeck` (Fisher-Yates, RandomIntegerInRange).
- Reparto inicial: columnas 1-7 cartas, última boca arriba, resto al Stock.
- Reglas de movimiento:
  - Tableau → Tableau: secuencia descendente alternando colores, sobre carta rango+1 de color contrario.
  - Tableau/Waste → Foundation: As primero, luego ascendente mismo palo.
  - Stock → Waste: clic voltea 1 (o 3) cartas; si Stock vacío, reciclar Waste.
- Auto-flip: al vaciar última carta de columna, voltear la siguiente.

### 6. Drag & Drop
- `OnDragDetected` en `WBP_Card`: crear Drag Drop Operation arrastrando la sub-pila desde la carta clicada.
- `OnDrop` en `WBP_PileSlot`: validar jugada según reglas del paso 5; si inválida, devolver carta a origen.

### 7. Detección de victoria
- Tras cada movimiento a Foundation, comprobar si las 4 Foundations tienen 13 cartas → pantalla de victoria.

### 8. Extras (opcional, después de lo básico)
- Animaciones de volteo (Widget Animation, escala X 1→0→1 con cambio de textura a la mitad).
- Doble clic para auto-mover a Foundation.
- Botón Undo con snapshots del estado del tablero.

## Notas
- Enfoque: solo Blueprints, sin C++.
- Visual: UI 2D con UMG/Widgets (no mundo 3D).
