# reproduction

**Warning:** лабу лучше делать на windows xp на виртуалке.

Нас интересует прерывание int8h.

Длина вектора прерывания - 4 байта. 4 * 8 = 32dec = 20h

Открываем в windows xp cmd
```cmd
DEBUG
D 0000:0020 L 4
```

Скачиваем где то sourcerer - SR.exe

```cmd
b *аддрес в обратном порядке*
```

```cmd
e ffff
```

```cmd
g
```

Все, получаем листинг!