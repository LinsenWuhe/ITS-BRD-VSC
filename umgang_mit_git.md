
### Schritt A: Update holen
Bevor du startest, ziehst du dir die Änderungen deiner Kollegen:
```bash
git pull origin main
```
*(Hinweis: Manchmal heißt der Hauptzweig noch `master`, meistens aber `main`.)*

### Schritt B: Einen eigenen "Feature Branch" erstellen
Arbeite **niemals** direkt auf dem Hauptzweig, wenn du mit anderen zusammenarbeitest. Erstelle eine eigene "Abzweigung":
```bash
git checkout -b mein-neues-feature
```


---

## 2. Deine Arbeit speichern und hochladen

Wenn du mit deinen Änderungen fertig (oder an einem guten Zwischenpunkt) bist, gehst du so vor:

1.  **Status prüfen:** Schau nach, was du verändert hast.
    * `git status`
2.  **Dateien vormerken:** Packe die Änderungen in den "Sammelkorb".
    * `git add .` (fügt alles hinzu) oder `git add dateiname.js`
3.  **Commit erstellen:** Den Stand mit einer Nachricht beschreiben.
    * `git commit -m "Layout der Startseite angepasst"`
4.  **Hochladen (Push):** Schicke deinen Branch ins Internet (zum Server).
    * `git push origin mein-neues-feature`

---

## 3. Was tun bei unterschiedlichen Versionen? (Merge Conflicts)

Das ist der Moment, vor dem viele Angst haben, der aber völlig normal ist: **Der Konflikt.** Er entsteht, wenn du und ein Kollege **dieselbe Zeile** in derselben Datei geändert habt.

### So löst du ihn:
1.  Git wird dir beim `git pull` oder beim Zusammenführen (Merge) sagen: *CONFLIT (content): Merge conflict in...*
2.  Öffne die betroffene Datei. Du wirst solche Zeichen sehen:
    ```text
    <<<<<<< HEAD
    Meine Änderung (Ich will Blau)
    =======
    Änderung vom Kollegen (Er will Rot)
    >>>>>>> main
    ```
3.  **Entscheide dich:** Lösche die Zeichen (`<<<<`, `====`, `>>>>`) und behalte nur den Code, der gewinnen soll (oder kombiniere beide).
4.  **Speichern & Committen:**
    * `git add .`
    * `git commit -m "Merge-Konflikt gelöst"`

---

## 4. Die Checkliste für Teams

| Situation | Befehl | Warum? |
| :--- | :--- | :--- |
| **Zwischendurch schauen** | `git log --oneline` | Um zu sehen, was zuletzt passiert ist. |
| **Arbeit kurz parken** | `git stash` | Wenn du den Branch wechseln musst, aber noch nicht fertig zum Committen bist. |
| **Arbeit zurückholen** | `git stash pop` | Um die geparkte Arbeit weiterzuführen. |
| **Branch wechseln** | `git checkout main` | Um zurück zum Hauptzweig zu springen. |

---

### Ein wichtiger Tipp zum Schluss:
Nutzt **Pull Requests** (auf GitHub/GitLab). Anstatt deinen Code direkt in den `main` zu mergen, stellst du online einen Antrag. Dein Kollege schaut drüber, gibt seinen Senf dazu, und erst wenn alles passt, wird der Code vereinigt. Das hält den Code sauber und schont die Nerven!

