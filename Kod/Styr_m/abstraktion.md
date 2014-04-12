Klasser
=========

- Map - "30x15-ish-matris"

- Basklass

- ExploredSection
- UnexploredSection
- Robot
- UnreachableSection
- Fire

Funktioner
==========

Styrning
===

- Välj växel (fwd, bwd, right, left)
- Kör med gaspådrag i %
- Sväng i antal grader

SLAM
===

- Sätt/Ersätt in sektion
- Sätt UnreachableSection på avstånd där sensor säger vägg (position n+1)
- Uppdatera robots position
- Sätt Fire i sektion där RFID-sensor detekterar taggar.
- Hitta närmaste outforskade område
- Åk till närmaste outforskade, om det finns flera lika nära (2 rutors felmarginal), åk till det som är mest rakt fram

Algoritmer
=======
- Endast kartläggning
- Brandhärdsfinnare
