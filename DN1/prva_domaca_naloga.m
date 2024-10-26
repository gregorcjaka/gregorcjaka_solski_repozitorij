% 1. naloga - uvozimo podatke z importdata in jih shranimo v vektor t
filename = "naloga1_1.txt";
headerlinesIn = 2; % Spustimo prvi 2 vrstici
delimiterIn = " ";
data = importdata(filename,delimiterIn, headerlinesIn);
t = data.data(:, 1);

% 2. naloga
filename2 = "naloga1_2.txt";
fid = fopen(filename2);
line = fgetl(fid);

% Da preberemo stevilo vrstic iz prve vrstice lahko uporabimo for loop, tako da gremo
% cez vse znake v prvi vrstici in iscemo tiste, ki so cifre in ne drugi
% znaki. Vrednosti(posamezne stevke) shranimo v array, ki ga nato zdruzimo, da dobimo 
% eno stevilko. Verjetno obstaja veliko hitrejsi in boljsi nacin, ampak se
% lahko s funkcijami, ki smo jih pokazali spomnim le tega. Vredno omembe je to, da ce bi 
% imeli v prvi vrstici se katerekoli druge stevke, bi s tem nacinom naleteli na
% tezave, saj bi tiste stevke prav tako zajeli v nasem arrayu.
stevke = [];
a = 1;  % Pomozen parameter, ki oznacuje pod kateri indeks shranimo cifro v "stevke".

for i = 1:length(line)
    if isstrprop(line(i), 'digit')
        stevke(a) = line(i) - '0'; % Odstejemo "0", ker dobimo vrednosti v ASCII zapisu
        a = a + 1;
    end
end

stevilo_vrstic = str2double(join(string(stevke), ''));

% Lotimo se branja vrednosti iz datoteke. Uporabimo podoben nacin kot prej.
% Ker smo ze prej enkrat poklicali fgetl, bomo tu avtomatsko zaceli z
% branjem pri drugi vrstici. Nato narisemo graf.
% Pri definiciji vektorja P, moramo biti pazljivi, da dobimo 100x1 vektor
% in ne 1x100, da se bo skladal z vektorjem t.
P = zeros(stevilo_vrstic, 1);

for i = 1:stevilo_vrstic
  line1  = fgetl(fid);
  P(i) = str2double(line1);
end

plot(t, P, "r", "LineWidth", 2)
grid on
title("Graf P(t)");
xlabel("t[s]");
ylabel("P[W]");
ax = gca;
ax.FontSize = 15;

fclose(fid);

% 3. naloga
trapz_funkcija = trapz(t, P); % Built in funkcija trapz integrala

% Napisemo naso trapz funkcijo. Po formuli za trapezno metodo najprej
% s for loopom sestejemo vsoto funkcijskih vrednosti. Ce je element prvi v
% P vektorju, ga ne mnozimo z 2, prav tako velja za zadnji element, kjer pa
% poleg pristevanja zadnjega elementa se mnozimo s korakom in delimo z 2,
% da dobimo koncno vrednost.
vrednost_integrala = 0;

for i = 1:length(P)
    if i == 1
        vrednost_integrala = vrednost_integrala + P(i);
    elseif i == length(P)
        vrednost_integrala = vrednost_integrala + P(i);
        vrednost_integrala = vrednost_integrala * (t(2) - t(1)) / 2;
    else
        vrednost_integrala = vrednost_integrala + 2 * P(i);
    end
end

razlika = trapz_funkcija - vrednost_integrala;
% Ugotovimo, da je razlika enaka 0, kar pomeni da smo dobili enako
% vrednost, kot jo je dobila built in funkcija trapz.