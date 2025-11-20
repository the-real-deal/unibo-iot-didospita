# Notes

variables should be read all at the start of game step, not only the isTimerEnded, and with interrupts disabled

```c
void gameStep(...) { // this is our equivalent of loop
    noInterrupts(); // MISSING
    bool isTimerEnded = ...
    Game currentGame = game // MISSING: copy entire game state
    interrupts() // MISSING
    ...
}
```
