# Grafemsko fonemska pretvorba

Repozitorij vsebuje izvorno kodo orodja za pretvorbo besedila iz **normiranega** grafemskega v fonemski zapis.

## Prevajanje

Izvorno kodo prevedemo bodisi z Visual Studiom, za katerega je že pripravljena projektna datoteka [rsdo-gfp.sln](./rsdo-gfp.sln), bodisi s prevajalnikom GCC. V slednjem primeru prevajanje izvedemo z naslednjimi ukazi:

```
mkdir build; cd build
cmake ..
make
```

## Primer uporabe

```
rsdo-gfp.exe -d .. ..\test.txt
```

Programu podamo ime besedilne datoteke (v utf-8 zapisu), ki jo želimo pretvoriti v fonemski zapis, z argumentom ```-d``` pa podamo pot do direktorija, kjer se nahaja datoteka [rsdo-gfp.ini](./rsdo-gfp.ini). Pretvorjeno datoteko bo program izpisal na standardni izhod.