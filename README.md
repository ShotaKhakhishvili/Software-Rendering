# CORE Inventory Plugin

A production-ready, fully optimized event-driven C++ and UMG Inventory & Proximity Interaction system for Unreal Engine 5.6+. 

This plugin handles categorized inventory, coin-based purchases, proximity widget displays, dynamic item scaling, action delegates, and widget reuse out of the box, making it attachable to **any character class**.

---

## Key Features

1. **Modular Architecture**: Attach `UInventoryComponent` to any character to grant them full inventory, shop, and interaction logic.
2. **Zero Blueprint Tick (Optimized)**: Auto-proximity scanning runs at a lightweight 10Hz. Progress bar holds are computed natively without ticking or delta-time overhead.
3. **UMG Slot Reuse**: Reuses scrollbox slot widgets rather than recreating them during UI updates. This allows playing clean, uninterrupted entry/exit/count animations.
4. **Shop & Currency Deduction**: Standard price checking and automatic deduction of coin currency when collecting paid item actors.
5. **Runtime Activation (Chests)**: Disable interaction colliders for items inside a closed chest, and dynamically activate them at runtime.
6. **Data-Driven Visual Scaling**: Apply relative transforms (location/rotation/scale) directly inside each `ItemDataAsset` to handle oversized meshes or off-center pivots.
7. **Null-Safety Guard**: Binds empty slots to a blank data asset, eliminating "Accessed None" warnings in your logs.

---

## 📂 File Architecture

* **`COREInventoryTypes.h`**: Enum definitions (`EItemCategory`) and stacks (`FItemStack`, `FInventorySaveData`).
* **`ItemDataAsset.h`**: Primary Data Asset outlining name, description, max stack size, icon, world mesh, transform, drop flags, hold rules, and price.
* **`ItemActorBase.h`**: The physical item actor placed in the world, managing collision, mesh, dynamic widget components, and outline events.
* **`InventoryComponent.h`**: The backend brain managing arrays, selection state, drop triggers, hold timers, and closest item scans.
* **`InventorySlotWidget.h`**: Base slot widget tracking clicks, hold times, progress percentages, and animation delegates.
* **`InventoryMainWidget.h`**: Base main screen UI managing edibles scrollbox, tools scrollbox, pockets, and EAT/DROP/DROP ALL button visibility.
* **`InventoryHUDWidget.h`**: Base HUD widget showing real-time pocket items and stack sizes on the player's screen.

---

## 🛠️ Step-by-Step Integration

### 1. Setup Input Bindings on your Character
Bind your interaction key (e.g. `E`) in your Character Blueprint to the inventory component:
* **Key Pressed** $\rightarrow$ Get `InventoryComponent` $\rightarrow$ Call `InteractPressed` (runs `PickUpClosestItem` and handles holds).
* **Key Released** $\rightarrow$ Get `InventoryComponent` $\rightarrow$ Call `InteractReleased` (cancels active hold progress).

---

### 2. Creating Items & Mesh Transforms
1. Right-click in Content Browser $\rightarrow$ **Miscellaneous $\rightarrow$ Data Asset** $\rightarrow$ Select `ItemDataAsset`.
2. Configure stack sizes, category, price, and world mesh.
3. If the static mesh is too large or offset from the root collision sphere, adjust the **`WorldMeshTransform`** vector inside the Data Asset. This transform updates immediately in the viewport.

---

### 3. Setting Up a Shop
* Open any `ItemDataAsset` and set `PurchaseCost` to a value greater than `0`.
* Placing this item in the world automatically makes it a paid item.
* When the player tries to collect it, the system checks if they have enough coins in their inventory. If true, it deducts the coins and adds the item. If false, it ignores the interaction.
* *Note: You can override the price of individual actors placed in the level by selecting the actor and changing the `PurchaseCostOverride` field in the Details panel.*

---

### 4. Chest Item Activation / Deactivation
To place items inside a chest that cannot be picked up or highlighted while the chest is closed:
1. Select the item actor in your level viewport and uncheck **`bInteractionEnabled`** in the Details panel.
2. In your Chest opening Blueprint sequence: Get references to the items inside, and call **`SetInteractionEnabled`** with `bEnabled = true`.
3. The items will instantly turn on their collision spheres and become visible to the player's range scanner.
4. *Hold-Safe*: If an item is disabled while the player is currently holding the interaction button to pick it up, the interaction is aborted automatically.

---

### 5. Slot Widget Animations (`WBP_InventorySlot`)
Create a custom Slot Widget Blueprint (`WBP_InventorySlot`) inheriting from `InventorySlotWidget`.
Override the following Event Nodes in the Graph to play custom widget animations:
* **`Event On Item Added`**: Called when an item is newly put into this slot. (Play a pop-in scale-bounce animation).
* **`Event On Item Removed`**: Called when the slot is emptied. (Play a shrink or fade-out animation).
* **`Event On Count Changed (OldCount, NewCount)`**: Called when the same item stack changes size. (Play a scale pulse; flash green if added, red if removed).
* **`Event On Slot Initialized (bHasItem)`**: Standard refresh node.

---

### 6. Event Listening (Quest/Mission Progress)
To progress quests when items are consumed:
1. Get `InventoryComponent` $\rightarrow$ Call **`Bind Event to On Item Consumed`**.
2. Connect it to a custom event which receives an `Item Data` parameter.
3. Compare `Item Data` using an `==` (Equal - Object) node against your target item (e.g. `DA_Apple`).
4. Connect the result to a `Branch` to trigger your Quest Stage progression.

```mermaid
graph TD
    A[OnItemConsumed Delegate] --> B[Custom Event: OnPlayerAteItem]
    B --> C["Branch (Is Consumed Item == DA_Apple?)"]
    C -- True --> D[Advance Mission Objective]
    C -- False --> E[Ignore (Player ate something else)]
```

---

### 7. Level Transitions (Save & Load)
Save inventory states cleanly across levels:
* **Before Leaving a Level**: Call `GetInventorySaveData` on the `InventoryComponent`. Save this struct to a variable on your custom **Game Instance**.
* **On Spawning in the New Level**: Get the saved struct from the **Game Instance** and pass it into `LoadInventorySaveData` on your new character's `InventoryComponent`. All slots, hotbars, and UI will restore automatically.

---

## 💻 C++ Dependencies (If using C++ in your main project)
If you want to access the plugin classes inside your main project's C++ code, open your project's `.Build.cs` file and add `"COREInventory"` to your dependencies:
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "COREInventory" });
```

---

## 🖋️ Credits
Plugin friendly name: **CORE Inventory**  
Author/Developer: **Konstantine**
