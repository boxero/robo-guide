robo-guide - robot przewodnik,
==========
Projekt realizowany przez Naukowe Koło Robotyki, Wydział Matematyki i Informatyki, UWM,

Wymagany komponenty:
- projekt jest tworzony w Visual Studio 2010,
- aby projekt kompilował się, na dysk C należy rozpakować bibliotekę:
http://wmii.uwm.edu.pl/~artem/nxtpp0-6-3.zip (w innej lokalizacji wymaga podlinkowania w VS),
- do sterowania robotem potrzebne są sterowniki Mindstorms NXT, NXT_32 lub NXT_64,
NXT_32:  http://wmii.uwm.edu.pl/~artem/NXT_32bit_driver.zip,
NXT_64:  http://wmii.uwm.edu.pl/~artem/NXT_64bit_driver.zip.

w projekcie mamy następujące moduły i pliki źródłowe,

main.cpp - podstawowe sterowanie pozwalające na zdalną kontrolę, oraz kod uruchamiany klawiszem 'z' włączający lokalizację w budynku,
- obszary budynku opisane są w formacie .bmf, który można wczytywać na bieżąco gdy robot zmienia rejon w którym się lokalizuje,
w plikach jest rozmiar mapy, inicjacyjna pozycja docelowa, mapa odległości od ścian, mapa ograniczeń pól, mapa odchyleń magnetycznych, oraz mapa pozycji przeszkód,
przeszkody są zaznaczane na tej ostatniej wartością 5,  na pozostałych mapach ściany są zaznaczone jako 0.

map_reader.h map_reader.cpp - do odczytywania plików mapy:
funkcje:
size_reader(...) odczytująca rozmiar mapy i inicjacyjna pozycje docelową,
map_reader(...) która wczytuje do tablic pozostałe dane mapy.

drive.h drive.cpp - sterowanie,
funkcje:
turn_to_specified_map_direction(); - skręcanie na mapie w określonym kierunku
drive_PD_in_specified_map_direction(); - jazda na mapie w określonym kierunku
drive_PD_in_specified_map_direction_between_walls(); - jazda na mapie w określonym kierunku pomiędzy ścianami,
drive_PD_left_wall(); - jazda na mapie w określonym kierunku śledząc lewą ścianę,
il_takt(); - funkcja szacuje liczbę taktów do przejechania, w celu zbliżenia się do ustalonego celu,
takt to czas w jakim robot przejeżdża ustalony odcinek drogi.

histogram_filter.h  histogram_filter.cpp - filtracja histogramowa,
funkcje:
mapa_init_prob(); - inicjuje prawdopodobieństwo lokalizacji we wszystkich polach mapy wartością 1/liczba dostępnych pól,
p_update_after_sense(); - odświeżamy prawdopodobieństwo położenia robota po pomiarze z sensorów - reguła Bayesa,
p_update_after_move(); - odświeżamy prawdopodobieństwo położenia robota po splocie - prawdopodobieństwo całkowite.

magnetic_anomalies.h  magnetic_anomalies.cpp - obsługa anomalii magnetycznych,
funkcje:
next_north_direction(); - ustalamy nowy kierunek północny po przejechaniu ustalonej liczby taktów, dzięki temu podczas potwierdzania położenia, mamy zniwelowany problem zmiany odczytu kierunku N,
emergency_north_direction(); - tutaj mamy funkcję która oblicza awaryjny kierunek północny, ma służyć w sytuacji gdy robot wykryje, że jest przenoszony.

move_policy.h move_policy.cpp - polityka poruszania się po mapie,
funkcje:
move_policy(); - czyli ustalanie sposobu poruszania się po mapie,

obstacle_avoidance.h obstacle_avoidance.cpp - unikanie przeszkód
funkcje:
odjedz_od_scian(...);

PID-controler.h PID-controler.cpp
funkcje:
convert_orientation();

radar.h radar.cpp
funkcje:
sense(...); - służy do pobierania dystansu od przeszkód na lewo, prawo i wprost za pomocą radaru obrotowego, wymaga odrębnej konstrukcji radaru, sonaru umieszczonego na serwo w OUT_A
sense2(...); - jest teraz używana, odczyt za pośrednictwem Multiplexera czujników, do którego są podłączone sonary,

additions.h additions.cpp
funkcje:
zapisz_do_pliku(...);
zapisz_tab2d_do_pliku(...);
